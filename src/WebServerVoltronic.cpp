/* WebServerVoltronic library by Benoit Galmiche
*/

#include "WebServerVoltronic.h"
#include <WebServer.h> 

WebServer wserver(80);

WebServerVoltronic::WebServerVoltronic(){}

WebServerVoltronic::~WebServerVoltronic(){}

void WebServerVoltronic::setup(){
  wserver.on("/", this->buildHomePage);  //Homepage loading.
  wserver.onNotFound(this->buildNotFoundPage);  // Notfound page loading.
  wserver.begin();
}

void WebServerVoltronic::handleClient(){
  wserver.handleClient();
}

void WebServerVoltronic::buildHomePage(){
    char page[] ="<!DOCTYPE html>";
    strcat(page, "<html lang='fr'>");
    strcat(page, "<head>");
    strcat(page, "    <title>Serveur ESP32</title>");
    strcat(page, "    <meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1' charset='UTF-8'/>");
    strcat(page, "    <link rel='stylesheet' href='https://www.w3schools.com/w3css/4/w3.css'>");  // Utilisation du css);
    strcat(page, "</head>");
    strcat(page, "<body>");
    strcat(page, "    <div class='w3-card w3-blue w3-padding-small w3-jumbo w3-center'>");
    //strcat(page, "        <p>ÉTAT LED: "; page += texteEtatLed[etatLed]; page += "</p>");
    strcat(page, "        <p>ÉTAT LED</p>");
    strcat(page, "    </div>");
    strcat(page, "    <div class='w3-bar'>");
    strcat(page, "        <a href='/on' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:50%; height:50%;'>ON</a>");
    strcat(page, "        <a href='/off' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:50%; height:50%;'>OFF</a>");
    strcat(page, "    </div>");
    strcat(page, "    <div class='w3-center w3-padding-16'>");
    strcat(page, "        <p>Ce serveur est hébergé sur un ESP32</p>");
    strcat(page, "        <i>Créé par Tommy Desrochers</i>");
    strcat(page, "    </div>");
    strcat(page, "</body>");
    strcat(page, "</html>");
    
    wserver.setContentLength(sizeof(page)); // Permet l'affichage plus rapide après chaque clic sur les boutons
    wserver.send(200, "text/html", page);

}

void WebServerVoltronic::buildNotFoundPage(){

    wserver.send(404, "text/plain","404: Not found");
}

