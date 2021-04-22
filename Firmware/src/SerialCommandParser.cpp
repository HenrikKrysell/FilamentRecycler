#include "SerialCommandParser.h"

SerialCommandParser::SerialCommandParser()
{
  _bufferIndex = 0;
}

SerialCommandParser::~SerialCommandParser()
{
}

Command SerialCommandParser::readIfDataPresent()
{
  Command cmd;
  cmd.type = CommandType::Nothing;

  int numAvailable = Serial.available();
  if (numAvailable > 0)
  {
    _buffer[_bufferIndex] = Serial.read();
    
    if (_buffer[0] != _commandStartChar)
    {
      _bufferIndex = 0;
    }
    else if (_buffer[_bufferIndex] == '\r' || _buffer[_bufferIndex] == '\n')
    {
      int index = 0;
      Parameter cmdParam = parseParameter(index);
      cmd.type = (CommandType)cmdParam.intValue;

      cmd.numParams = 0;
      while (cmd.numParams < MAX_PARAMETERS && index < _bufferIndex)
      {
        if (_buffer[index] == ' ')
        {
          index++;
          continue;
        }

        Parameter param = parseParameter(index);
        if (param.name < 'A' || param.name > 'Z')
        {
          Serial.println("ERROR: Invalid parameter name");
          cmd.type = CommandType::Nothing;
          _bufferIndex = 0;
          break;
        }
        cmd.parameters[cmd.numParams++] = param;
      }
      
      _bufferIndex = 0;
    }
    else
    {
      _bufferIndex++;

      if (_bufferIndex > _bufferSize)
      {
        Serial.println("ERROR: command too long, discarding command");
        _bufferIndex = 0;
      }
    }
  }

  return cmd;
}

Parameter SerialCommandParser::parseParameter(int& index)
{
  char name = _buffer[index++];

  bool negativeValue = false;
  if (_buffer[index] == '-')
  {
    negativeValue = true;
    index++;
  }

  float floatValue = 0.0f;
  int intValue = 0;
  while (index < _bufferIndex)
  {
    char currentChar = _buffer[index];
    if (currentChar < '0' || currentChar > '9')
      break;

    floatValue = floatValue * 10.0f + (currentChar - '0');
    intValue = intValue * 10 + (currentChar - '0');
    index++;
  }
  if (_buffer[index] == '.')
  {
    index++;

    // decimals
    float decimalPosition = 0.1f;
    while (index < _bufferIndex)
    {
      char currentChar = _buffer[index];
      if (currentChar < '0' || currentChar > '9')
        break;

      floatValue = floatValue + (currentChar - '0') * decimalPosition;
      decimalPosition *= 0.1;
      index++;
    }
  }

  Parameter param;
  param.name = name;
  param.floatValue = negativeValue ? -floatValue : floatValue;
  param.intValue = negativeValue ? -intValue : intValue;
  return param;
}