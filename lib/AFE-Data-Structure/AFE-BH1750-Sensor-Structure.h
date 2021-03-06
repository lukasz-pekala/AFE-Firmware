/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_BH1750_Structure_h
#define _AFE_BH1750_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

struct BH1750 {
  char name[17];
  uint32_t interval;
  uint8_t i2cAddress;
  uint8_t mode;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif
