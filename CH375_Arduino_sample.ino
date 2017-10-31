#include <SoftwareSerial9.h>
#include "CH375.h"

CH375 ch375(10, 11);

void setup() {
  unsigned char res;
  Serial.begin(9600);
  
  Serial.println("Sending GET_IC_VER Command...");
  ch375.port->write9(0x101);
  
  for(int i = 0; i < 1000; ++i) {
    delay(1);
    if(ch375.port->available()) {
      Serial.println("got responce");
      res = ch375.port->read();
    }
  }
  Serial.print("0x");
  Serial.println(res, HEX);

  /*** 5.1 CMD_GET_IC_VER ***/
  Serial.println("\n## 5.1 CMD_GET_IC_VER");
  ch375.cmd(CMD_GET_IC_VER);
  res = ch375.read();
  Serial.print("CH375B Version: 0x");
  Serial.println(res - 0x80, HEX); // remove the bit 7 according to datasheet

  /*** 5.4 CMD_RESET_ALL ***/
  Serial.println("\n## 5.4 CMD_RESET_ALL: Hard resetting CH375");
  ch375.cmd(CMD_RESET_ALL);
  delay(200); // wait until reset is done

  /*** 5.9 CMD_SET_USB_MODE ***/
  Serial.println("\n## 5.9 CMD_SET_USB_MODE: USB-HOST with SOF package");
  ch375.cmd(CMD_SET_USB_MODE);
  ch375.write(0x06);
  res = ch375.read();
  if (res == CMD_RET_SUCCESS) {
    Serial.println("SET_USB_MODE success");
  }
  else {
    Serial.println("SET_USB_MODE failed");
  }

  /*** 5.10 CMD_TEST_CONNECT ***/
  Serial.println("\n## 5.10 CMD_TEST_CONNECT");
  ch375.cmd(CMD_TEST_CONNECT);
  handleInterruptionState(ch375.read());

  /*** 5.12 CMD_GET_STATUS ***/
  Serial.println("\n## 5.12 CMD_GET_STATUS");
  ch375.cmd(CMD_GET_STATUS);
  handleInterruptionState(ch375.read());

  /*** DS2 1.11 CMD_GET_DESCR ***/
  Serial.println("\n## DS2 1.11 CMD_GET_DESCR");
  ch375.cmd(CMD_GET_DESCR);
  ch375.write(0x01); // device descriptor
  ch375.cmd(CMD_GET_STATUS);
  handleInterruptionState(ch375.read());
}

void loop() {}

void handleInterruptionState(uint8_t c) {
  switch(c) {
    case USB_INT_SUCCESS:
      Serial.println("Operation successful");
      break;
    case USB_INT_CONNECT:
      Serial.println("USB device connected");
      break;
    case USB_INT_DISCONNECT:
      Serial.println("USB device disconnected");
      break;
    case USB_INT_BUF_OVER:
      Serial.println("Buffer overflow");
      break;
    case USB_INT_DISK_READ:
      Serial.println("Reading from USB storage");
      break;
    case USB_INT_DISK_WRITE:
      Serial.println("Writing to USB storage");
      break;
    case USB_INT_DISK_ERR:
      Serial.println("USB storage error");
      break;
    default:
      Serial.print("Error: operation failed with code 0x");
      Serial.println(c, HEX);
      break;
  }
}
