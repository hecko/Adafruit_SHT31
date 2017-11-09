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

SGP30 sgp30 = SGP30();

void setup() {
  Serial.begin(9600);

  while (!Serial)
    delay(10);

  Serial.println("SGP30 test");
  if (! sgp30.begin(SGP30_DEFAULT_ADDR)) {
    Serial.println("Couldn't find SGP30");
    while (1) delay(1);
  }
}


void loop() {
  Serial.print(sgp30.readCo2eq());
  delay(100);
  Serial.print(sgp30.readTvoc());
  delay(1000);
}
