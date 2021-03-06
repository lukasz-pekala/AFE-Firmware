/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-Rainmeter.h"

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR

AFESensorRainmeter::AFESensorRainmeter(){};

boolean AFESensorRainmeter::begin(AFEDataAccess *Data,
                                  AFESensorBinary *Sensor) {
  _Data = Data;
  _Sensor = Sensor;
  Data->getConfiguration(&configuration);

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
#endif

  _initialized = true;

  _Sensor->begin(configuration.sensitiveness);
#ifdef DEBUG
  Serial << endl
         << F("INFO: Rain sensor initialized and working") << endl
         << F(" - GPIO: ") << configuration.gpio << endl
         << F(" - Interval: ") << configuration.interval << endl
         << F(" - Boucing: ") << configuration.sensitiveness;
#endif

  RAINMETER_DATA _previous;
  _Data->get(&_previous);

  memcpy(current.last1h, _previous.last1h, sizeof(_previous.last1h[0]) * 60);
  current.index1h = _previous.index1h;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  current.counter = _previous.counter;
#else
  for (uint8_t i = 0; i < 12; i++) {
    current.last12h[i] = _previous.last12h[i];
  }
  current.last24h[0] = _previous.last24h[0];
  current.last24h[1] = _previous.last24h[1];

  current.index12h = _previous.index12h;
  current.index24h = _previous.index24h;
#endif
  startTime = millis();
  start60Sec = millis();

  return _initialized;
}

boolean AFESensorRainmeter::listener(void) {
  boolean _ret = false;
  if (_initialized) {

    // Calculate every 60sec
    if ((millis() - start60Sec >= 60000)) {

      uint32_t noOfImpulses;
      uint32_t duration;
      _Sensor->get(noOfImpulses, duration);

      /* Level during last 1 minute */
      rainLevelLast1Minute = noOfImpulses * configuration.resolution / 1000;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      current.counter += rainLevelLast1Minute;
#endif
      /* Storing water level for passed 1 minute */
      current.last1h[current.index1h] = rainLevelLast1Minute;

      /* Recalculating the water level during the last hour */
      rainLevelLastHour = 0;
      for (uint8_t i = 0; i <= current.index1h; i++) {
        rainLevelLastHour = rainLevelLastHour + current.last1h[i];
      }

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED // Not required for Domoticz
      /* Storing water level for passed hour */

      current.last12h[current.index12h] = rainLevelLastHour;
      /* Recalculating the water level during the last 12 hours */
      rainLevelLast12Hours = 0;

      for (uint8_t i = 0; i <= current.index12h; i++) {
        rainLevelLast12Hours = rainLevelLast12Hours + current.last12h[i];
      }

      current.last24h[current.index24h] = rainLevelLast12Hours;

      rainLevelLast24Hours = current.last24h[0] + current.last24h[1];

      if (current.index1h >= 59) {

        if (current.index12h >= 11) {
          current.index24h = current.index24h == 0 ? 1 : 0;
        }

        /* Changing index for 12 hours array */
        current.index12h = current.index12h >= 11 ? 0 : current.index12h + 1;
      }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

      /* Changing index for 60 minutes array */
      current.index1h = current.index1h >= 59 ? 0 : current.index1h + 1;

#ifdef DEBUG
      Serial << endl
             << F("INFO: Reading rain sensor data:") << endl
             << F(" - last 1min: ") << rainLevelLast1Minute << F(" mm/min")
             << endl
             << F(" - last hour: ") << rainLevelLastHour << F(" mm/hour")
             << endl
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
             << F(" - last 12 hours: ") << rainLevelLast12Hours << F(" mm/12-hours")
             << endl
             << F(" - last 24 hours: ") << rainLevelLast24Hours
             << F(" mm/24-hours") << endl
#endif
             << F(" - INDEX: ") << endl
             << F("  : Hour Index= ") << current.index1h << endl
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
             << F("  : 12 Hours Index = ") << current.index12h << endl
             << F("  : 24 Hours index = ") << current.index24h
#endif
             << endl;
#endif

      start60Sec = millis();
    }
  }

  /* Counter for calculating the data */
  if ((millis() - startTime >= configuration.interval * 1000)) {
    startTime = millis();
    _Data->save(&current);
    _ret = true;
  }

  return _ret;
}

void AFESensorRainmeter::getJSON(char *json) {
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  sprintf(json, "{\"rainmeter\":[{\"value\":%.3f,\"unit\":\"mm/"
                "min\"},{\"value\":%.3f,\"unit\":\"mm/"
                "h\"},{\"value\":%.3f,\"unit\":\"mm/"
                "12h\"},{\"value\":%.3f,\"unit\":\"mm/24h\"}]}",
          rainLevelLast1Minute, rainLevelLastHour, rainLevelLast12Hours,
          rainLevelLast24Hours);
#else
  sprintf(json, "{\"rainmeter\":[{\"value\":%.3f,\"unit\":\"mm/"
                "min\"},{\"value\":%.3f,\"unit\":\"mm/"
                "h\"}]}",
          rainLevelLast1Minute, rainLevelLastHour);
#endif
}

#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR