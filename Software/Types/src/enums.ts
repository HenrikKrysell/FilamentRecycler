export enum STREAMING_CONTEXT {
  CURRENT_SESSION = "CURRENT_SESSION",
  CONSOLE = "CONSOLE",
  RECORDED_SESSION = "RECORDED_SESSION",
}

export enum FROM_BACKEND_MESSAGE_TYPE {
  STATE_UPDATED = "STATE_UPDATED",
  CONSOLE_UPDATED = "CONSOLE_UPDATED",
  ERROR = "ERROR",
  ACK = "ACK",
  SEND_COMMAND = "SEND_COMMAND",
}

export enum TO_BACKEND_MESSAGE_TYPE {
  SET_RPM = "SET_RPM",
  SET_TARGET_TEMPERATURE = "SET_TARGET_TEMPERATURE",
  SHUTDOWN = "SHUTDOWN",
}
