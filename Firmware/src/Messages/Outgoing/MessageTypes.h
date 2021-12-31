#ifndef OutgoingMessageTypes_h
#define OutgoingMessageTypes_h

enum OUTGOING_MESSAGE_TYPE {
  WELCOME = 0,
  ERROR = 1,
  INITIALIZE_DONE = 2,
  TELEMETRIC_DATA = 3,
};

enum ERROR_CODE {
  INVALID_INITIALIZE_MESSAGE = 1,
  INVALID_MESSAGE_TYPE = 2,
};

#endif