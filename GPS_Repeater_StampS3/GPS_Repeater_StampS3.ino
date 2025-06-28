#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>
#include "config.h"



TaskHandle_t Task1;
unsigned int counter = 0;

unsigned long sendTime = 0;
unsigned long last_message = 3600;
String rssi = "RSSI --";
String packSize = "--";
String packet;
esp_now_peer_info_t peerInfo;
float batt1 = 0;
float gpsbatt = 0;
bool lorasending = false;
bool loradata = false;
bool lorasendready = false;
void setup() {

  // disableCore0WDT(); // Disable for core 0
  // disableCore1WDT(); // Disable for core 1 (if needed)


  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("------------------------------------");
  Serial.println();
  Serial.println("LoRa Sender Test");

  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  LoRa.setTxPower(22);
  LoRa.setSignalBandwidth(128E3);
  LoRa.setCodingRate4(8);
  LoRa.setSpreadingFactor(12);
  LoRa.setPreambleLength(16);
  // LoRa.onReceive(cbk);
  // LoRa.receive();
  Serial.println("init ok");
  //  LoRa.receive();

  delay(1500);

  my_esp_now_init();
  xTaskCreatePinnedToCore(
    LoraReceiver, /* Function to implement the task */
    "Task1",      /* Name of the task */
    10000,        /* Stack size in words */
    NULL,         /* Task input parameter */
    10,           /* Priority of the task */
    &Task1,       /* Task handle. */
    0);           /* Core where the task should run */
}

void loop() {

  if (loradata) {
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&position_ls, sizeof(position_ls));
    loradata = false;
    Serial.println(" ++++++++++++ Received from lora +++++++++++++");
  }
  if (lorasendready) {
    LoRa.beginPacket();
    LoRa.write((uint8_t *)&position_ts, sizeof(position_ts));
    LoRa.endPacket();
    lorasendready = false;
    Serial.println("######   Sending to Lora ####################");
   // LoRa.receive();
  }
//  delay(20);
}
