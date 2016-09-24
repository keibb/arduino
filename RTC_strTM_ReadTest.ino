#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}

void loop() {
  tmElements_t tm;

  if (RTC.read(tm)) {

    String strH = String(tm.Hour);
    String strM = String(tm.Minute);
    String strS = String(tm.Second);

    if (strH.length() == 1) {
      strH = String("0" + strH);
    }

    if (strM.length() == 1) {
      strM = String("0" + strM);
    }

    if (strS.length() == 1) {
      strS = String("0" + strS);
    }

    String strTM = String(strH +":"+ strM +":"+ strS );
    Serial.println(strTM);

  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}


