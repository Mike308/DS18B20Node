#ifndef RF24_NETWORK_H 
#define RF24_NETWORK_H
#include <RF24.h>
#include <math.h>
#define RX_BUF_SIZE 32
#define TX_BUF_SIZE 64

class RF24Network : public RF24 {

public:
  RF24Network(uint16_t cePin, uint16_t csPin) : RF24(cePin, csPin) {}
  RF24Network(uint16_t cePin, uint16_t csPin, String nodeAddress,
              String separator = "|");
  void setNodeAddress(String nodeAddress);
  bool sendMessage(String message);
  bool receiveMessage(char *data);

private:
  String nodeAddress;
  char buf[TX_BUF_SIZE];
  String separator;
  bool nativeWriteData(String message);

};
#endif