
#ifndef _XBEE_BROADCAST_MESSAGE_H_
#define _XBEE_BROADCAST_MESSAGE_H_


#include "Arduino.h"

// This is a wrapper that makes broadcasting messages easy with the
// XBee radio devices. This software package has been tested with the
// Series 1 radio, the Pro (60mW) and the Regular (1mW).
// This class can both send and recieve messages.
//
// PREREQUISITE:
//  Each XBee must be set to API = 1, which is the standard requirement
//  since this obects uses the xbee-arduino library. All other settings
//  are default but it is recommended that the interface speed is increased
//  to 57600 in the xbee configuration software. Note that Serial.begin(57600)
//  will have to be invoked in your setup program.
//
// --------------------------------
// Example (Test_Tx.ino)
//
// #include "XbeeBroadcastMessage.h"
// XbeeBroadcastMessage xbee_broadcast(Serial);
//
//void setup() {
//  Serial.begin(9600);
//}
//
//void loop() {
//  const int err = xbee_broadcast.SendString("Hello World!");
//  if (0 == err) {
//    // Success.
//  } else {
//    // Failure.
//  }
//  delay(1000);
//}
//
// --------------------------------
// Example (Test_Rx.ino):
//
// #include "XbeeBroadcastMessage.h"
// XbeeBroadcastMessage xbee_broadcast(Serial);
// void setup() {
//   Serial.begin(9600);
// }
// void loop() {
//   uint8_t input_msg[32];
//   const int n_bytes =
//       xbee_broadcast.ReceiveData(input_msg, sizeof(input_msg));
//
//  if (n_bytes < 0) {
//    // Error condition.
//    delay(1000);
//    return;
//  }
//
//  if (n_bytes == 0) {
//    // No message recieved.
//    delay(100);
//    return;
//  }
//
//  if (0 == strcmp(reinterpret_cast<char*>(input_msg), "Hello World!")) {
//    // Success.
//  } else {
//    // Failure.
//  }
// }
class XbeeBroadcastMessage {
 public:
  XbeeBroadcastMessage(Stream& tx_rx_stream) : xbee_() {
    xbee_.setSerial(tx_rx_stream);
  }

  enum SendErr {
    kOk = 0,
    kNoRecipients = 1,
    kInvalidResponse = 2,
    kNoResponse = 3,
    kTimedOut = 4,
    kUnexpected = 5
  };

  SendErr SendString(const char* m) {
    return SendPacket(reinterpret_cast<const uint8_t*>(m), strlen(m) + 1);
  }

  SendErr SendData(const uint8_t* data, uint8_t length) {
    return SendPacket(data, length);
  }

  // Returns number of bytes recieved. 0 means that there was no
  // pending message. -1 means there was an error.
  int ReceiveData(uint8_t* data_dst, uint8_t data_length);

  // size is number of bytes in the string, including it's '\0' null character.
  bool ReceiveString(char* dest, int size) {
    if (!size)
      return false;
    int bytes_written = ReceiveData(reinterpret_cast<uint8_t*>(dest), size);
    if (!bytes_written) {
      return false;
    }
    dest[size - 1] = '\0';
    return true;
  }

  bool ReceiveStringCmp(const char* str) {
    char msg[32];
    if (!ReceiveString(msg, 32)) {
      // No message.
      return false;
    }
    return 0 == strcmp(str, msg);
  }

 private:
  SendErr SendPacket(const uint8_t* data, uint8_t length);
  XBee xbee_;
};


// Fake implementation of NullXbeeBroadcastMessage. This is useful
// for disabling XbeeBroadcastMessage in code so that Serial.print
// works as expected.
class NullXbeeBroadcastMessage {
 public:
  NullXbeeBroadcastMessage(Stream& tx_rx_stream) {}

  enum SendErr {
    kOk = 0,
    kNoRecipients = 1,
    kInvalidResponse = 2,
    kNoResponse = 3,
    kTimedOut = 4,
    kUnexpected = 5
  };

  SendErr SendString(const char* m) {
    return kOk;
  }

  SendErr SendData(const uint8_t* data, uint8_t length) {
    return kOk;
  }

  // Returns number of bytes recieved. 0 means that there was no
  // pending message. -1 means there was an error.
  int ReceiveData(uint8_t* data_dst, uint8_t data_length) {
    return 0;
  }

  // size is number of bytes in the string, including it's '\0' null character.
  bool ReceiveString(char* dest, int size) {
    if (size) {
      *dest = '\0';
    }
    return false;
  }

  bool ReceiveStringCmp(const char* str) { return false; }
};


//////////////////// implimentation details //////////////////////////
inline int XbeeBroadcastMessage::ReceiveData(uint8_t* data_dst, uint8_t data_length) {
  xbee_.readPacket();

  if (xbee_.getResponse().isError())
    return -1;

  if (!xbee_.getResponse().isAvailable())
    return 0;


  Rx16Response rx16;
  xbee_.getResponse().getRx16Response(rx16);

  uint8_t option = rx16.getOption();
  (void)option;

  uint8_t* data = rx16.getData();
  const uint8_t length = rx16.getDataLength();


  if (xbee_.getResponse().getApiId() == RX_16_RESPONSE) {
    xbee_.getResponse().getRx16Response(rx16);
    option = rx16.getOption();
    uint8_t* data = rx16.getData();
    const uint8_t length = rx16.getDataLength();

    const int min_length = min(length, data_length);
    memcpy(data_dst, data, min_length);
    return min_length;
  } else {
    return -2;
  }
}


inline XbeeBroadcastMessage::SendErr XbeeBroadcastMessage::SendPacket(
  const uint8_t* data, uint8_t length) {

  static const uint16_t kBroadcast = 0xFFFF;
  Tx16Request tx(kBroadcast, const_cast<uint8_t*>(data), length);
  TxStatusResponse tx_status;

  xbee_.send(tx);

  // after sending a tx request, we expect a status response
  // wait up to 5 seconds for the status response
  if (!xbee_.readPacket(5000)) {
    // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
    return kTimedOut;
  }

  if (xbee_.getResponse().isError()) {
    //nss.print("Error reading packet.  Error code: ");
    //nss.println(xbee.getResponse().getErrorCode());
    // or flash error led
    return kNoResponse;
  }

  if (xbee_.getResponse().getApiId() != TX_STATUS_RESPONSE) {
    return kInvalidResponse;
  }

  // should be a znet tx status
  xbee_.getResponse().getZBTxStatusResponse(tx_status);
  // get the delivery status, the fifth byte
  if (tx_status.getStatus() != SUCCESS) {
    // the remote XBee did not receive our packet. is it powered on?
    return kNoRecipients;
  }

  // success.  time to celebrate
  return kOk;
}


#endif  // _XBEE_BROADCAST_MESSAGE_H_