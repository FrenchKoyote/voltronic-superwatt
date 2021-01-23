//https://github.com/r0oland/ESP32_mini_KiCad_Library
//https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/HardwareSerial.cpp

//For debugging
//sudo apt-get install libncurses5 libncurses5:i386
//sudo apt install libpython2.7

//Configure plateformio.ini
/*debug_tool = custom
debug_init_break=
debug_server =
 $PLATFORMIO_HOME_DIR/packages/tool-openocd-esp32/bin/openocd
 -s
 $PLATFORMIO_HOME_DIR/packages/tool-openocd-esp32/share/openocd/scripts/
 -f
 $PLATFORMIO_HOME_DIR/packages/tool-openocd-esp32/share/openocd/scripts/interface/ftdi/digilent_jtag_smt2.cfg
 -f
 $PLATFORMIO_HOME_DIR/packages/tool-openocd-esp32/share/openocd/scripts/board/esp-wroom-32.cfg
 -c 'ftdi_vid_pid 0x0403 0x6011'
debug_load_cmd=preload*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include "WatchPower.h"
#include <WiFi.h>
#include <stdio.h>
#include <PubSubClient.h>
#include <JsonVoltronic.h>

char ssid[] = "Livebox-A784";    //  your network SSID (name)
char pass[] = "cxvhQN9JvmHFQPoe52";   // your network password

HardwareSerial SerialSuperWatt(2);
WatchPower watchPower(SerialSuperWatt);

// Initialize the client library
WiFiClient wifi_client;
PubSubClient pubsub_client(wifi_client);
JsonVoltronic jmess;

void reconnect() {
  // Loop until we're reconnected
  while (!pubsub_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (pubsub_client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      //pubsub_client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(pubsub_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  //SerialSuperWatt.begin(2400, SERIAL_8N1, 16, 17);

  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if(WiFi.status()  == WL_CONNECTED) {

    IPAddress addr(192, 168 ,1 ,22);
    pubsub_client.setServer(addr, 1883);

    while (!pubsub_client.connected())
    {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (pubsub_client.connect("ESP32Client")) {
        Serial.println("connected");
      } else {
        Serial.print("failed, rc=");
        Serial.print(pubsub_client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }
}

void loop() {

  /* Refresh the stats */
  bool error = watchPower.refreshData();

  if(error) {
    Serial.println("************** error detected ! **************");
  }
  /* Print stats from the inverter to the serial console */
  /*Serial.print("Solar Current: ");
  Serial.println(watchPower.solarCurrent.str);

  Serial.print("Solar Voltage: ");
  Serial.println(watchPower.solarVoltage.str);*/

  
  if (!pubsub_client.connected() || !wifi_client.connected()) {
    reconnect();
  }

  // Create QPGS1 JSON and send message
  // By default PubSubClient limits the message size to 256 bytes (including header), see the documentation
  char JSONmessageBuffer[500];
  int n = jmess.buildQPGIS1(& watchPower, JSONmessageBuffer);

  Serial.println("Sending QPGIS1 message to MQTT topic..");
  Serial.println(JSONmessageBuffer);

  if (pubsub_client.publish("solar/QPIGS1", JSONmessageBuffer, n) == true) {
    Serial.println("Success sending QPGIS1 message");
  } else {
    Serial.println("Error sending QPGIS1 message");
  }

  //Empty JSONmessageBuffer
  memset(JSONmessageBuffer, 0, sizeof(JSONmessageBuffer));

  // Create QPGS2 JSON and send message
  n = jmess.buildQPGIS2(& watchPower, JSONmessageBuffer);

  Serial.println("Sending QPGIS2 message to MQTT topic..");
  Serial.println(JSONmessageBuffer);

  if (pubsub_client.publish("solar/QPIGS2", JSONmessageBuffer, n) == true) {
    Serial.println("Success sending QPGIS2 message");
  } else {
    Serial.println("Error sending QPGIS2 message");
  }
 
  pubsub_client.loop();
  Serial.println("-------------");
 
  delay(10000);
} 