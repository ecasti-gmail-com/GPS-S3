#pragma once
#define SENDINGTIME '7'
#define DEVICEID 1

#define SCK 5    // GPIO5  -- SX1278's SCK
#define MISO 9  // GPIO19 -- SX1278's MISnO
#define MOSI 7  // GPIO27 -- SX1278's MOSI
#define SS 3    // GPIO18 -- SX1278's CS
#define RST 1   // GPIO14 -- SX1278's RESET
#define DI0 10   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND 868E6


#define maxpeers 10
// 48:27:E2:66:F1:44
uint8_t broadcastAddress[] = { 0x8C, 0xAA, 0xB5, 0x83, 0x7D, 0x24 };  // MAC CORTO


// communication

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

struct lora_peer {
  float snr;
  float rssi;
  unsigned long ts;
};

data_info data_ts;
position position_ts;
position position_ls;
struct lora_peer lora_data;