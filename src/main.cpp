
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <printf.h>
#include "RF24Network.h"

uint64_t pipeAddress = 0xF0F0F0F0F101;
uint64_t receivingPipeAddress = 0xF0F0F0F0E101;
char rxBuf[32];

RF24Network node(9, 10, "ds18Node");
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  node.begin();
  node.openWritingPipe(pipeAddress);
  node.openReadingPipe(1, receivingPipeAddress);
  node.startListening();
  sensors.begin();
}

void loop() {



  if (node.receiveMessage(rxBuf)) {
    char *atCommand = strtok(rxBuf, "?");
    if (strcmp(atCommand, "AT+DS18B20") == 0) {
      sensors.requestTemperatures();
      float temperature = sensors.getTempCByIndex(0);
      String atCommandStr = "AT+DS18B20=";
      String strValues = String(temperature);
      atCommandStr += strValues;
      node.sendMessage(atCommandStr);
      Serial.println(atCommandStr);
    }
  }
}
