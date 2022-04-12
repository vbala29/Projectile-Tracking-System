/**
 * @brief Web Server for ESP8266 Microcontroller
 * @author Developed by Vikram Bala, C'24 University of Pennsylvania
 * @date April 12th, 2022
 * @cite https://lastminuteengineers.com/creating-esp8266-web-server-arduino-ide/
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "AirPennNet-Device"; 
const char* password = "penn1740wifi";  

//Class C subnet, where the last octet is the host address
//and the first 3 octets are the network address
IPAddress subnet(255,255,255,0); 

//Start the server on port 80
ESP8266WebServer server(80);


void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);

   //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  
  server.on("/", handle_OnConnect);
  server.on("/laserOn", handle_laserOn);
  server.on("/laserOff", handle_laserOff);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); 
}

void handle_OnConnect() {
  server.send(200, "text/plain", "Connected");
}

void handle_laserOn() {
  server.send(200, "text/plain", "Laser On");
}

void handle_laserOff() {
  server.send(200, "text/plain", "Laser off");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Error 404 not found");
}
