export enum STREAMING_CONTEXT {
  CURRENT_SESSION = "CURRENT_SESSION",
  CONSOLE = "CONSOLE",
  RECORDED_SESSION = "RECORDED_SESSION",
}

export enum MESSAGE_TYPE {
  STATE_UPDATED = "STATE_UPDATED",
  CONSOLE_UPDATED = "CONSOLE_UPDATED",
  SET_RPM = "SET_RPM",
  ERROR = "ERROR",
  ACK = "ACK",
  SEND_COMMAND = "SEND_COMMAND",
}
