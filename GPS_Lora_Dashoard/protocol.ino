/*
  Lenght
  destination 1 byte
  type

  struct pachetto {
  byte[2] destination;
  byte[2] sender;
  byte[2] age
  byte[1] retrasmission;
  byte type ; ( position | wp | msg | query | emergency )
  byte flags (
  0 encrypted
  1 forward
  2 query
  3 ack



  )
  bytes[10-100] payload
  byte checksum




  }
