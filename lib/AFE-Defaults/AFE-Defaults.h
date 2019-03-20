/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Defaults_h
#define _AFE_Defaults_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>
#include <AFE-Data-Access.h>
#include <AFE-EEPROM.h>
#include <ESP8266WiFi.h>

#ifdef CONFIG_HARDWARE_SPIFFS
#include <FS.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEDefaults {
private:
  AFEEEPROM Eeprom;
  AFEDataAccess *Data;

public:
  AFEDefaults();

  void addDomoticzConfiguration();
#ifdef CONFIG_HARDWARE_LED
  void addLEDConfiguration(uint8_t id, uint8_t gpio);
#endif

  /* Method erases EEPROM */
  void eraseConfiguration();

#ifdef CONFIG_HARDWARE_SPIFFS
  void formatSPIFFS();
#endif

  /* Method set default values */
  void set();
};
#endif
