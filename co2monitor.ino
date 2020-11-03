#include <SoftwareSerial.h>
SoftwareSerial mySerial(9,10); //RX,TX

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
  int val;
  int checksum = 0;
  mySerial.write(cmd, len);
  int retry = 0;
  
  while(!mySerial.available()){
    delay(100);
    if(retry++>0){
      break;
      }
  }
  while(mySerial.available()>0){
    for(int i = 0;i < len;i++){
      value[i] = mySerial.read();
      }
      for(int j=1; j<8; j++){
        checksum += value[j];
        }
      checksum = 0xff - checksum;
      checksum += 1;
      if(checksum = value[8]
        //value[0] == 0xff && value[1] == 0x86
        ){
        val = value[2]*256 + value[3];
        Serial.println((float)val);
        }
      }
      delay(5000);
  }
