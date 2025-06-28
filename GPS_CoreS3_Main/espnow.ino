struct data_info {
  float dist;
  float lat;
  float lon;
  float alt;
  int azimuth;
  int wp_azimuth;
  int battery;
  int sat;
  int fixed;
  int speed;
  char time[9];
  char wp_name[25] = "";
};

struct position {
  float lat;
  float lon;
  float alt;
  uint8_t device_id;
  char device_name[25] = "";
};

position position_ls;

//uint8_t broadcastAddress[] = { 0x8C, 0xAA, 0xB5, 0x83, 0x7D, 0x24 };  // TTGO Watch
//uint8_t broadcastAddress[] = { 0x4c, 0x75, 0x25, 0xc2, 0xe8, 0x04 }; //TTGO LORA
uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };  //BROADCAST
String success;
String receive_string;
data_info data_rs;
void data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
/*  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
  Serial.println(WiFi.macAddress());
  */
}

// Callback when data is received
void data_receive(const esp_now_recv_info *info, const uint8_t *data, int data_len) {
  char macStr[18];
   char tmpstr[64];
  uint8_t deviceid, waypointid;
  memcpy(&position_ls, data, sizeof(position_ls));
  Serial.print("Received remote data from: ");
  Serial.println(position_ls.device_name);
  deviceid = position_ls.device_id;
  if (deviceid < 10) {
    peerspoints[deviceid].lat = position_ls.lat;
    peerspoints[deviceid].lon = position_ls.lon;
    peerspoints[deviceid].high = position_ls.alt;
    
    peerspoints[deviceid].name = position_ls.device_name;
    Serial.print("deviceid: ");
    Serial.println(peerspoints[deviceid].id );
    if (peerspoints[deviceid].id < 1) {
      Serial.print("wp_t: ");
      Serial.println(wp_t);
     
      waypoints[wp_t].lat = position_ls.lat;
      waypoints[wp_t].lon = position_ls.lon;
      waypoints[wp_t].high = position_ls.alt;
      waypoints[wp_t].name = position_ls.device_name;
      Serial.print("Stored: ");
      Serial.println(waypoints[wp_t].name );
      waypoints[wp_t].type = 15;
     
      peerspoints[deviceid].id = wp_t;
       wp_t++;
    } else {
     waypointid = peerspoints[deviceid].id;
      waypoints[waypointid].lat = position_ls.lat;
      waypoints[waypointid].lon = position_ls.lon;
      waypoints[waypointid].high = position_ls.alt;
      waypoints[waypointid].name = position_ls.device_name;
      waypoints[waypointid].type = 15;

    }
  }
  // Serial.print("Last Packet Recv Data: ");
  //  Serial.println(*data);
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
  current_status.espnow = true;
}

void stop_espnow() {
  esp_now_deinit();
  current_status.espnow = false;
}

void send_position() {
  data_info data_ts;
  offset_m off;
  int speed;
  speed = int(movement.dist * 0.36);
  off = calc_movement(waypoints[wp_pos], current);
  data_ts.dist = off.dist;
  data_ts.lat = current.lat;
  data_ts.lon = current.lon;
  data_ts.alt = current.high;
  data_ts.battery = getBatteryLevel();
  data_ts.azimuth = movement.azimuth;
  data_ts.wp_azimuth = off.azimuth;
  data_ts.fixed = mygps.nsat_t;
  data_ts.sat = mygps.nsat;
  data_ts.speed = speed;
  //strncpy(data_ts.time, mygps.parsedtime,8)
  mygps.parsedtime.toCharArray(data_ts.time, 9);
 // Serial.print("--");
 // Serial.print(waypoints[wp_pos].name.substring(0, 20));
 // Serial.print(" | ");
 // Serial.println(waypoints[wp_pos].name.length());
  // data_ts.wp_name = String(waypoints[wp_pos].name.substring(0, 14));
  strncpy(data_ts.wp_name, waypoints[wp_pos].name.substring(0, 24).c_str(), waypoints[wp_pos].name.length());
  //data_ts.wp_name = "24-03-23 06:40:51";
  //Serial.println("sendind data");
  // Serial.print("Waypoint name: ");
  //    Serial.println(data_ts.wp_name);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&data_ts, sizeof(data_ts));
  //esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&data_ts, 100);
}