#include "U8glib.h"
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10); //RX,TX

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
}
/*
  void loop() {
  // put your main code here, to run repeatedly:
  getCo2();
  delay(10000);
  }
*/
void loop()
{
  byte cmd[9] = { 0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  const int len = 9;
  byte value[9];
  String val;
  int checksum = 0;
    int retry = 0;
    char charval[10];
  mySerial.write(cmd, len);


  while (!mySerial.available()) {
    delay(100);
    if (retry++ > 0) {
      break;
    }
  }
  while (mySerial.available() > 0) {
    for (int i = 0; i < len; i++) {
      value[i] = mySerial.read();
    }
    for (int j = 1; j < 8; j++) {
      checksum += value[j];
    }
    checksum = 0xff - checksum;
    checksum += 1;
    if (checksum = value[8]
                   //value[0] == 0xff && value[1] == 0x86
       ) {
      val = "CO2:" + String(value[2] * 256 + value[3]);
      val.toCharArray(charval,10);
      Serial.println(val);
        u8g.setFont(u8g_font_unifont);
        u8g.firstPage();  
  do {
    u8g.drawStr( 0, 22, charval);
  } while( u8g.nextPage() );
        
    }
  }
  delay(5000);
}
