#ifndef SerialCommandParser_h
#define SerialCommandParser_h

#include <Arduino.h>
#include "Incomming/IncommingMessage.h"
#include "Outgoing/Errors.h"

class SerialCommandParser
{
  public:
    SerialCommandParser();
    ~SerialCommandParser();

    IncommingMessage* readIfDataPresent();

  private:
    const static int _bufferSize = 256;
    const static int _numberBufferSize = 20;

    Parameter parseParameter(int &index);

    char _buffer[_bufferSize];
    int _bufferIndex;
    bool _discardCurrentMessage;
};

#endif