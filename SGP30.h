/***************************************************
  This is a library for the SGP30 Digital VOC Sensor

  Written by Limor Fried/Ladyada for Adafruit Industries.
  Changed to be used for SGP30 sensors instead of SHT3x by Marcel Hecko (maco@blava.net)
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

#define SGP30_DEFAULT_ADDR         0x58
#define SGP30_MEASURE_AIR_QUALITY  0x2008
#define SGP30_MEASURE_TEST         0x2032
#define SGP30_SOFT_RESET           0x0006

class SGP30 {
 public:
  SGP30();
  boolean begin(uint8_t i2caddr = SGP30_DEFAULT_ADDR);
  uint16_t readCo2eq(void);
  uint16_t readTvoc(void);
  void reset(void);
  uint8_t crc8(const uint8_t *data, int len);
  uint16_t measureTest(void);

 private:
  boolean readAirq(void);
  void writeCommand(uint16_t cmd);

  uint8_t _i2caddr;
  boolean readData(void);
  uint16_t co2eq, tvoc;
};

