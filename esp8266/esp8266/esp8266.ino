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
  server.send(200, "text/html", HTML_STATUS(0));
}

void handle_laserOn() {
  server.send(200, "text/html", "Laser On");
}

void handle_laserOff() {
  server.send(200, "text/plain", "Laser off");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Error 404 not found");
}

String HTML_STATUS(unsigned int led_status) {
    
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html lang=\"en\">\n";
  ptr += "<head>\n";
  ptr += "<style>\n";
  ptr += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "body{margin-top: 10px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "ul {\n";
  ptr += "list-style-type: none;\n";
  ptr += "margin: 0;\n";
  ptr += "padding: 0;\n";
  ptr += "overflow: hidden;\n";
  ptr += "background-color: #333;}\n";
  
  ptr += "li {\n";
  ptr += "float: left;}\n";
  
  ptr += "li a {\n";
  ptr += "display: block;\n";
  ptr += "color: white;\n";
  ptr += "text-align: center;\n";
  ptr += "padding: 14px 16px;\n";
  ptr += "text-decoration: none;}\n";
  
  /* Change the link color to #111 (black) on hover */
  ptr += "li a:hover {\n";
  ptr += "background-color: #111;}\n";
  
  ptr += "</style>\n";
  ptr += "<meta charset=\"UTF-8\">\n";
  ptr += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  ptr += "<title>Document</title>\n";
  ptr += "<ul>\n";
  ptr += "<li><a href=\"\">S.P.L.A.T: A Simple Projectile Locating and Aiming Turret</a></li>\n";
  ptr += "<li><a href=\"#home\">Status Panel</a></li>\n";
  ptr += "<li><a href=\"#about\">About</a></li>\n";
          
  ptr += "</ul>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Status Panel</h1>\n";
  ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";

  return ptr;
}
