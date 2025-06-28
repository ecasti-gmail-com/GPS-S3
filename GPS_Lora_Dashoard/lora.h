//#include <SPI.h>
#include "M5_SX127X.h"
//#include "storage.h"
// Module Connect Pins Config
#define CS_PIN 5
#define RST_PIN 13
#define IRQ_PIN 34

#define LORA_MISO 19
#define LORA_MOSI 23
#define LORA_SCLK 18

// LoRa Parameters Config
// #define LORA_FREQ     433E6
#define LORA_FREQ 868E6
#define LORA_SF 12
#define LORA_BW 125E3
#define LORA_TX_POWER 22

#define maxpeers 10

//void store_peer_position(int device_id) {};

void lora_begin() {
  LoRa.setPins(CS_PIN, RST_PIN, IRQ_PIN);  // set CS, reset, IRQ pin
  while (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init fail.");
    delay(1000);
  }

  LoRa.setTxPower(LORA_TX_POWER);
  LoRa.setSignalBandwidth(LORA_BW);
  LoRa.setSpreadingFactor(LORA_SF);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(16);
  Serial.println("LoRa init Completed");
}

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
  char wp_name[25];
};
struct position {
  float lat;
  float lon;
  float alt;
  uint8_t device_id;
  char device_name[25] = "";
};

struct lora_peer {
  float snr;
  float rssi;
  unsigned long ts;
};


struct data_info data_ts;
struct position position_ts;
struct  position peers_position[maxpeers];
struct lora_peer peers_stats[maxpeers];
struct lora_peer lora_data;

bool newdata = false;
String str2;

uint8_t peer_selected  = 1 ;
void store_peer_position(int device_id);

/*
void store_peer_position(int device_id) {
 char stringa[128];
  char tmpstr[18];
  mygps.rawdate.toCharArray(tmpstr, 18);
  sprintf(stringa, "Insert into remote_peers(timestamp,lat,lon,device_id) values (\'%s\',%f,%f,%f)", tmpstr, peers_position[peer_selected].lat, peers_position[peer_selected].lon, device_id);
  Serial.println(stringa);
  rc = db_exec(db1, stringa);
  if (rc != SQLITE_OK) {
    Serial.println("Unable to store data");
    M5.Lcd.setCursor(20, 225);
    M5.Lcd.print("Unable to store data: ");
    M5.Lcd.print(rc);
  }
}
*/

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
      memcpy(&position_ts, tmp, 40);
      int deviceid = position_ts.device_id;
      if (deviceid < maxpeers) {
            memcpy(&peers_position[deviceid],tmp,40);
            peers_stats[deviceid].snr = LoRa.packetSnr();
            peers_stats[deviceid].rssi = LoRa.packetRssi();
            peers_stats[deviceid].ts = millis();
            Serial.print("Received from: ");
            Serial.println(deviceid);
          //  store_peer_position(deviceid);
      }
       else {
        Serial.println(deviceid);
       }
      lora_data.snr = LoRa.packetSnr();
      lora_data.rssi = LoRa.packetRssi();
      lora_data.ts = millis();
      newdata = true;
      i = 0;
    }
  }
}