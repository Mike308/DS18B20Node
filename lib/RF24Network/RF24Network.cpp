#include "RF24Network.h"

RF24Network::RF24Network(uint16_t cePin, uint16_t csPin, String nodeAddress,
                         String separator)
    : RF24(cePin, csPin) {
  this->nodeAddress = nodeAddress;
  this->separator = separator;
}

bool RF24Network::sendMessage(String message) {
  this->stopListening();
  bool status = false;
  String dataToSend = nodeAddress + String(separator) + message;
  this->nativeWriteData(dataToSend);
  this->startListening();
  return status;
}

bool RF24Network::nativeWriteData(String data) {
  data += "$";
  uint8_t length = data.length();
  Serial.print("length: ");
  Serial.println(length);
  uint8_t iterations = ceil((length) / 32.00);
  char originalMessageBuf[length];
  char txBuffor[32];
  bool result;
  data.toCharArray(originalMessageBuf, length + 1);
  uint8_t i = 0;
  if (iterations == 1) {
    this->write(&originalMessageBuf, length);
  } else if (iterations == 2) {
    Serial.print("I[2]: ");
    for (uint8_t j = 0; j < 31; j++) {
      txBuffor[j] = originalMessageBuf[j];
      Serial.print(txBuffor[j]);
    }
    this->write(&txBuffor, 31);
    memset(txBuffor, 0, 32);
    Serial.println();
    Serial.print("II[2]: ");
    for (uint8_t j = 31; j < length; j++) {
      txBuffor[i] = originalMessageBuf[j];
      Serial.print(txBuffor[i]);
      i++;
      if (i > length - 32) {
        i = 0;
      }
    }
    result = this->write(&txBuffor, 31);
    memset(txBuffor, 0, 32);
  } else if (iterations == 3) {
    Serial.print("I[3]: ");
    i = 0;
    for (uint8_t j = 0; j < 31; j++) {
      txBuffor[j] = originalMessageBuf[j];
      Serial.print(txBuffor[j]);
    }
    this->write(&txBuffor, 31);
    memset(txBuffor, 0, 32);
    Serial.println();
    Serial.print("II[3]: ");
    i = 0;
    for (uint8_t j = 31; j < 62; j++) {
      txBuffor[i] = originalMessageBuf[j];
      Serial.print(txBuffor[i]);
      i++;
    }
    this->write(&txBuffor, 31);
    i = 0;
    memset(txBuffor, 0, 32);
    Serial.println();
    Serial.print("III[3]: ");
    for (uint8_t j = 62; j < length; j++) {
      txBuffor[i] = originalMessageBuf[j];
      Serial.print(txBuffor[i]);
      i++;
    }
    Serial.println();
    this->write(&txBuffor, 31);
    memset(txBuffor, 0, 32);
  }
  return result;
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
