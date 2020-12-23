#include <Arduino.h>
#include <CurieBle.h>



/**
 * Alert Parameters
 */


static const char* bluetoothDeviceName = "Zackees";
const char* alertMessage = "make_call";
const char* changeLightModeCommand = "change_light_mode";
const char* heartbeatCommand = "hb";
int rssi;
int visualizationDelay_ms = 0;
const char* heartbeatReceivedMessage = "hb_received";
const char* heartbeatOnMessage = "hb_on";
const char* heartbeatOffMessage = "hb_off";
static const unsigned int alertDelay_ms = 5000;
unsigned long lastUpdateTime_ms = 0;

const char* flowControlMessage = "ready";

bool bleCharacteristicNeedsToBeFlushed = false;

static const int NOTHING_QUEUED = 0;
static const int HEARTBEAT_QUEUED = 1000;
int commandQueue = NOTHING_QUEUED;



/**
 * Bluetooth attributes
 */
static const char* serviceUUID = "180D"; 
static const char* writeCharacteristicUUID = "2A37";
static const char* readCharacteristicUUID = "2A38";

BLEPeripheral blePeripheral;

BLEService service(serviceUUID);
BLECharacteristic writeCharacteristic(
  writeCharacteristicUUID,
  BLERead | BLENotify,
  20
);

BLECharacteristic readCharacteristic(
  readCharacteristicUUID,
  BLEWrite | BLERead | BLENotify,
  20
);



void bleDeviceConnected(BLECentral& central) {
  Serial.print("Device connected: ");
    Serial.println(central.address());
}

void bleDeviceDisconnected(BLECentral& central) {
  Serial.print("Device disconnected: ");
    Serial.println(central.address());  
}


void bleCharacteriticWritten(BLECentral& central, BLECharacteristic &characteristic) {

  const char* uuid = characteristic.uuid();
  const unsigned short valueLength = characteristic.valueLength();
  //const unsigned char value[valueLength];
  //value =& characteristic.value();
  const unsigned char* value = characteristic.value();

  Serial.print("Characteristic ");
  Serial.print(uuid);
  Serial.print(" written to: ");

  char realValue[valueLength+1];
  // move values from value to realValue - the original has extra stuff in it
  memcpy(realValue, value, (valueLength)*sizeof(char));
  // since we are printing this out, let's make sure the string terminates
  realValue[valueLength] = '\0';

  for (int i=0; i<valueLength; i++) {
    Serial.print(realValue[i], HEX);
  }
//  Serial.print(realValue);
  Serial.print(", value length: ");
  Serial.println(valueLength);
  /*
  for (int i=0; i<valueLength; i++) {
    Serial.println(value[i], HEX);
  }
  */


  // look for commands
  char command[10];
  strncpy ( command, (const char*) value, sizeof(command) );
  Serial.print("command:   ");
  Serial.println(command);
  Serial.print("heartbeat: ");
  Serial.println(heartbeatCommand);
  Serial.println(strcmp(command, heartbeatCommand));
  if (strcmp(command, heartbeatCommand) == 0) {
    // grab the rssi
    rssi = value[15];
    visualizationDelay_ms = value[14];
    commandQueue = HEARTBEAT_QUEUED;
  }

  bleCharacteristicNeedsToBeFlushed = true;
}

void setHeartbeatIntensity(int rssi, int visualizationTimeout_ms) {
  Serial.print("Setting heartbeat intensity to: ");
  Serial.println(rssi);
  Serial.print("Setting visualization timeout to: ");
  Serial.println(visualizationTimeout_ms);
  commandQueue = NOTHING_QUEUED;
  writeCharacteristic.setValue((const unsigned char*) heartbeatReceivedMessage, 12); // size of heartbeatReceivedMessage
}

void setHeartbeatEnabled() {
  Serial.println("Enabling heartbeat");
  heartbeat_active = true;
  writeCharacteristic.setValue((const unsigned char*) heartbeatOnMessage, 5); // size of heartbeatReceivedMessage
}
void setHeartbeatDisabled() {
  Serial.println("Disabling heartbeat");
  heartbeat_active = false;
  writeCharacteristic.setValue((const unsigned char*) heartbeatOffMessage, 6); // size of heartbeatReceivedMessage
}

void bleCharacteristicSubscribed(BLECentral& central, BLECharacteristic &characteristic) {
  const char* uuid = characteristic.uuid();
  Serial.print("Characteristic ");
  Serial.print(uuid);

  if (characteristic.subscribed()) {
    Serial.println(" subscribed to");
  } else {
    Serial.println(" unsubscribed from");
  }
}

void sendChangeLightModeCommand(const unsigned char* changeLightModeCommand, int commandLength) {
  Serial.println("Sending change light mode command over ble");
  writeCharacteristic.setValue(changeLightModeCommand, commandLength);
}


void setup_bluetooth() {
  
  Serial.print("Setting ble device name to: ");
  Serial.println(bluetoothDeviceName);
  blePeripheral.setLocalName(bluetoothDeviceName);
  Serial.print("Adding primary service with UUID: ");
  Serial.println(serviceUUID);
  blePeripheral.setAdvertisedServiceUuid(service.uuid());
  blePeripheral.addAttribute(service);
  Serial.println("adding writable characteristic");
  blePeripheral.addAttribute(writeCharacteristic);
  blePeripheral.addAttribute(readCharacteristic);

  Serial.println("adding event handlers");

  blePeripheral.setEventHandler(
    BLEConnected,
    bleDeviceConnected
  );
  blePeripheral.setEventHandler(
    BLEDisconnected,
    bleDeviceDisconnected
  );

  // these apparently do nothing
  writeCharacteristic.setEventHandler(
    BLESubscribed,
    bleCharacteristicSubscribed
  );
  writeCharacteristic.setEventHandler(
    BLEUnsubscribed,
    bleCharacteristicSubscribed
  );


  readCharacteristic.setEventHandler(
    BLEWritten,
    bleCharacteriticWritten
  );

  Serial.println("starting bluetooth broadcasting");
  blePeripheral.begin();
}

void loop_bluetooth() {
  
 
  if (bleCharacteristicNeedsToBeFlushed) {
    bleCharacteristicNeedsToBeFlushed = false;
    writeCharacteristic.setValue((const unsigned char*) flowControlMessage, 5);
  }

  if (commandQueue == HEARTBEAT_QUEUED) {
    setHeartbeatIntensity(rssi, visualizationDelay_ms);
  }
}

