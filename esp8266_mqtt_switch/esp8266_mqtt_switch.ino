// code from: http://www.arduinesp.com/wifiwebserver
// more examples from: https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide
// cleaner code: https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-posting-to-phant
// mqtt example: http://www.hivemq.com/blog/mqtt-client-library-encyclopedia-arduino-pubsubclient/
// esp8266 mqtt example: https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/**
 * WiFi Settings
 */
const char* wifiAccessPoint = "6";
const char* wifiPassword = "whatpassword";

/**
 * MQTT Server settings
 */
const char* mqttServer = "app.tonygaitatzis.com";
const int mqttPort = 8883;
const char* mqttClientName = "smartSwitch";
const char* mqttControlTopic = "smartswitch/control";
const char* mqttStatusTopic = "smartswitch/status";

const char MQTT_MESSAGE_LIGHT_OFF = '0';
const char MQTT_MESSAGE_LIGHT_ON = '1';
const char MQTT_MESSAGE_LIGHT_STATUS = '2';

/**
 * Program
 */
const int STATE_ON  = 0x100;
const int STATE_OFF = 0x101;
int state = STATE_OFF;

int serialBaudRate = 115200;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;



void setupWifi(void);
void onMqttMessageReceived(char* topic, byte* payload, unsigned int length);
void connectMQTT(void);
void reportStatus(void);
void turnOn(void);
void turnOff(void);


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(serialBaudRate);
  setupWifi();
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(onMqttMessageReceived);
  turnOn();
}

void loop() {

  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  /*
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqttTopic, msg);
  }
  */
}




void setupWifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiAccessPoint);

  WiFi.begin(wifiAccessPoint, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void onMqttMessageReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //if (strcmp(mqttStatusTopic, (const char*) topic) == 0) {
  //  reportStatus();
  //} else 
  if (strcmp(mqttControlTopic, (char*) topic) == 0) {
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == MQTT_MESSAGE_LIGHT_ON) {
      Serial.println("turning lights on");
      turnOn();
    } else if ((char)payload[0] == MQTT_MESSAGE_LIGHT_OFF) {
      Serial.println("turning lights off");
      turnOff();
    } else if ((char)payload[0] == MQTT_MESSAGE_LIGHT_STATUS) {
      reportStatus();
    }
    
  } else {
    Serial.println("unknown topic");
  }



}

void reportStatus() {
  char msg[5];
  switch (state) {
    case STATE_ON:
      strcpy(msg, "on");
    break;
    case STATE_OFF:
      strcpy(msg, "off");
    break;
  }
  
  mqttClient.publish(mqttControlTopic, msg);
}
void turnOn() {
  state = STATE_ON;
  digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is acive low on the ESP-01)
}
void turnOff() {
  state = STATE_OFF;
  digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
}

void connectMQTT() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(mqttClientName)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //mqttClient.publish(mqttStatusTopic, "hello world");
      // ... and resubscribe
      //mqttClient.subscribe(mqttStatusTopic);
      mqttClient.subscribe(mqttControlTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

