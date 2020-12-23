
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "LEDCandle.h"

/**
 * Configuration
 */
const char *wifiName = "20mission";
const char *wifiPassword = "windypage23";

const boolean serialEnabled = true;
const unsigned int serialSpeed = 9600;
const unsigned int httpPort = 80;

const byte candleLEDPin = 2;


const char* host = "192.168.0.1";
const char* hostIP = "192.168.0.1";
String url = "/getIP.php?psswd=";
WiFiServer server(80);


/**
 * Variables
 */
 
LEDCandle ledCandle = LEDCandle(candleLEDPin);

ESP8266WebServer webServer(httpPort);

/**
 * Serve root page
 */
void handleRoot() {
  String greetingString = "LED Candle Serial #: " + ledCandle.getSerialNumber();
  char greeting[80];
  greetingString.toCharArray(greeting, 80); 
  webServer.send(200, "text/plain", greeting);
}

/**
 * Handle state changes
 */
void handleTurnOn() { 
  ledCandle.turnOn();
  
  String message="Candle is on\r\n";
  webServer.send(200, "text/plain", message);
}

void handleTurnOff() { 
  ledCandle.turnOff();
  
  String message="Candle is off\r\n";
  webServer.send(200, "text/plain", message);
}
 
 
void handleNotFound(){
  String message = "File Not Found\n\n"; 
  webServer.send(404, "text/plain", message);
}
 
void setup(void){
  String serialNumber = WiFi.macAddress();
  ledCandle.setSerialNumber(serialNumber);
  // by default turn candle on
  ledCandle.turnOn();
  
  //Start the serial and connect to network
  Serial.begin(serialSpeed);
  WiFi.begin(wifiName, wifiPassword);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  server.begin();  // Start Server
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(wifiName);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  // we could output a serial number here



  // Use WiFiClient class to create TCP connections
  Serial.println("connecting to ");
  Serial.println(host);
  delay(10);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(hostIP, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  
  WiFiClient clientS = server.available();

  int state = 0;
  unsigned long whileTimeout = 0;
  
  Serial.println("http client connected");
  whileTimeout = millis();
  while (!clientS.available() ) {
    if (  (millis() - whileTimeout) > 5000 ) {
      clientS.stop();
      return;
    }
    delay(1);
  }
  String req = clientS.readStringUntil('\r');
  Serial.print("Client got: ");
  Serial.println(req);
  
  clientS.flush();
  int val = 1;
  if (req.indexOf("/gpio/0") != -1) {// LOW
    state = 1;
  } else if (req.indexOf("/gpio/1") != -1) { //HIGH
    state = 2;
  }  else if  (req.indexOf("/gpio/s") != -1) { // GET STATUS
  } else if  (req.indexOf("/gpio/u") != -1) {
    val = 0;  // UPDATE
  } else {
    clientS.stop();
    return;
  }
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n Last GPIO state was:  ";
  s += (state) ? "high" : "low";
  s += "</html>\n";
  clientS.print(s);
  delay(10);
  clientS.stop();
  if (val) return;

 
}
 
void loop(void){
}
