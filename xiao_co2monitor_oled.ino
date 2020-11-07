#include <U8g2lib.h>
#include <U8x8lib.h>


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  u8g2.begin();
}

void loop()
{
  byte cmd[9] = { 0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  const int len = 9;
  byte value[9];
  String val;
  int checksum = 0;
  int retry = 0;
  char charval[10];
  char pastval[] = "";
  Serial1.write(cmd, len);

  //u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.setFont(u8g2_font_8x13_tf); //u8g2_font_micro_tr
  while (!Serial1.available()) {
    delay(100);
    if (retry++ > 0) {
      break;
    }
  }
  while (Serial1.available() > 0) {
    for (int i = 0; i < len; i++) {
      value[i] = Serial1.read();
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
      val.toCharArray(charval, 10);
      Serial.println(val);
      u8g2.clearBuffer();          // clear the internal memory
      u8g2.drawStr( 0, 10, pastval);  // write something to the internal memory
      u8g2.drawStr( 0, 22, charval);
      u8g2.sendBuffer();          // transfer internal memory to the display
      pastval = charval;  
    }

  }

  delay(5000);
}
