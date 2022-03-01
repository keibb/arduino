#include <Seeed_BME280.h>

/*
    note: need add library Adafruit_BMP280 from library manage
    Github: https://github.com/adafruit/Adafruit_BMP280_Library
*/

#include <M5StickC.h>
#include <Wire.h>

BME280 bme;  


void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin(32, 33);
  M5.Axp.ScreenBreath(9);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);

  pinMode(M5_BUTTON_HOME, INPUT);

  if (!bme.init()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}
uint8_t setup_flag = 1;
void loop() {
  // put your main code here, to run repeatedly:
  float tmp = bme.getTemperature();
  float hum = bme.getHumidity();
  
  M5.Lcd.setCursor(0, 0, 4);
  M5.Lcd.printf("Temp: %2.1f", tmp);
  M5.Lcd.setCursor(0, 20, 4);
  M5.Lcd.printf("Humi: %2.0f%%", hum);
  
  float pressure = bme.getPressure();
  M5.Lcd.setCursor(0, 60, 2);
  M5.Lcd.printf("pressure: %2.1f", pressure);
  delay(100);
  if (!setup_flag) {
    setup_flag = 1;
    if (!bme.init()) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
    }
  }
  if (digitalRead(M5_BUTTON_HOME) == LOW) {
    setup_flag = 0;
    while (digitalRead(M5_BUTTON_HOME) == LOW);
  }
}
