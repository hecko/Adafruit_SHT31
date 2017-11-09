/***************************************************
  This is an example for the SHT31-D Humidity & Temp Sensor

  Designed specifically to work with the SHT31-D sensor from Adafruit
  ----> https://www.adafruit.com/products/2857

  These sensors use I2C to communicate, 2 pins are required to
  interface
 ****************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "SGP30.h"

#define VOC_EN     6
#define VOC_LLC_EN A2

SGP30 sgp30 = SGP30();

void setup() {
  pinMode(VOC_EN, OUTPUT);
  pinMode(VOC_LLC_EN, OUTPUT);

  digitalWrite(VOC_EN, HIGH);
  delay(10);
  digitalWrite(VOC_LLC_EN, HIGH);
  delay(10);
  
  Serial.begin(9600);

  while (!Serial)
    delay(10);

  Serial.println("SHT31 test");
  if (! sgp30.begin(SGP30_DEFAULT_ADDR)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}


void loop() {
  Serial.print(sgp30.readCo2eq());
  delay(100);
  Serial.print(sgp30.readTvoc());
  delay(1000);
}
