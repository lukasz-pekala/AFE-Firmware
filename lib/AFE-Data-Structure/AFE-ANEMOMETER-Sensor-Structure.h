/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_ANEMOMETER_SENSOR_Structure_h
#define _AFE_ANEMOMETER_SENSOR_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct ANEMOMETER {
  uint8_t gpio;
  char name[17];
  uint8_t sensitiveness;
  uint32_t interval;
  float impulseDistance;
  uint8_t impulseDistanceUnit;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif
