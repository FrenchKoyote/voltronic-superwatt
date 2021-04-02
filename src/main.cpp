//https://github.com/r0oland/ESP32_mini_KiCad_Library
//https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/HardwareSerial.cpp

#include <Arduino.h>
#include <ArduinoJson.h>
#include "WatchPower.h"
#include <WiFi.h>
#include <stdio.h>
#include <PubSubClient.h>
#include <JsonVoltronic.h>
#include <WebServer.h>

TaskHandle_t Task1;
TaskHandle_t Task2;

char ssid[] = "Livebox-A784";         //  your network SSID (name)
char pass[] = "cxvhQN9JvmHFQPoe52";   // your network password

HardwareSerial SerialSuperWatt(2);
WatchPower watchPower(SerialSuperWatt);

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
.card{
    max-width: 400px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>
<body>

<div class="card">
  <h4>Onduleur Superwatt</h4><br>
  <h4>Mode de chargement des batteries : <span id="ChargingValue">0</span></h4><br>
</div>

<button oncLick="setChargePrioritySolar()"> Par solaire </button>
<button oncLick="setChargePriorityUtility()"> Par reseau </button>

<div class="card">
  <h4>Onduleur Superwatt</h4><br>
  <h4>Mode de sortie életrique : <span id="OuputValue">0</span></h4><br>
</div>

<button oncLick="setOutputPrioritySolar()"> Par solaire </button>
<button oncLick="setOutputPriorityUtility()"> Par reseau </button>

<script>

function setChargePriorityUtility() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ChargingValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "setChargePriorityUtility", true);
  xhttp.send();
}

function setChargePrioritySolar() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ChargingValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "setChargePrioritySolar", true);
  xhttp.send();
}

function setOutputPrioritySolar() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("OuputValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "setOutputPrioritySolar", true);
  xhttp.send();
}

function setOutputPriorityUtility() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("OuputValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "setOutputPriorityUtility", true);
  xhttp.send();
}


</script>
</body>
</html>
)=====";

WebServer server(80);
   
// Initialize the client library
WiFiClient wifi_client;
PubSubClient pubsub_client(wifi_client);
JsonVoltronic jmess;

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

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

void setChargePriorityUtility() {
 
 Serial.println("inside setChargePriorityUtility");

 String s="";

 if(watchPower.setChargePriority(WatchPower::ChargePriorities::UtilityFirst))
 {
   s = "Chargement par réseau appliqué.";
 }
 else
 {
   s = "Chargement par réseau non appliqué.";
 }
  
 server.send(200, "text/plain", s);

 Serial.println("outside setChargePriorityUtility");
}

void setChargePrioritySolar() {
 
 Serial.println("inside setChargePrioritySolar");

 String s="";

 if(watchPower.setChargePriority(WatchPower::ChargePriorities::SolarFirst))
 {
   s = "Chargement par solaire appliqué.";
 }
 else
 {
   s = "Chargement par solaire non appliqué.";
 }
  
 server.send(200, "text/plain", s);

 Serial.println("outside setChargePrioritySolar");
}

void setOutputPrioritySolar() {
 
 Serial.println("inside setOutputPrioritySolar");

 String s="";

 if(watchPower.setOutputSourcePriority(WatchPower::OutputSourcePriorities::SolarFirst))
 {
   s = "Sortie par solaire appliquée.";
 }
 else
 {
   s = "Sortie par solaire non appliquée.";
 }
  
 server.send(200, "text/plain", s);

 Serial.println("outside setOutputPrioritySolar");
}


void setOutputPriorityUtility() {
 
 Serial.println("inside setOutputPriorityUtility");

 String s="";

 if(watchPower.setOutputSourcePriority(WatchPower::OutputSourcePriorities::UtilityFirst))
 {
   s = "Chargement par réseau appliqué.";
 }
 else
 {
   s = "Chargement par réseau non appliqué.";
 }
  
 server.send(200, "text/plain", s);

 Serial.println("outside setOutputPriorityUtility");
}


void Task1code( void * pvParameters ){
  Serial.println("Task1 running on core ");
  Serial.print(xPortGetCoreID());

  for(;;){
      server.handleClient();
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

  server.on("/", handleRoot);      //This is display page
  server.on("/setChargePriorityUtility", setChargePriorityUtility); 
  server.on("/setChargePrioritySolar", setChargePrioritySolar); 
  server.on("/setOutputPriorityUtility", setOutputPriorityUtility); 
  server.on("/setOutputPrioritySolar", setOutputPrioritySolar); 
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");

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

void loop() {

  
} 