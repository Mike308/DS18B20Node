#include "RF24Network.h"

RF24Network::RF24Network(uint16_t cePin, uint16_t csPin, String nodeAddress,
                         String separator)
    : RF24(cePin, csPin) {
  this->nodeAddress = nodeAddress;
  this->separator = separator;
}

bool RF24Network::sendMessage(String message) {
  this->stopListening();
  String dataToSend = nodeAddress + String(separator) + message;
  dataToSend.toCharArray(buf, TX_BUF_SIZE);
  bool status = this->write(&buf, strlen(buf));
  this->startListening();
  return status;
}

void RF24Network::setNodeAddress(String nodeAddress) {
  this->nodeAddress = nodeAddress;
}

bool RF24Network::receiveMessage(char *data) {
  if (this->available()) {
    char msg[RX_BUF_SIZE];
    uint8_t len = 0;
    len = this->getDynamicPayloadSize();
    this->read(&msg, len);
    char *parsed = strtok(msg, separator.c_str());

    if (String(parsed).compareTo(nodeAddress) == 0) {
      char *parsedData = strtok(NULL, separator.c_str());
      char rxBuf[RX_BUF_SIZE];
      String(parsedData).toCharArray(rxBuf, RX_BUF_SIZE);
      for (uint8_t i = 0; i < String(rxBuf).length(); i++) {
        *data++ = rxBuf[i];
      }

      return true;
    } else {
      return false;
    }
  }

  return false;
}
