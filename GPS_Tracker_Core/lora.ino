#include "config.h"
char* Data = new char[42];

void ATCommand(char cmd[], char date[], uint32_t timeout = 1000) {
  char buf[256] = { 0 };
  if (date == NULL) {
    sprintf(buf, "AT+%s", cmd);
  } else {
    sprintf(buf, "AT+%s=%s", cmd, date);
  }
  Serial1.write(buf);
  ReceiveAT(timeout);
}


bool ReceiveAT(uint32_t timeout) {
  uint32_t nowtime = millis();
  while (millis() - nowtime < timeout) {
    if (Serial1.available() != 0) {
      String str = Serial1.readString();
      Serial.println(str);
      if (str.indexOf(" OK") != -1 || str.indexOf(" ERROR") != -1) {
        Serial.println(str);
        return true;
      } else {
        Serial.println("[!] Syntax Error");
        break;
      }
    }
  }
  Serial.println("[!] Timeout");
  return false;
}


void init_lora() {

  Serial1.begin(115200, SERIAL_8N1, 15, 13);

  ATCommand("XXX", "X");
  ATCommand("LORAWAN", "0");
  ATCommand("LORAWAN", "?");

  ATCommand("LoraSet", "868000000,22,12,0,4,16,0,0,1");
  ATCommand("AutoLPM", "0");
  ATCommand("RX", "0");
  ATCommand("RX", "?");
  ATCommand("LoraSet", "?");
  /* SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(LORA_BAND * 1E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  LoRa.setTxPower(TXPOWER);
  LoRa.setSignalBandwidth(BANDWITH);
  LoRa.setCodingRate4(CODINGRATE);
  LoRa.setSpreadingFactor(SPREADINGFACTOR);
  LoRa.setPreambleLength(PREAMBLE);
  */
}
void dump_data() {
  for (int i = 0; i < sizeof(position_ts); i++) {
    Serial.print(Data[i]);
  }
  Serial.println();
  Serial.println("+++++++++++++++");


  for (size_t i = 0; i < sizeof(position_ts); ++i) {
    if (Data[i] < 0x10) Serial.print("0");  // Add leading zero
    Serial.print(Data[i], HEX);
    Serial.print(" ");
    if ((i + 1) % 16 == 0) Serial.println();  // Optional: new line every 16 bytes
  }
  Serial.println("+++++++++++++++++");
  Serial.print("Size: ");
  Serial.println(sizeof(Data));
  Serial.println("+++++++++++++++++");
}

void send_position() {
  /*LoRa.beginPacket(); */
  // char Data[sizeof(position_ts)];
  //
  char hexBuf[2 * sizeof(position_ts) + 1];
  position_ts.lat = current.lat;
  position_ts.lon = current.lon;
  position_ts.alt = current.high;
  position_ts.device_id = DEVICEID;
  strcpy(position_ts.device_name, "Tracker2");
  uint8_t* bytes = (uint8_t*)&position_ts;
  for (size_t i = 0; i < sizeof(position_ts); i++) {
    sprintf(hexBuf + i * 2, "%02X", bytes[i]);
  }

  ATCommand("SendHex",hexBuf);

}