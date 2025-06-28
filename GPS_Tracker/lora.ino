#include "config.h"

void init_lora() {
  SPI.begin(SCK, MISO, MOSI, SS);
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
}


void send_position() {
  LoRa.beginPacket();
  position_ts.lat = current.lat;
  position_ts.lon = current.lon;
  position_ts.alt = current.high;
  position_ts.device_id = DEVICEID;
  strcpy(position_ts.device_name, "Tracker1");


  LoRa.write((uint8_t *)&position_ts, sizeof(position_ts));
  LoRa.endPacket();
  //LoRa.receive();
    Serial.print("-----");
  Serial.print(position_ts.lat);
  Serial.print("-----");
  Serial.print(position_ts.lon);
  Serial.print("-----");
  Serial.println(position_ts.alt);
}