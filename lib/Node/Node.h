#ifndef NODE_H
#define NODE_H
#include <OneWire.h>
#include <DallasTemperature.h>

#include "RF24Network.h"



class Node {
    public:
        Node(DallasTemperature *bus, RF24Network * network);
        void start();
        void poolSensors(unsigned long currentMillis);
    private:
        String getSensorAddress(uint8_t *address);
        String getSensorAddresses(uint8_t factor);
        String getATResponse(uint8_t factor);
        DallasTemperature *bus;
        RF24Network *network;
        char rxBuff[32];
        uint8_t _sensorCnt = 8;
};
#endif

