
uint8_t broadcastAddress[] = { 0x98, 0xF4, 0xAB, 0x6C, 0xE5, 0x58 };
String success;
String poba = "Poba";
String receive_poba;
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

// Callback when data is received
void data_receive(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&data_ts, incomingData, sizeof(data_ts));
 /* Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println(data_ts.dist);
  Serial.println(data_ts.azimuth);
  Serial.println(data_ts.wp_azimuth);
  Serial.println(data_ts.battery);
  Serial.println(data_ts.fixed);
  Serial.println(data_ts.sat);
  Serial.println(data_ts.speed);*/
  Serial.println(data_ts.wp_name);
  
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

void send_position() {
  Serial.println("sendind data");
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&data_ts, sizeof(data_ts));
}
