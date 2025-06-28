#include "config.h"




String success;
String receive_string;

void data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
  Serial.println(WiFi.macAddress());
}

void lorasend() {
  LoRa.beginPacket();
  LoRa.write((uint8_t *)&position_ts, sizeof(position_ts));
  LoRa.endPacket();
}

void hexDump(const void* data, size_t size) {
  const byte* byteData = (const byte*)data;
  for (size_t i = 0; i < size; i++) {
    if (i % 16 == 0) {
      Serial.println();
      Serial.print("0x");
      Serial.print((unsigned int)i, HEX);
      Serial.print(": ");
    }

    if (byteData[i] < 0x10) Serial.print("0"); // Leading zero
    Serial.print(byteData[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// Callback when data is received
void data_receive(const esp_now_recv_info *info, const uint8_t *data, int data_len) {
  char macStr[18];
  memcpy(&data_ts, data, sizeof(data_ts));
  // Serial.print("Last Packet Recv Data: ");
  //  Serial.println(*data);
  if (data_ts.time[7] == SENDINGTIME) {
    sendTime = millis();
    position_ts.lat = data_ts.lat;
    position_ts.lon = data_ts.lon;
    position_ts.alt = data_ts.alt;
    position_ts.device_id = 1;
    strcpy(position_ts.device_name, "CoreS3");
    lorasendready = true;
  }
}

void my_esp_now_init() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(data_sent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(data_receive);
}

void stop_espnow() {
  esp_now_deinit();
}

void send_position() {
  data_info data_ts;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&data_ts, 100);
}

void get_battery() {
  VBAT = (float)(analogRead(vbatPin)) / 4095 * 2 * 3.3 * 1.1;
  batt1 = (VBAT - 3.5) * 34;
  if (batt1 < 0) { batt1 = 0; }
  if (batt1 > 20) { batt1 = 20; };
  if (sendTime > 0) {
    last_message = (millis() - sendTime) / 1000;
  }
  if (last_message < 600) {
    gpsbatt = data_ts.battery / 5;
  } else {
    gpsbatt = 0;
  }
};




void LoraReceiver(void *) {
  char tmp[40];
  int i = 0;
  for (;;) {
      int packetSize = LoRa.parsePacket();

      if (packetSize) {

        Serial.println();
        Serial.print("Packet size: ");
        Serial.println(packetSize);

        while (LoRa.available()) {
          //Serial.print((char)LoRa.read());
          tmp[i++] = (char)LoRa.read();
        }
        memcpy(&position_ls, tmp, 40);
        int deviceid = position_ls.device_id;
        Serial.print("Received Lora from: ");
        Serial.println(deviceid);

        lora_data.snr = LoRa.packetSnr();
        lora_data.rssi = LoRa.packetRssi();
        lora_data.ts = millis();
        lorareceivedready = true;
        // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&position_ls, sizeof(position_ls));
        i = 0;
      }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
