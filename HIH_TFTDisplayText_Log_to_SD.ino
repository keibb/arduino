/*
  Arduino TFT text example

  This example demonstrates how to draw text on the
  TFT with an Arduino. The Arduino reads the value
  of an analog sensor attached to pin A0, and writes
  the value to the LCD screen, updating every
  quarter second.

  This example code is in the public domain

  Created 15 April 2013 by Scott Fitzgerald

  http://www.arduino.cc/en/Tutorial/TFTDisplayText

*/

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#include <Wire.h>
#include <HIH61XX.h>

#include <SD.h>


//  Create an HIH61XX with I2C address 0x27
HIH61XX hih(0x27);

// pin definition for the Uno
#define LCD_cs  10
#define dc   9
#define rst  8
#define SD_cs 4

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

// create an instance of the library
TFT TFTscreen = TFT(LCD_cs, dc, rst);

// char array to print to the screen
char sensorPrintouth[6];
char sensorPrintoutt[6];

void setup() {
  Wire.begin();

  pinMode(LCD_cs, OUTPUT);
  digitalWrite(LCD_cs, HIGH);
  pinMode(SD_cs, OUTPUT);
  digitalWrite(SD_cs, HIGH);
  SPI.begin();


  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("Humidity:\n ", 0, 0);
  TFTscreen.text("Temperature:\n ", 0 , 40);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(2);

  if (!SD.begin(SD_cs))
  {
    return;
  }
}

void loop() {
  hih.start();
  hih.update();
  float hum = hih.humidity();
  float temp = hih.temperature();

  String dataStringh = String(hum * 100, 2);
  String dataStringt = String(temp, 2);

  // convert the reading to a char array
  dataStringh.toCharArray(sensorPrintouth, 6);
  dataStringt.toCharArray(sensorPrintoutt, 6);

  digitalWrite(LCD_cs, LOW);
  // set the font color
  TFTscreen.stroke(255, 255, 255);
  // print the sensor value
  TFTscreen.text(sensorPrintouth, 0, 20);
  TFTscreen.text(sensorPrintoutt, 0, 60);
  // wait for a moment
  delay(5000);
  // erase the text you just wrote
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintouth, 0, 20);
  TFTscreen.text(sensorPrintoutt, 0, 60);
  digitalWrite(LCD_cs, HIGH);
  digitalWrite(SD_cs, LOW);

  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    String dataString = "";
    dataString += dataStringh;
    dataString += ",";
    dataString += dataStringt;
    dataFile.println(dataString);
    dataFile.close();

  }
  else {
    return;
  }
  digitalWrite(SD_cs, HIGH);
}


