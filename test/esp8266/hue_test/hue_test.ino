#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
const char *ssid = "20mission";
const char *password = "windypage23";
 
ESP8266WebServer server(80);
 
//Assign color to gpio
#define rgbRED 12
#define rgbGREEN 13
#define rgbBLUE 2
#define rgbWHITE 14
 
//RGB Struct
struct RGB {
  byte r;
  byte g;
  byte b;
  byte w;
};
 
RGB miColor;
 
//Update PWM according to the new values
void updateLights()
{
  analogWrite(rgbRED, miColor.r);
  analogWrite(rgbGREEN, miColor.g);
  analogWrite(rgbBLUE, miColor.b);
  analogWrite(rgbWHITE, miColor.w);
}
 
//Handle main page
void handleRoot() {
  server.send(200, "text/plain", "Hello from Mi Light!");
}
 
//Handle color change
void handleColorChange(){ 
  for (uint8_t i=0; i<server.args(); i++){
    switch (server.argName(i)[0])
    {
      case 'r':
        miColor.r = server.arg(i).toInt();
      break;
      case 'g':
        miColor.g = server.arg(i).toInt();
      break;
      case 'b':
        miColor.b = server.arg(i).toInt();
      break;
      case 'w':
        miColor.w = server.arg(i).toInt();
      break;
    }
  }
  //Update the light with to the received data
  updateLights();
  
  String message="Colors: \r\n";
  message += "Red: " + (String)miColor.r + "\r\n";
  message += "Green: " + (String)miColor.g + "\r\n";
  message += "Blue: " + (String)miColor.b + "\r\n";
  message += "White: " + (String)miColor.w + "\r\n";
  server.send(200, "text/plain", message);
  
}
 
void handleNotFound(){
  String message = "File Not Found\n\n"; 
  server.send(404, "text/plain", message);
}
 
void setup(void){
//Initialize starting color (only white on full power)  
  miColor = (RGB){0,0,0,255};
 
//Initialize GPIO's as OUTPUT
  pinMode(rgbRED, OUTPUT);
  pinMode(rgbGREEN, OUTPUT);
  pinMode(rgbBLUE, OUTPUT);
  pinMode(rgbWHITE, OUTPUT);
 
//I use 8bit RGB color space, so we will limit the PWM to 255 (2^8)
  analogWriteRange(255);
 
//Set the output to their initialized state (only white is on)
  updateLights();
 
 //Start the serial and connect to network
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  if (MDNS.begin("ESP_MiLight")) {
    Serial.println("MDNS responder started");
  }
 
//handle root page
  server.on("/", handleRoot);
 
//To change the colors the esp will listen to data at http://host/rgb
  server.on("/rgb", handleColorChange);
 
//Handle path not found
 server.onNotFound(handleNotFound);
 
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
}
