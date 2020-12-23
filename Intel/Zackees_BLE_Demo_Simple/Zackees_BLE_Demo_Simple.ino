#include <BleCharacteristic.h>
#include <BleDevice.h>
#include <BleService.h>
#include <BleCommon.h>
#include <BleDescriptor.h>



/*
 * This sketch partially implements the standard Bluetooth Low-Energy "Automation IO" service.
 * It is a relatively complex example which demonstrates use of BLE descriptors and
 * multi-instance characteristics for data input and data output.
 *
 * For more information: https://developer.bluetooth.org/gatt/services/Pages/ServicesHome.aspx
 */

/* Device name: Appears in advertising packets. Must not exceed 20 characters in length */
/* UUID for Automation I/O service */
#define SERVICE_UUID_AUTOMATIONIO (0x1815)
/* UUID for digital characteristic */
#define CHAR_UUID_DIGITAL         (0x2A56)
/* UUID for analog characteristic */
#define CHAR_UUID_ANALOG          (0x2A58)

const char* btDeviceName = "Zackees Intel";
const uint16_t serviceUUID = SERVICE_UUID_AUTOMATIONIO;
const uint16_t communicationCharacteristic = CHAR_UUID_DIGITAL;



/* BLE Peripheral Device (this Intel Curie device) */
BlePeripheral bleDevice;

/* BLE Automation I/O Service */
BleService ioService(SERVICE_UUID_AUTOMATIONIO);


BleCharacteristic transmitReceiveCharactaristic(
  communicationCharacteristic,
  BLE_MAX_ATTR_DATA_LEN,
  BLE_CLIENT_ACCESS_READ_WRITE,
  BLE_CLIENT_NOTIFY_ENABLED
);

BleCharacteristic receiveCharactaristic(
  communicationCharacteristic,
  BLE_MAX_ATTR_DATA_LEN,
  BLE_CLIENT_ACCESS_READ_ONLY,
  BLE_CLIENT_NOTIFY_ENABLED
);



void checkStatus(BleStatus status) {
  if (status != BLE_STATUS_SUCCESS) {
    Serial.print("Error: Could not set BLE Setting: ");
    Serial.println(status, HEX);
  }
}


/* This function will be called when a BLE GAP event is detected by the
 * Intel Curie BLE device 
 */
void blePeripheralConnected_callback(BlePeripheral &peripheral, BlePeripheralEvent event, void *arg) {
  if (event == BLE_PERIPH_EVENT_CONNECTED) {
    Serial.println("Bluetooth client connected");    
  } else if (event == BLE_PERIPH_EVENT_DISCONNECTED) {
    Serial.println("Bluetooth client disconnected");
  } else {
    Serial.println("Got UNKNOWN peripheral event");
  }
}


/* This function will be called when a connected remote peer sets a new value for a digital output characteristic */
void bleDataSent_callback(BleCharacteristic &characteristic, BleCharacteristicEvent event, void *arg) {
  uint8_t val;

   Serial.println("Bluetooth event!");
    characteristic.getValue(val);
    
    Serial.print("Data Sent:");
    Serial.println(val);

  if (event == BLE_CHAR_EVENT_WRITE) {
    /* The remote client has updated the value for this pin, get the current value */
    characteristic.getValue(val);
    
    Serial.print("Data Sent:");
    Serial.print(val);
  } else
    Serial.println("Got UNKNOWN characteristic event");
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial.available());
  
  Serial.println("Hello World");

  checkStatus(bleDevice.setName(btDeviceName));
  checkStatus(bleDevice.init());
  bleDevice.setEventCallback(blePeripheralConnected_callback);
  checkStatus(bleDevice.addPrimaryService(ioService, true));

 

  Serial.println("adding user description");
  checkStatus(transmitReceiveCharactaristic.addUserDescription("input"));

  
  /* Add the Presentation-Format for this pin
   *   format:      0x2    (unsigned 2-bit integer)
   *   exponent:    0      (Not Applicable)
   *   unit:        0x2700 (BLE GATT Unit UUID: unitless)
   *   nameSpace:   0x1    (Bluetooth standard namespace)
   *   description: pin+1  (Instance number of this characteristic)
   */
  //Serial.println("adding presentation to characteristic");
  //checkStatus(transmitReceiveCharactaristic.addPresentationFormat(0x02, 0, 0x2700, 0x1, 1));


  //Add a callback to be triggered if the remote device updates the value for this pin 
  Serial.println("adding event callback");
  transmitReceiveCharactaristic.setEventCallback(bleDataSent_callback);
  
  Serial.println("adding charactaristic");
  // Add the characteristic for this pin 
  checkStatus(ioService.addCharacteristic(transmitReceiveCharactaristic));

  
  checkStatus(ioService.addCharacteristic(receiveCharactaristic));
  checkStatus(receiveCharactaristic.setValue(1));
  
  
  //checkStatus(transmitReceiveCharactaristic.setValue((uint8_t) 1));
  /* */

  // Now activate the BLE device.  It will start continuously transmitting BLE
  // advertising packets and thus become visible to remote BLE central devices
  // (e.g smartphones) until it receives a new connection 
  Serial.println("Starting bluetooth service.");
  checkStatus(bleDevice.start());

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  uint8_t randval = (uint8_t) random(1,100);
    Serial.print("Changing the value of our data point to ");
    Serial.println(randval);
    
    checkStatus(receiveCharactaristic.setValue(randval));
    delay(500);
}
