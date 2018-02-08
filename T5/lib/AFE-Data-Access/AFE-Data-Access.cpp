/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

DEVICE AFEDataAccess::getDeviceConfiguration() {
  DEVICE configuration;

  Eeprom.read(9, 16).toCharArray(configuration.name,
                                 sizeof(configuration.name));

  uint8_t index = 3;
  for (uint8_t i = 0; i < sizeof(configuration.isLED); i++) {
    configuration.isLED[i] = Eeprom.read(366 + i * index);
  }

  index = 8;
  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
    configuration.isSwitch[i] = Eeprom.read(397 + i * index);
  }

  index = 24;
  for (uint8_t i = 0; i < sizeof(configuration.isContactron); i++) {
    configuration.isContactron[i] = Eeprom.read(413 + i * index);
  }

  configuration.isDHT = Eeprom.read(376);
  configuration.httpAPI = Eeprom.read(25);
  configuration.mqttAPI = Eeprom.read(228);

  return configuration;
}

FIRMWARE AFEDataAccess::getFirmwareConfiguration() {
  FIRMWARE configuration;

  Eeprom.read(0, 7).toCharArray(configuration.version,
                                sizeof(configuration.version));
  Eeprom.read(28, 120).toCharArray(configuration.upgradeURL,
                                   sizeof(configuration.upgradeURL));
  configuration.type = Eeprom.readUInt8(7);
  configuration.autoUpgrade = Eeprom.readUInt8(27);
  return configuration;
}

NETWORK AFEDataAccess::getNetworkConfiguration() {
  NETWORK configuration;

  Eeprom.read(148, 32).toCharArray(configuration.ssid,
                                   sizeof(configuration.ssid));
  Eeprom.read(180, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));

  configuration.isDHCP = Eeprom.read(212);
  configuration.ip = Eeprom.readIP(213);
  configuration.gateway = Eeprom.readIP(217);
  configuration.subnet = Eeprom.readIP(221);
  configuration.noConnectionAttempts = Eeprom.readUInt8(225);
  configuration.waitTimeConnections = Eeprom.readUInt8(226);
  configuration.waitTimeSeries = Eeprom.readUInt8(227);
  return configuration;
}

MQTT AFEDataAccess::getMQTTConfiguration() {
  MQTT configuration;

  Eeprom.read(229, 32).toCharArray(configuration.host,
                                   sizeof(configuration.host));
  configuration.ip = Eeprom.readIP(261);
  configuration.port = Eeprom.read(265, 5).toInt();
  Eeprom.read(270, 32).toCharArray(configuration.user,
                                   sizeof(configuration.user));
  Eeprom.read(302, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));
  Eeprom.read(334, 32).toCharArray(configuration.topic,
                                   sizeof(configuration.topic));

  return configuration;
}

LED AFEDataAccess::getLEDConfiguration(uint8_t id) {
  LED configuration;
  uint8_t nextLed = 3;

  configuration.gpio = Eeprom.readUInt8(367 + id * nextLed);
  configuration.changeToOppositeValue = Eeprom.read(368 + id * nextLed);
  return configuration;
}

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
  RELAY configuration;
  uint8_t nextRelay = 5;

  configuration.gpio = Eeprom.readUInt8(461 + id * nextRelay);
  configuration.timeToOff = Eeprom.read(462 + id * nextRelay, 4).toInt();

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;
  uint8_t nextSwitch = 8;
  configuration.gpio = Eeprom.readUInt8(398 + id * nextSwitch);
  configuration.type = Eeprom.readUInt8(399 + id * nextSwitch);
  configuration.sensitiveness = Eeprom.read(400 + id * nextSwitch, 3).toInt();
  configuration.functionality = Eeprom.readUInt8(403 + id * nextSwitch);
  configuration.relayID = Eeprom.readUInt8(404 + id * nextSwitch);
  return configuration;
}

CONTACTRON AFEDataAccess::getContactronConfiguration(uint8_t id) {
  CONTACTRON configuration;
  uint8_t nextContactron = 24;
  MQTT configurationMQTT;
  char mqttTopic[49];

  configuration.gpio = Eeprom.readUInt8(414 + id * nextContactron);
  configuration.outputDefaultState =
      Eeprom.readUInt8(415 + id * nextContactron);
  configuration.ledID = Eeprom.readUInt8(416 + id * nextContactron);
  configuration.bouncing = Eeprom.read(417 + id * nextContactron, 4).toInt();

  Eeprom.read(421 + id * nextContactron, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  return configuration;
}

DH AFEDataAccess::getDHTConfiguration() {
  DH configuration;
  configuration.gpio = Eeprom.readUInt8(377);
  configuration.type = Eeprom.readUInt8(378);
  configuration.temperature.interval = Eeprom.read(379, 5).toInt();
  configuration.temperature.unit = Eeprom.readUInt8(384);
  configuration.temperature.correction = Eeprom.read(385, 4).toFloat();
  configuration.humidity.interval = Eeprom.read(389, 5).toInt();
  configuration.humidity.correction = Eeprom.read(394, 3).toFloat();
  return configuration;
}

void AFEDataAccess::saveConfiguration(DEVICE configuration) {
  Eeprom.write(9, 16, configuration.name);

  uint8_t index = 8;
  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
    Eeprom.write(397 + i * index, configuration.isSwitch[i]);
  }

  index = 3;
  for (uint8_t i = 0; i < sizeof(configuration.isLED); i++) {
    Eeprom.write(366 + i * index, configuration.isLED[i]);
  }

  index = 24;
  for (uint8_t i = 0; i < sizeof(configuration.isContactron); i++) {
    Eeprom.write(413 + i * index, configuration.isContactron[i]);
  }

  Eeprom.write(376, configuration.isDHT);
  Eeprom.write(25, configuration.httpAPI);
  Eeprom.write(228, configuration.mqttAPI);
}

