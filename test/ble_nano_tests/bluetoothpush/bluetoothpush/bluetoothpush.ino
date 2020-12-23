#include <BLE_API.h>

#define TXRX_BUF_LEN                      20

BLE                                 	    ble;
Timeout                                   timeout;
Ticker                    ticker_task1;

static uint8_t rx_buf[TXRX_BUF_LEN];
static uint8_t rx_buf_num;
static uint8_t rx_state=0;

// The Nordic UART Service
static const uint8_t service1_uuid[]                = {0x71, 0x3D, 0, 0, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t service1_tx_uuid[]             = {0x71, 0x3D, 0, 3, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t service1_rx_uuid[]             = {0x71, 0x3D, 0, 2, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t uart_base_uuid_rev[]           = {0x1E, 0x94, 0x8D, 0xF1, 0x48, 0x31, 0x94, 0xBA, 0x75, 0x4C, 0x3E, 0x50, 0, 0, 0x3D, 0x71};

uint8_t tx_value[TXRX_BUF_LEN] = {0,};
uint8_t rx_value[TXRX_BUF_LEN] = {0,};


static uint8_t hrmCounter = 100;
static uint8_t bpm[2] = {0x00, hrmCounter};


GattCharacteristic  characteristic1(service1_tx_uuid, tx_value, 1, TXRX_BUF_LEN, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ );

GattCharacteristic  characteristic2(service1_rx_uuid, rx_value, 1, TXRX_BUF_LEN, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

GattCharacteristic *uartChars[] = {&characteristic1, &characteristic2};

GattService         uartService(service1_uuid, uartChars, sizeof(uartChars) / sizeof(GattCharacteristic *));





unsigned long lastTime_ms = 0; 
const unsigned int heartbeatTimeout_ms = 500;
boolean notify = false;
boolean sendData = false;


void connectionCallback(const Gap::ConnectionCallbackParams_t* params) {
    Serial.println("Cannected to a device!");
}
 
void disconnectionCallBack(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    Serial.println("Disconnected!");
    Serial.println("Restarting the advertising process");
    ble.startAdvertising();
}

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

void m_uart_rx_handle()
{   //update characteristic data
    ble.updateCharacteristicValue(characteristic1.getValueAttribute().getHandle(), rx_buf, rx_buf_num);
    ble.updateCharacteristicValue(characteristic2.getValueAttribute().getHandle(), rx_buf, rx_buf_num);
    memset(rx_buf, 0x00,20);
    rx_state = 0;
}


// read from Serial if available, and transmit via BLEs
void uart_handle(uint32_t id, SerialIrq event)
{   /* Serial rx IRQ */
    if(event == RxIrq) {
        if (rx_state == 0) {
            rx_state = 1;
            timeout.attach_us(m_uart_rx_handle, 100000);
            rx_buf_num=0;
        }
        
        for (int x=0; x<TXRX_BUF_LEN; x++) {
          rx_buf[x] = random(65, 90);
        }
        
    }
}


void updatesEnabledCallback(uint16_t charHandle) {
    if (charHandle == characteristic2.getValueAttribute().getHandle()) {
      notify = true;
      Serial.write("Notifications enabled\n\r");
      
      ticker_task1.attach(periodicCallback, 1);
    }
}
 
void updatesDisabledCallback(uint16_t charHandle) {
    if (charHandle == characteristic2.getValueAttribute().getHandle()) {
       notify = false;
       Serial.write("Notifications disabled\n\r"); 
       ticker_task1.detach();
    }
}


void periodicCallback() {
    if (ble.getGapState().connected)  {
      sendData = true; // because this operates in an interrupt, it's better to handle heavy lifting in main thread
    }
}

void setup() {

    // put your setup code here, to run once
    Serial.begin(9600);
    

    /* Setup the local GAP/GATT event handlers */
    ble.onConnection(connectionCallback);
    //ble.onTimeout(disconnectionCallBack);
    ble.onDisconnection(disconnectionCallBack);
    ble.onUpdatesEnabled(updatesEnabledCallback);
    ble.onUpdatesDisabled(updatesDisabledCallback);
    ble.onDataWritten(writtenHandle);

    
    /* Initialise the nRF51822 */
    Serial.write("Initialising the nRF51822\n\r");
    ble.init();

    // setup adv_data and srp_data
    /* Add BLE-Only flag and complete service list to the advertising data */
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
                                     (const uint8_t *)"Zackees", sizeof("Zackees") - 1);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                     (const uint8_t *)uart_base_uuid_rev, sizeof(uart_base_uuid_rev));
    /* */
 

    // set adv_type
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    // add service
    ble.addService(uartService);
    // set device name
    ble.setDeviceName((const uint8_t *)"Simple Chat");
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
    
    unsigned long currentTime_ms = millis();

    /*
    if (currentTime_ms - lastTime_ms >= heartbeatTimeout_ms) {
      lastTime_ms = currentTime_ms;   
      if (notify) {
        uint8_t value = (uint8_t) random(0, 254);
        
        for (int x=0; x<TXRX_BUF_LEN; x++) {
          rx_buf[x] = random(65, 90);
        }
        rx_buf[TXRX_BUF_LEN-1] = '\0';
        ble.updateCharacteristicValue(characteristic2.getValueAttribute().getHandle(), rx_buf, rx_buf_num);
        ble.updateCharacteristicValue(characteristic1.getValueAttribute().getHandle(), rx_buf, rx_buf_num);
        
        Serial.write(rx_buf, HEX);
        Serial.write("\n");
      } else {
        // this enters low power mode until something interesting happens, effectively stalling the chip
        ble.waitForEvent();
      }
    }
    /* */

    if (sendData) {
      
        /* Update the HRM measurement */
        /* First byte = 8-bit values, no extra info, Second byte = uint8_t HRM value */
        /* See --> https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.heart_rate_measurement.xml */

        if (ble.getGapState().connected) {
          for (int x=0; x<TXRX_BUF_LEN-1; x++) {
            rx_value[x] = random(65, 90);
          }
          rx_value[TXRX_BUF_LEN-1] = '\0';
          Serial.print("printing output: ");
          Serial.println((char*) rx_value);
          
//          ble.updateCharacteristicValue(characteristic1.getValueAttribute().getHandle(), rx_value, rx_buf_num);
//          ble.updateCharacteristicValue(characteristic2.getValueAttribute().getHandle(), rx_value, rx_buf_num);
          ble.updateCharacteristicValue(characteristic2.getValueAttribute().getHandle(), rx_value, TXRX_BUF_LEN);
          sendData = false;
        }

    } else {
      ble.waitForEvent();
    }
    
}


