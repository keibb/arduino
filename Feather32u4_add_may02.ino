#include"Adafruit_BluefruitLE_SPI.h"
Adafruit_BluefruitLE_SPI ble(8, 7, 4);
const int buttonPin = 2;
const int ledPin = 13;
boolean wasPressed = false;

void setup(void) {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  ble.begin(false);
  ble.factoryReset();
  Serial.println("Factory reset performed");
}

void loop(void) {
  boolean isPressed = (digitalRead(buttonPin) == HIGH);
  if (!wasPressed && isPressed) {
    digitalWrite(ledPin, HIGH);
    String command = "AT+BLEBEACON=";
    command += "0x004C,";
    command += "01-12-23-34-45-56-67-78-89-9A-AB-BC-CD-DE-EF-F0,";
    command += "0x0000,";
    command += "0x0000,";
    command += "-59";
    ble.sendCommandCheckOK(command.c_str());
    Serial.println("Advertising");
  }
  else if (wasPressed && !isPressed) {
    digitalWrite(ledPin, LOW);
    ble.factoryReset();
    Serial.println("Stopped advertising");
  }
  wasPressed = isPressed;
  delay(1000);
}
