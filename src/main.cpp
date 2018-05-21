#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RF24Network.h"

uint64_t pipeAddress = 0xF0F0F0F0F101;
uint64_t receivingPipeAddress = 0xF0F0F0F0E101;
char rxBuf[128];

RF24Network node(9, 10, "ds18b20Node");
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);


void setup()
{
    sensors.begin();
    node.begin();
    node.begin();
    node.openWritingPipe(pipeAddress);
    node.openReadingPipe(1, receivingPipeAddress);
    node.startListening();
    Serial.begin(9600);
}

void loop()
{
    unsigned long currentMillis = millis();
    if ((currentMillis % 3000) == 0)
    {
        sensors.requestTemperatures();
        float temperature = sensors.getTempCByIndex(0);
        String atCommand = "AT+DS18B20=";
        String strValues = String(temperature);
        atCommand += strValues;
        node.sendMessage(atCommand);
        Serial.println(atCommand);
    } 
}