#include "CH375.h"

CH375::CH375(uint8_t s1, uint8_t s2) {
  port = new SoftwareSerial9(s1, s2, false);
  port->begin(9600);
}

unsigned char CH375::read() {
  for(int i = 0; i < 1000; ++i) {
    delay(1);
    if(port->available()) {
      uint8_t c = port->read();
      return c & 0xFF;
    }
  }
  return 0;
}

void CH375::write(uint16_t c) {
  port->write9(c);
  delay(1);
}

void CH375::cmd(uint16_t c) {
  write(c + 0x100); // bit 9 = 1
}
