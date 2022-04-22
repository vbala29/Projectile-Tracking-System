/**
 * @brief Web Server for ESP8266 Microcontroller
 * @author Developed by Vikram Bala, C'24 University of Pennsylvania
 * @date April 12th, 2022
 * @cite https://lastminuteengineers.com/creating-esp8266-web-server-arduino-ide/
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <stdlib.h>
#include <stdio.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

const char* ssid = "AirPennNet-Device"; 
const char* password = "penn1740wifi";  

//Class C subnet, where the last octet is the host address
//and the first 3 octets are the network address
IPAddress subnet(255,255,255,0); 

//Start the server on port 80
ESP8266WebServer server(80);

unsigned int laser = 0; //Boolean indicating if laser is on
unsigned short int pitch_num;
unsigned short int yaw_num;

SoftwareSerial s(3, 1); //Starts s using Rx/Tx labeled pins on the esp8266.

void setup() {
  s.begin(9600);
  delay(100);

  pinMode(D2, OUTPUT);

  WiFi.begin(ssid, password);

   //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  s.print(".");
  }
  s.println("");
  s.println("WiFi connected..!");
  s.print("Got IP: ");  s.println(WiFi.localIP());
  
  server.on("/", handle_OnConnect);
  server.on("/laserOn", handle_laserOn);
  server.on("/laserOff", handle_laserOff);
  server.on("/turret", handle_turret);
  server.onNotFound(handle_NotFound);
  server.enableCORS(true);
  server.begin();
  s.println("HTTP server started");
}

 void loop() {
  if (s.available()) {
    String loc = s.readStringUntil('.');
    unsigned int comma_index = loc.indexOf(',');
    s.println(loc);
    String pitch = loc.substring(0, comma_index);
    String yaw = loc.substring(comma_index + 1);
    s.read();
    unsigned short int pitch_num_temp = pitch.toInt();
    unsigned short int yaw_num_temp = yaw.toInt();

    pitch_num = pitch_num_temp;
    yaw_num = yaw_num_temp;

    char str[25];
    sprintf(str, "Received pitch: %u, yaw: %u", pitch_num, yaw_num);
    s.println(str);
  }

server.handleClient(); 
  if (laser) {
   digitalWrite(D2, HIGH);
  } else {
    digitalWrite(D2, LOW);
  }
}

void handle_OnConnect() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", HTML_STATUS(0));
}

void handle_laserOn() {
  laser = 1;
  //Redirect to "/" route -> (onConnect)
  server.sendHeader("Location", String("/"), true);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send ( 302, "text/plain", "");
}

void handle_laserOff() {
  laser = 0;
  //Redirect to "/" route -> (onConnect)
  server.sendHeader("Location", String("/"), true);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send( 302, "text/plain", "");
}

void handle_turret() {
  DynamicJsonDocument doc(512);
  doc["pitch"] = pitch_num;
  doc["yaw"] = yaw_num;

  String buf;
  serializeJson(doc, buf);
  s.print("Sending data: ");
  s.println(buf);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", buf);
}

void handle_NotFound() {
   if (server.method() == HTTP_OPTIONS)
    {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Max-Age", "10000");
        server.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "*");
        s.print("204 sent\n");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(204);
    }
    else
    {
        DynamicJsonDocument doc(512);
        doc["pitch"] = pitch_num;
        doc["yaw"] = yaw_num;
      
        String buf;
        serializeJson(doc, buf);
        s.print("Sending data: ");
        s.println(buf);
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "application/json", buf);
        s.print("200 response inside of 404\n");
    }
}

String HTML_STATUS(unsigned int led_status) {
    
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html lang=\"en\">\n";
  ptr += "<head>\n";
  ptr += "<script>\n";  
  ptr +="function loadDoc() {\n";
  ptr += "var xhttp = new XMLHttpRequest();\n";
  ptr += "xhttp.onreadystatechange = function() {\n";
  ptr += "console.log(this.responseText); let pitch = JSON.parse(this.responseText).pitch; let yaw = JSON.parse(this.responseText).yaw; \n";
  ptr += "console.log(\"pitch = \" + pitch + \" yaw = \" + yaw);\n";
  ptr +=  "document.getElementById(\"pitch\").innerHTML = \"Pitch Angle of Turret: \" + pitch + \"º above horizon\";\n";
  ptr +=  "document.getElementById(\"yaw\").innerHTML = \"Yaw Angle of Turret: \" + yaw + \"º above horizon\";\n";
  ptr += "}\n";
  ptr += "xhttp.open(\"GET\", \"10.103.219.210/turret\");\n";
  ptr += "xhttp.send();}\n";
  ptr += "var intervalId = setInterval(loadDoc, 1000);\n"; //repeat every one second
  ptr += "</script>\n";
  ptr += "<style>\n";
  ptr += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "body{margin-top: 10px;} h1 {color: #444444;margin: 20px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
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
  if (laser) {
      ptr += "<p>Laser Status: ON</p><a class=\"button button-off\" href=\"/laserOff\">OFF</a>\n";
  } else {
    ptr += "<p>Laser Status: OFF</p><a class=\"button button-on\" href=\"/laserOn\">ON</a>\n";
  }

  ptr += String("<h1 id=\"pitch\">Pitch Angle of Turret: " + String(pitch_num) + "º above horizon</h1>\n");
  ptr += String("<h1 id=\"yaw\">Yaw Angle of Turret: " + String(yaw_num) + "º</h1>\n");

  ptr += "</body>\n";
  ptr += "</html>\n";

  return ptr;
}
