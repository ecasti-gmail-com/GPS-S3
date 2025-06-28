
#include "config.h"





String success;
String receive_string;

void data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  // if (status == 0) {
  //   success = "Delivery Success :)";
  // } else {
  //   success = "Delivery Fail :(";
  // }
  // Serial.println(WiFi.macAddress());
}

// Callback when data is received
void data_receive(const esp_now_recv_info *info, const uint8_t *data, int data_len) {

  char macStr[18];
  uint8_t *mac = info->src_addr;
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  sendTime = millis();
  memcpy(&data_ts, data, sizeof(data_ts));
  Serial.print("Last Packet Recv from: ");
  Serial.println(macStr);
  Serial.print("Last Packet Recv Size: ");
  Serial.println(data_len);
  // Serial.print("Last Packet Recv Data: ");
  //  Serial.println(*data);

  Serial.println(data_ts.time[7]);
  if (data_ts.time[7] == SENDINGTIME) {
    lorasending = true;
    Serial.println("Preparing to send Lora");
    // LoRa.print(data_ts.wp_name);
    position_ts.lat = data_ts.lat;
    position_ts.lon = data_ts.lon;
    position_ts.alt = data_ts.alt;
    position_ts.device_id = 1;
    strcpy(position_ts.device_name, "CoreS3");
    lorasendready = true;

  } else {
 /*
    Serial.print("second: ");
    Serial.println(data_ts.time);
    Serial.print("WP Name: ");
    Serial.println(data_ts.wp_name);
    Serial.print("Control: ");
    Serial.print(data_ts.time[7]);
    Serial.print(" | : ");
    Serial.println(&data_ts.time[7]);
    */
  }
  lorasending = false;
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



void LoraReceiver(void *) {
  char tmp[40];
  int i = 0;
  for (;;) {
    if (!lorasending) {
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
        loradata = true;
        // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&position_ls, sizeof(position_ls));
        i = 0;
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
