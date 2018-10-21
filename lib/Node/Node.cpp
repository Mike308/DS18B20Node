#include "Node.h"

Node::Node(DallasTemperature *bus, RF24Network *network) {
  this->bus = bus;
  this->network = network;
}

String Node::getSensorAddress(uint8_t *address) {
  String sensorAddress;
  for (uint8_t i = 0; i < 8; i++) {
    sensorAddress += String(address[i], 16);
  }
  return sensorAddress;
}

String Node::getSensorAddresses(uint8_t factor) {
  uint8_t sensorAddress[8];
  uint8_t sensorCnt = bus->getDeviceCount();
  String atResponse = "+S=" + String(sensorCnt) + "^";
  for (uint8_t i = (factor * 4); i < (factor * 4) + 4; i++) {
    bus->getAddress(sensorAddress, i);
    String address = getSensorAddress(sensorAddress);
    if (i > sensorCnt - 1) {
      atResponse += address;
      break;
    } else if (i < ((factor * 4) + 4) - 1) {
      atResponse += address + ";";
    } else if (i == ((factor * 4) + 4) - 1) {
      atResponse += address;
    }
  }
  return atResponse;
}

String Node::getATResponse(uint8_t factor) {
  uint8_t sensorCnt = bus->getDeviceCount();
  uint8_t sensorAddress[8];
  String atResponse = "+T=" + String(sensorCnt) + "^";
  for (uint8_t i = (factor * 4); i < (factor * 4) + 4; i++) {
    String temperature = String(bus->getTempCByIndex(i));
    bus->getAddress(sensorAddress, i);
    String sensorAddressStr = getSensorAddress(sensorAddress);
    if (i > sensorCnt - 1) {
      atResponse += temperature + "#" + sensorAddressStr;
      break;
    } else if (i < ((factor * 4) + 4) - 1) {
      atResponse += temperature + "#" + sensorAddressStr + ";";
    } else if (i == ((factor * 4) + 4) - 1) {
      atResponse += temperature + "#" + sensorAddressStr;
    }
  }
  return atResponse;
}

void Node::start() {
  if (network->receiveMessage(rxBuff)) {
    Serial.print("AT request: ");
    Serial.println(rxBuff);
    char *atCommand = strtok(rxBuff, "?");
    if (!strcmp("AT+DS18B20", atCommand)) {
      String atResponse = getATResponse(0);
      network->sendMessage(atResponse);
    } else if (!strncmp("AT+DS18B20,", atCommand, 11)){
      strtok(atCommand, ",");
      char *factorStr = strtok(NULL, ",");
      uint8_t factor = atoi(factorStr);
      String atResponse = getATResponse(factor);
      network->sendMessage(atResponse);
    } else if (!strcmp("AT+SENS", atCommand)) {
      String atResponse = getSensorAddresses(0);
      network->sendMessage(atResponse);
    } else if (!strncmp("AT+SENS,", atCommand, 8)){
      strtok(atCommand, ",");
      char *factorStr = strtok(NULL, ",");
      uint8_t factor = atoi(factorStr);
      String atResponse = getSensorAddresses(factor);
      network->sendMessage(atResponse);
    }
  }
}

void Node::poolSensors(unsigned long currentMillis) {
  static unsigned long previousMillis = 0;
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    bus->requestTemperatures();
  }
}
