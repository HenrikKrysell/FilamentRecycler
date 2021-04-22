#ifndef SerialCommandParser_h
#define SerialCommandParser_h

#include <Arduino.h>
#include "DataStructures/Command.h"

class SerialCommandParser
{
  public:
    SerialCommandParser();
    ~SerialCommandParser();
    
    Command readIfDataPresent();

  private:
  const static char _commandStartChar = 'C';
  const static int _bufferSize = 64;
  const static int _numberBufferSize = 20;

  Parameter parseParameter(int& index);

  char _buffer[_bufferSize];
  int _bufferIndex;
};

#endif