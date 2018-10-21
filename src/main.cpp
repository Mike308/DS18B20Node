
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <printf.h>
#include "RF24Network.h"
#include "Node.h"

uint64_t pipeAddress = 0xF0F0F0F0F101;
uint64_t receivingPipeAddress = 0xF0F0F0F0E101;
char rxBuf[32];
uint8_t sensorAddressArray[8];
unsigned long  currentMillis;

RF24Network node(9, 10, "0");
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);
Node customNode(&sensors, &node);

void setup() {
  Serial.begin(9600);
  node.begin();
  node.setRetries(15, 15);
  node.openWritingPipe(pipeAddress);
  node.openReadingPipe(1, receivingPipeAddress);
  node.setAutoAck(true);
  node.startListening();
  sensors.begin();
}

void loop() {
  currentMillis = millis();
  customNode.poolSensors(currentMillis);
  customNode.start();
}
