#include <SPI.h>
#include <Wire.h>
///#include <SSD1306.h>
//#include <LoRa.h>
#include <Arduino.h>
#include "config.h"
#include <M5Unified.h>

#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16

#define SCK 5    // GPIO5  -- SX1278's SCK
#define MISO 19  // GPIO19 -- SX1278's MISO
#define MOSI 27  // GPIO27 -- SX1278's MOSI
#define SS 18    // GPIO18 -- SX1278's CS
#define RST 14   // GPIO14 -- SX1278's RESET
#define DI0 26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
TaskHandle_t Task1;
//SSD1306 display(0x3c, OLED_SDA, OLED_SCL);

// Store points parts of track
struct points {
  float lat;
  float lon;
  float high;
  int type;
  int groups;
  String name;
  int id;
};



// Store last position
struct gps {
  int fix;
  float lat;
  float lon;
  float high;
  int nsat;
  int nsat_t;
  String fixtime;
  String parsedtime;
  String rawdate;
  float distance;
};

// Store satellites info
struct sat {
  int sat_n;
  int elevation;
  int azimuth;
  int snr;
};

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
  char wp_name[25] = "";
};

struct position {
  float lat;
  float lon;
  float alt;
  uint8_t device_id;
  char device_name[25] = "track1";
};

// Latest position received from GPS
struct points current;
position position_ts;

String str = "";
// Substring parsed from full GPS sentences
String parsed;
HardwareSerial GPSRaw(2);

struct gps mygps;
// NUmber of satellite received from sentence GPGSV
int sv;
struct sat mysat[32];
// New data from the GPS parser available
bool data_ready = false;




// Forward declarations
void displayLoraData(int packetSize, String packet, String rssi);
void showLogo();

void setup() {
  Serial.begin(115200);
  GPSRaw.begin(9600, SERIAL_8N1, 16, 17);
  while (!Serial)
    ;
  Serial.println();
  Serial.println("LoRa Receiver");

  // Configure the LoRA radio
  init_lora();
  Serial.println("init ok");


  xTaskCreatePinnedToCore(
    parser_task, /* Function to implement the task */
    "Task1",     /* Name of the task */
    10000,       /* Stack size in words */
    NULL,        /* Task input parameter */
    0,           /* Priority of the task */
    &Task1,      /* Task handle. */
    0);          /* Core where the task should run */
}

void loop() {

  String nsats;
  String nsatf;

  delay(10);
  if (data_ready) {
    nsats = String(mygps.nsat);
    nsatf = String(mygps.nsat_t);
    Serial.println("N " + nsatf + "/" + nsats + "  sats |  " + mygps.parsedtime);
    //send_position();

    data_ready = false;
    if (mygps.parsedtime.substring(7, 8) == SENDINGTIME) {
    
      send_position();
      Serial.println("sending...");
    } else {
    //  Serial.println("second: " + mygps.parsedtime.substring(7, 8));

    }
  }
}

