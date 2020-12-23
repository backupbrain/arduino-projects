#include <BleCharacteristic.h>
#include <BleDevice.h>
#include <BleService.h>
#include <BleCommon.h>
#include <BleDescriptor.h>



/* Device name: Appears in advertising packets. Must not exceed 20 characters in length */
/* UUID for Automation I/O service */
#define SERVICE_UUID_AUTOMATIONIO (0x1815)

const char* btDeviceName = "Zackees Intel";

/* BLE Peripheral Device (this Intel Curie device) */
BlePeripheral bleDevice;

/* BLE Automation I/O Service */
BleService ioService(SERVICE_UUID_AUTOMATIONIO);


void checkStatus(BleStatus status) {
  if (status != BLE_STATUS_SUCCESS) {
    Serial.print("Error: Could not set BLE Setting: ");
    Serial.println(status, HEX);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial.available());
  
  Serial.println("Hello World");

  checkStatus(bleDevice.setName(btDeviceName));
  checkStatus(bleDevice.init());
  checkStatus(bleDevice.addPrimaryService(ioService, true));

 

  // Now activate the BLE device.  It will start continuously transmitting BLE
  // advertising packets and thus become visible to remote BLE central devices
  // (e.g smartphones) until it receives a new connection 
  Serial.println("Starting bluetooth service.");
  checkStatus(bleDevice.start());

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
}
