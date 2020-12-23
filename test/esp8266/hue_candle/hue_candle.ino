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
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(wifiName);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  // we could output a serial number here
 
  if (MDNS.begin("ESP_LEDCandle")) {
    Serial.println("MDNS responder started");
  }
 
  //handle root page
  webServer.on("/", handleRoot);
 
  //To change the colors the esp will listen to data at http://host/rgb
  webServer.on("/on", handleTurnOn);
  webServer.on("/off", handleTurnOff);
 
  //Handle path not found
  webServer.onNotFound(handleNotFound);
 
  webServer.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  webServer.handleClient();
}
