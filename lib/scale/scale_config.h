#include <Arduino.h>

#ifndef scale_config_h
#define scale_config_h
unsigned calibrationValue = 1897.0;
boolean _tare = true;
unsigned stabilizingtime = 2000;
const int calVal_eepromAdress = 0;
#endif