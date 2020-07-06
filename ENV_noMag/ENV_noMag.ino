/*
    note: need add library Adafruit_BMP280 from library manage
    Github: https://github.com/adafruit/Adafruit_BMP280_Library
*/

#include <M5StickC.h>
#include "DHT12.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>

DHT12 dht12;
Adafruit_BMP280 bme;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin(0, 26);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);

  pinMode(M5_BUTTON_HOME, INPUT);

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}
uint8_t setup_flag = 1;
void loop() {
  // put your main code here, to run repeatedly:
  float tmp = dht12.readTemperature();
  float hum = dht12.readHumidity();
  M5.Lcd.setCursor(0, 0, 4);
  M5.Lcd.printf("Temp: %2.1f", tmp);
  M5.Lcd.setCursor(0, 20, 4);
  M5.Lcd.printf("Humi: %2.0f%%", hum);
  float pressure = bme.readPressure();
  M5.Lcd.setCursor(0, 60, 2);
  M5.Lcd.printf("pressure: %2.1f", pressure);
  delay(100);
  if (!setup_flag) {
    setup_flag = 1;
    if (!bme.begin(0x76)) {
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
    }
  }
  if (digitalRead(M5_BUTTON_HOME) == LOW) {
    setup_flag = 0;
    while (digitalRead(M5_BUTTON_HOME) == LOW);
  }
}
