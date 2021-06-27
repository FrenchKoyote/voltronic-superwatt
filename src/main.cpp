//https://github.com/r0oland/ESP32_mini_KiCad_Library
//https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/HardwareSerial.cpp

#include <Arduino.h>
#include <ArduinoJson.h>
#include "WatchPower.h"
#include <WiFi.h>
#include <stdio.h>
#include <PubSubClient.h>
#include <JsonVoltronic.h>
#include <WebServerVoltronic.h>

TaskHandle_t Task1;
TaskHandle_t Task2;

char ssid[] = "Livebox-A784";         //  your network SSID (name)
char pass[] = "cxvhQN9JvmHFQPoe52";   // your network password

HardwareSerial SerialSuperWatt(2);
WatchPower watchPower(SerialSuperWatt);

WebServerVoltronic webserver(watchPower);

// Initialize the client library
WiFiClient wifi_client;
PubSubClient pubsub_client(wifi_client);
JsonVoltronic jmess;

void wifiConnection()
{
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
}

void mqttConnection()
{
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

void reconnect() {
  wifiConnection ();
  mqttConnection();

  webserver.setup();
}

void Task1code( void * pvParameters ){
  Serial.println("Task1 running on core ");
  Serial.print(xPortGetCoreID());

  for(;;){
      webserver.handleClient();
      delay(1);
  } 
}

void Task2code( void * pvParameters ){
  Serial.println("Task2 running on core ");
  Serial.print(xPortGetCoreID());

  for(;;){
      /* Refresh the stats */
      bool error = watchPower.refreshData();

      if(error) {
        Serial.println("************** error detected ! **************");
      }
        
      if (!pubsub_client.connected() || !wifi_client.connected()) {
        reconnect();
      }

      // Create QPGS1 JSON and send message
      // By default PubSubClient limits the message size to 256 bytes (including header), see the documentation
      char JSONmessageBuffer[500];
      int n = jmess.buildQPGIS1(& watchPower, JSONmessageBuffer);

      Serial.println("Sending QPGIS1 message to MQTT topic..");
      //Serial.println(JSONmessageBuffer);

      // Send QPIGS1 MQTT message 
      if (pubsub_client.publish("solar/QPIGS1", JSONmessageBuffer, n) == true) {
        Serial.println("Success sending QPGIS1 message");
      } else {
        Serial.println("Error sending QPGIS1 message");
      }

      //Empty JSONmessageBuffer
      memset(JSONmessageBuffer, 0, sizeof(JSONmessageBuffer));

      // Create QPGS2 JSON and send message
      // By default PubSubClient limits the message size to 256 bytes (including header), see the documentation
      n = jmess.buildQPGIS2(& watchPower, JSONmessageBuffer);

      Serial.println("Sending QPGIS2 message to MQTT topic..");
      //Serial.println(JSONmessageBuffer);

      // Send QPIGS2 MQTT message 
      if (pubsub_client.publish("solar/QPIGS2", JSONmessageBuffer, n) == true) {
        Serial.println("Success sending QPGIS2 message");
      } else {
        Serial.println("Error sending QPGIS2 message");
      }

      //Empty JSONmessageBuffer
      memset(JSONmessageBuffer, 0, sizeof(JSONmessageBuffer));

      // Create QPGS3 JSON and send message
      // By default PubSubClient limits the message size to 256 bytes (including header), see the documentation
      n = jmess.buildQPGIS3(& watchPower, JSONmessageBuffer);

      Serial.println("Sending QPGIS3 message to MQTT topic..");
      //Serial.println(JSONmessageBuffer);

      // Send QPIGS2 MQTT message 
      if (pubsub_client.publish("solar/QPIGS3", JSONmessageBuffer, n) == true) {
        Serial.println("Success sending QPGIS3 message");
      } else {
        Serial.println("Error sending QPGIS3 message");
      }
    
      pubsub_client.loop();
      Serial.println("-------------");
    
      //Wait 1 seconds
      delay(1000);
    }
}


void setup() {
  Serial.begin(115200);

  wifiConnection();

  if(WiFi.status()  == WL_CONNECTED) {

    IPAddress addr(192, 168 ,1 ,22);
    pubsub_client.setServer(addr, 1883);

    mqttConnection();
  }

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 



  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 
}

void loop() { } 