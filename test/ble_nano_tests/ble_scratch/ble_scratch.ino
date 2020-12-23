#include <BLE_API.h>

#ifndef ARRAY_LENGTH
#define ARRAY_LENGTH
#define array_length(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#endif

/**
 * Bluetooth data exchange stuff
 */
static const uint8_t TXRX_BUF_LEN = 20;

static uint8_t rx_buf[TXRX_BUF_LEN];
static uint8_t rx_buf_num;
static uint8_t rx_state=0;

// here's where we store incoming and outgoing data
uint8_t tx_value[TXRX_BUF_LEN] = {0,};
uint8_t rx_value[TXRX_BUF_LEN] = {0,};


/**
 *  Device Stuff
 */
const char* deviceName = "Zackees";
 
// The Nordic UART Service
static const uint8_t service1_uuid[]                = {0x71, 0x3D, 0, 0, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t service1_tx_uuid[]             = {0x71, 0x3D, 0, 3, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t service1_rx_uuid[]             = {0x71, 0x3D, 0, 2, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
// I think this is version information
static const uint8_t uart_base_uuid_rev[]           = {0x1E, 0x94, 0x8D, 0xF1, 0x48, 0x31, 0x94, 0xBA, 0x75, 0x4C, 0x3E, 0x50, 0, 0, 0x3D, 0x71};

// create two Serial characteristics
GattCharacteristic  characteristic1(service1_tx_uuid, tx_value, 1, TXRX_BUF_LEN, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ );
GattCharacteristic  characteristic2(service1_rx_uuid, rx_value, 1, TXRX_BUF_LEN, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
// join them into an array
GattCharacteristic *uartChars[] = {&characteristic1, &characteristic2};
// bind them under a service
GattService         uartService(service1_uuid, uartChars, sizeof(uartChars) / sizeof(GattCharacteristic *));


/**
 * nrf51:822 I/O
 */
BLE                                       ble;          // bluetooth device


/**
 * Other Stuff
 */


/**
 * Internal State
 */
Timeout                                   timeout;
boolean notify = false;


/**
 * Callbacks
 */


void writtenHandle(const GattWriteCallbackParams *Handler)
{
    uint8_t buf[TXRX_BUF_LEN];
    uint16_t bytesRead, index;

    Serial.println("onDataWritten : ");
    if (Handler->handle == characteristic1.getValueAttribute().getHandle()) {
        ble.readCharacteristicValue(characteristic1.getValueAttribute().getHandle(), buf, &bytesRead);
        Serial.print("bytesRead: ");
        Serial.println(bytesRead, HEX);
        for(byte index=0; index<bytesRead; index++) {
            Serial.write(buf[index]);
        }
        Serial.println("");
    }
}

void m_uart_rx_handle() {   //update characteristic data
  for (int x=0; x<TXRX_BUF_LEN; x++) {
    rx_buf[x] = random(65, 90);
  }
  Serial.print("Read request, sending: ");
  Serial.println((char*)rx_buf);
  
  ble.updateCharacteristicValue(characteristic1.getValueAttribute().getHandle(), rx_buf, rx_buf_num);
  ble.updateCharacteristicValue(characteristic2.getValueAttribute().getHandle(), rx_buf, rx_buf_num);
 
  memset(rx_buf, 0x00,20);
  rx_state = 0;
}
void uart_handle(uint32_t id, SerialIrq event) {   /* Serial rx IRQ */
    if(event == RxIrq) {
        if (rx_state == 0) {
            rx_state = 1;
            timeout.attach_us(m_uart_rx_handle, 100000);
            rx_buf_num=0;
        }
        while(Serial.available()) {
            if(rx_buf_num < 20) {
                rx_buf[rx_buf_num] = Serial.read();
                rx_buf_num++;
                Serial.println((char*)rx_buf);
            } else {
                Serial.read();
            }
        }
    }
}


void connectionCallback(const Gap::ConnectionCallbackParams_t *params) {
  Serial.println("Connected!");
}
void disconnectionCallBack(Gap::Handle_t handle, Gap::DisconnectionReason_t reason) {
    Serial.println("Disconnected!");
    Serial.println("Restarting the advertising process");
    ble.startAdvertising();
}


void updatesEnabledCallback(uint16_t charHandle) {
    if (charHandle == characteristic2.getValueAttribute().getHandle()) {
      notify = true;
      Serial.write("Notifications enabled\n\r");
    }
}
 
void updatesDisabledCallback(uint16_t charHandle) {
    if (charHandle == characteristic2.getValueAttribute().getHandle()) {
       notify = false;
       Serial.write("Notifications disabled\n\r"); 
    }
}


void setup() {
    // put your setup code here, to run once
    Serial.begin(9600);
    Serial.attach(uart_handle);

    // attach callbacks
    ble.onConnection(connectionCallback);
    ble.onDisconnection(disconnectionCallBack);
    ble.onUpdatesEnabled(updatesEnabledCallback);
    ble.onUpdatesDisabled(updatesDisabledCallback);
    ble.onDataWritten(writtenHandle);

    // initialize bluetooth device
    Serial.write("Initialising the nRF51822\n\r");
    ble.init();


    // advertise BLE mode only.
    // https://developer.mbed.org/teams/Bluetooth-Low-Energy/code/BLE_API/docs/bfc5b9b6ecf5/classGapAdvertisingData.html
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);
    // set the broadcast name                                        
    ble.accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME, (const uint8_t*)deviceName, array_length(deviceName) - 1);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                     (const uint8_t *)uart_base_uuid_rev, sizeof(uart_base_uuid_rev));


    // set advertizing type
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED); 
    // add service
    ble.addService(uartService);
    // set device name
    ble.setDeviceName((const uint8_t *)deviceName);
    // set tx power,valid values are -40, -20, -16, -12, -8, -4, 0, 4
    ble.setTxPower(4);    
    // set adv_interval, 100ms in multiples of 0.625ms.
    ble.setAdvertisingInterval(160);
    // set adv_timeout, in seconds
    ble.setAdvertisingTimeout(0);
    // start advertising
    ble.startAdvertising();

    /* */
    Serial.println("Advertising Start!");
    
    
    
    
}

void loop() {
  
    ble.waitForEvent();

}
