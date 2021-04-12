/* WebServerVoltronic library by Benoit Galmiche
*/

#include "WebServerVoltronic.h"
#include <WebServer.h> 
#include <WatchPower.h>

WebServer wserver(80);

WatchPower* WebServerVoltronic::refWat;

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
<button oncLick="setChargePriorityUtility()"> Par r&eacute;seau </button>

<div class="card">
  <h4>Onduleur Superwatt</h4><br>
  <h4>Mode de alimentation des r&eacute;cepteurs : <span id="OuputValue">0</span></h4><br>
</div>

<button oncLick="setOutputPrioritySolar()"> Par solaire </button>
<button oncLick="setOutputPriorityUtility()"> Par r&eacute;seau </button>

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

function getOutputPriority() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("OuputValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "getOutputPriority", true);
  xhttp.send();
}

function getChargePriority() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ChargingValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "getChargePriority", true);
  xhttp.send();
}

function getStatus() {
 getOutputPriority();
 getChargePriority();
}

window.onload = getStatus;
</script>
</body>
</html>
)=====";

WebServerVoltronic::WebServerVoltronic(WatchPower &_refWat){
  refWat = &_refWat;
}

WebServerVoltronic::~WebServerVoltronic(){}

void WebServerVoltronic::setup(){
  wserver.on("/", buildHomePage);      //This is display page
  wserver.on("/setChargePriorityUtility", setChargePriorityUtility); 
  wserver.on("/setChargePrioritySolar", setChargePrioritySolar); 
  wserver.on("/setOutputPriorityUtility", setOutputPriorityUtility); 
  wserver.on("/setOutputPrioritySolar", setOutputPrioritySolar); 

  wserver.on("/getOutputPriority", getOutputPriority); 
  wserver.on("/getChargePriority", getChargePriority); 
 
  wserver.begin();                  //Start server
  Serial.println("HTTP server started");
}

void WebServerVoltronic::handleClient(){
  wserver.handleClient();
}

void WebServerVoltronic::buildHomePage(){
  String s = MAIN_page; //Read HTML contents
  wserver.send(200, "text/html", s); //Send web page
}

void WebServerVoltronic::buildNotFoundPage(){
    wserver.send(404, "text/plain","404: Not found");
}

void WebServerVoltronic::setChargePriorityUtility() {

 String s="";

 if(refWat->setChargePriority(WatchPower::ChargePriorities::UtilityFirst)) {
   s = "Chargement par r&eacute;seau appliqu&eacute;.";
 }
 else {
   s = "Chargement par r&eacute;seau non appliqu&eacute;.";
 }
  
 wserver.send(200, "text/plain", s);
}

void WebServerVoltronic::setChargePrioritySolar() {

 String s="";

 if(refWat->setChargePriority(WatchPower::ChargePriorities::SolarFirst)) {
   s = "Chargement par solaire appliqu&eacute;.";
 }
 else {
   s = "Chargement par solaire non appliqu&eacute;.";
 }
  
 wserver.send(200, "text/plain", s);
}

void WebServerVoltronic::setOutputPrioritySolar() {

 String s="";

 if(refWat->setOutputSourcePriority(WatchPower::OutputSourcePriorities::SolarFirst)) {
   s = "Sortie par solaire appliqu&eacute;e.";
 }
 else {
   s = "Sortie par solaire non appliqu&eacute;e.";
 }
  
 wserver.send(200, "text/plain", s);
}


void WebServerVoltronic::setOutputPriorityUtility() {

 String s="";

 if(refWat->setOutputSourcePriority(WatchPower::OutputSourcePriorities::UtilityFirst)) {
   s = "Chargement par r&eacute;seau appliqu&eacute;.";
 }
 else {
   s = "Chargement par r&eacute;seau non appliqu&eacute;.";
 }

 wserver.send(200, "text/plain", s);
}

void WebServerVoltronic::getOutputPriority() {

  String s="";

  if(refWat->isOnBattery()) {
    s = "Alimentation des r&eacute;cepteurs par batterie.";
  }
  else if(refWat->isOnGrid()) {
    s = "Alimentation des r&eacute;cepteurs par secteur.";
  }

   wserver.send(200, "text/plain", s);
}

void WebServerVoltronic::getChargePriority() {
   
  String s="";

  if(refWat->isSolarCharging()) {
    s = "Chargement des batteries par solaire.";
  }
  else if(refWat->isGridCharging()) {
    s = "Chargement des batteries par secteur.";
  }

   wserver.send(200, "text/plain", s);
}