void AFEDataAccess::saveConfiguration(FIRMWARE configuration) {
  Eeprom.write(0, 7, configuration.version);
  Eeprom.writeUInt8(7, configuration.type);
  Eeprom.writeUInt8(27, configuration.autoUpgrade);
  Eeprom.write(28, 120, configuration.upgradeURL);
}

void AFEDataAccess::saveConfiguration(NETWORK configuration) {
  Eeprom.write(148, 32, configuration.ssid);
  Eeprom.write(180, 32, configuration.password);
  Eeprom.write(212, configuration.isDHCP);
  Eeprom.writeIP(213, configuration.ip);
  Eeprom.writeIP(217, configuration.gateway);
  Eeprom.writeIP(221, configuration.subnet);
  Eeprom.writeUInt8(225, configuration.noConnectionAttempts);
  Eeprom.writeUInt8(226, configuration.waitTimeConnections);
  Eeprom.writeUInt8(227, configuration.waitTimeSeries);
}

void AFEDataAccess::saveConfiguration(MQTT configuration) {
  Eeprom.write(229, 32, configuration.host);
  Eeprom.writeIP(261, configuration.ip);
  Eeprom.write(265, 5, (long)configuration.port);
  Eeprom.write(270, 32, configuration.user);
  Eeprom.write(302, 32, configuration.password);
  Eeprom.write(334, 32, configuration.topic);
}

void AFEDataAccess::saveConfiguration(uint8_t id, RELAY configuration) {
  uint8_t nextRelay = 5;
  Eeprom.writeUInt8(461 + id * nextRelay, configuration.gpio);
  Eeprom.write(462 + id * nextRelay, 4, long(configuration.timeToOff));
}

void AFEDataAccess::saveConfiguration(uint8_t id, LED configuration) {
  uint8_t nextLed = 3;

  Eeprom.writeUInt8(367 + id * nextLed, configuration.gpio);
  Eeprom.write(368 + id * nextLed, configuration.changeToOppositeValue);
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
  uint8_t nextSwitch = 8;
  Eeprom.writeUInt8(398 + id * nextSwitch, configuration.gpio);
  Eeprom.writeUInt8(399 + id * nextSwitch, configuration.type);
  Eeprom.write(400 + id * nextSwitch, 3, (long)configuration.sensitiveness);
  Eeprom.writeUInt8(403 + id * nextSwitch, configuration.functionality);
  Eeprom.writeUInt8(404 + id * nextSwitch, configuration.relayID);
}

void AFEDataAccess::saveConfiguration(uint8_t id, CONTACTRON configuration) {
  uint8_t nextContactron = 24;
  Eeprom.writeUInt8(414 + id * nextContactron, configuration.gpio);
  Eeprom.writeUInt8(415 + id * nextContactron,
                    configuration.outputDefaultState);
  Eeprom.writeUInt8(416 + id * nextContactron, configuration.ledID);
  Eeprom.write(417 + id * nextContactron, 4, (long)configuration.bouncing);
  Eeprom.write(421 + id * nextContactron, 16, configuration.name);
}

void AFEDataAccess::saveConfiguration(DH configuration) {
  Eeprom.writeUInt8(377, configuration.gpio);
  Eeprom.writeUInt8(378, configuration.type);
  Eeprom.write(379, 5, (long)configuration.temperature.interval);
  Eeprom.writeUInt8(384, configuration.temperature.unit);
  Eeprom.write(385, 4, (float)configuration.temperature.correction);
  Eeprom.write(389, 5, (long)configuration.humidity.interval);
  Eeprom.write(394, 3, (float)configuration.humidity.correction);
}

const char AFEDataAccess::getVersion() {
  char version[7];
  Eeprom.read(0, 7).toCharArray(version, sizeof(version));
  return *version;
}

void AFEDataAccess::saveVersion(String version) { Eeprom.write(0, 7, version); }

uint8_t AFEDataAccess::getDeviceMode() { return Eeprom.readUInt8(26); }

void AFEDataAccess::saveDeviceMode(uint8_t mode) {
  Eeprom.writeUInt8(26, mode);
}

uint8_t AFEDataAccess::getLanguage() { return Eeprom.readUInt8(8); }

void AFEDataAccess::saveLanguage(uint8_t language) {
  Eeprom.writeUInt8(8, language);
}

uint8_t AFEDataAccess::getSystemLedID() { Eeprom.readUInt8(375); }

void AFEDataAccess::saveSystemLedID(uint8_t id) { Eeprom.writeUInt8(375, id); }

boolean AFEDataAccess::getRelayState(uint8_t id) { return false; }

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {}
