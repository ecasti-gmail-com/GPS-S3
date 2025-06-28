#pragma once
#define SENDINGTIME '8'
#define DEVICEID 1

#define SCK 5    // GPIO5  -- SX1278's SCK
#define MISO 19  // GPIO19 -- SX1278's MISnO
#define MOSI 27  // GPIO27 -- SX1278's MOSI
#define SS 18    // GPIO18 -- SX1278's CS
#define RST 23   // GPIO14 -- SX1278's RESET
#define DI0 26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND 868E6


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

uint8_t broadcastAddress[] = { 0x48, 0x27, 0xE2, 0x66, 0xF1, 0x44 };  // MAC CORTO