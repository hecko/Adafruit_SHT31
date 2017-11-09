/***************************************************
  This is a library for the SGP30 Digital VOC Sensor

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "SGP30.h"

SGP30::SGP30() {
}


boolean SGP30::begin(uint8_t i2caddr) {
  Wire.begin();
  _i2caddr = i2caddr;
  reset();
  return true;
}

uint16_t SGP30::measureTest(void) {
  writeCommand(SGP30_MEASURE_TEST);
  delay(230); // min 220 as stated in datasheet
  Wire.requestFrom(_i2caddr, (uint8_t)3);
  uint16_t stat = Wire.read();
  stat <<= 8;
  stat |= Wire.read();
  return stat;
}

void SGP30::reset(void) {
  writeCommand(SGP30_SOFT_RESET);
  delay(10);
}

uint16_t SGP30::readCo2eq(void) {
  if (! readAirq()) return NAN;
  return co2eq;
}

uint16_t SGP30::readTvoc(void) {
  if (! readAirq()) return NAN;
  return tvoc;
}

// we should run the following command for at least 15 seconds
// to start getting real numbers, otherwise 400 cor CO2eq and
// 0 for TVOC will be returned
boolean SGP30::readAirq(void) {
  byte readbuffer[6];

  writeCommand(SGP30_MEASURE_AIR_QUALITY);

  delay(15); // 12 minimum per datasheet
  Wire.requestFrom(_i2caddr, (uint8_t) 6);
  if (Wire.available() != 6)
    return false;
  for (uint8_t i=0; i<6; i++) {
    readbuffer[i] = Wire.read();
    // Serial.write(readbuffer[i]);
  }

  uint16_t CO2EQ, TVOC;

  CO2EQ = readbuffer[0];
  CO2EQ <<= 8;
  CO2EQ |= readbuffer[1];

  TVOC = readbuffer[3];
  TVOC <<= 8;
  TVOC |= readbuffer[4];

  if (readbuffer[2] != crc8(readbuffer, 2)) return false;
  if (readbuffer[5] != crc8(readbuffer+3, 2)) return false;

  co2eq = CO2EQ;
  tvoc = TVOC;

  return true;
}


void SGP30::writeCommand(uint16_t cmd) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(cmd >> 8);
  Wire.write(cmd & 0xFF);
  Wire.endTransmission();
}

uint8_t SGP30::crc8(const uint8_t *data, int len)
{
/*
*
 * CRC-8 formula from page 14 of SHT spec pdf
 * CRC-8 formula from page 10 of SGP30 spec pdf
 *
 * Test data 0xBE, 0xEF should yield 0x92
 *
 * Initialization data 0xFF
 * Polynomial 0x31 (x8 + x5 +x4 +1)
 * Final XOR 0x00
 */

  const uint8_t POLYNOMIAL(0x31);
  uint8_t crc(0xFF);

  for ( int j = len; j; --j ) {
      crc ^= *data++;

      for ( int i = 8; i; --i ) {
	crc = ( crc & 0x80 )
	  ? (crc << 1) ^ POLYNOMIAL
	  : (crc << 1);
      }
  }

  return crc;
}

/*********************************************************************/
