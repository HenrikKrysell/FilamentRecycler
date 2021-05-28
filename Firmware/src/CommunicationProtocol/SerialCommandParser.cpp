#include "SerialCommandParser.h"

SerialCommandParser::SerialCommandParser()
{
  _bufferIndex = 0;
  _discardCurrentMessage = false;
}

SerialCommandParser::~SerialCommandParser()
{
}

Message* SerialCommandParser::readIfDataPresent()
{
  int numAvailable = Serial.available();
  if (numAvailable > 0)
  {
    _buffer[_bufferIndex] = Serial.read();
    
    if (_buffer[_bufferIndex] == '\r' || _buffer[_bufferIndex] == '\n')
    {
      if (_discardCurrentMessage) {
        _bufferIndex = 0;
        return NULL;
      }

      Message *msg = new Message();
      int index = 0;
      Parameter cmdParam = parseParameter(index);
      msg->type = (MessageType)cmdParam.name;
      msg->subType = cmdParam.longValue;
      msg->id = -1;

      msg->numParams = 0;
      while (msg->numParams < MAX_PARAMETERS && index < _bufferIndex)
      {
        if (_buffer[index] == ' ')
        {
          index++;
          continue;
        }

        Parameter param = parseParameter(index);
        if (param.name < 'A' || param.name > 'Z')
        {
          SerialSendError(ERROR_INVALID_PARAMETER_NAME);
          delete msg;
          _bufferIndex = 0;
          return NULL;
        }

        if (param.name == ID_PARAMETER_PREFIX) {
          msg->id = param.longValue;
          continue;
        }

        msg->parameters[msg->numParams++] = param;
      }
      
      _bufferIndex = 0;
      return msg;
    }
    else
    {
      _bufferIndex++;

      if (_bufferIndex > _bufferSize)
      {
        SerialSendError(ERROR_COMMAND_TOO_LONG);
        _discardCurrentMessage = true;
        _bufferIndex = 0;
      }
    }
  }

  return NULL;
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
  long longValue = 0;
  while (index < _bufferIndex)
  {
    char currentChar = _buffer[index];
    if (currentChar < '0' || currentChar > '9')
      break;

    floatValue = floatValue * 10.0f + (currentChar - '0');
    longValue = longValue * 10 + (currentChar - '0');
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
  param.longValue = negativeValue ? -longValue : longValue;
  return param;
}