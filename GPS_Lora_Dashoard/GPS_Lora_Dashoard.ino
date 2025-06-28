/*
Use version M5Unified 0.2.7
ESP32 3.2.0

*/

#include <M5Unified.h>
//#include <M5Stack.h>
#include <Wire.h>
//#//include "Free_Fonts.h"
#include <SD.h>
#include <sqlite3.h>
#define KEYBOARD_I2C_ADDR 0X08
#define KEYBOARD_INT 5
#include <M5GFX.h>
#include "image_array.h"
#include "lora.h"


#define SD_SPI_SCK_PIN 18
#define SD_SPI_MISO_PIN 19
#define SD_SPI_MOSI_PIN 23
#define SD_SPI_CS_PIN 4



M5GFX display;
M5Canvas canvas1(&display);
M5Canvas sidet(&display);
M5Canvas canvas2(&canvas1);
M5Canvas compass_mini(&canvas1);
M5Canvas grid(&canvas1);

TaskHandle_t Task1;
TaskHandle_t Task2;
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



// Distance, expressed in meters, between two points
struct offset_m {
  float x;
  float y;
  float dist;
  int azimuth;
};

struct groups_type {
  String name;
  String description;
  int group_id;
};

sqlite3 *db1;
struct gps mygps;
struct sat mysat[32];
unsigned long timing1, timing2;

long nowtime;
long time_sample = 0;
const long sample_delay = 10000;
int max_distance = 0;


// Waypoints loaded from SD
struct points waypoints[50];
struct groups_type groups[20];
struct points myposition;
int groups_t = 0;
int tmp_group = 0;
int ccccc = 0;
bool data_ready = false;
// Points from current track
const short max_points = 500;
static points track[max_points];
// Pointer to latest stored position inside the track
bool overflow_track = false;
int track_pos = -1;  ///-1

// Latest position received from GPS
struct points current;

// Latest movement
struct offset_m movement;


// Menu offsets
int menu_x;
int menu_y;
int *point_select;
int *point_max;
// Distance covered
float distance = 0;

// Switch to turn off display and block keyboard
int disp_on = 1;
// Return code from sqllite
int rc;
// Waypoint selected from the list
int wp_pos = 0;
// Number of Waypoints loaded into the array
int wp_t = 0;


// NUmber of satellite received from sentence GPGSV
int sv;
// Page displayed
int page = 0;
// Background page, used in pop-up menu
int bg_page = 0;
// Zoom array: meter per pixel
const int zoom[] = { 1, 2, 4, 10, 20, 40, 100, 200, 400, 1000, 2000, 4000, 10000, 20000 };
const int zoom_c[] = { 25, 50, 100, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000, 100000, 250000, 500000 };
const String zoom_l[] = { "25m", "50m", "100m", "250m", "500m", "1km", "2.5km", "5km", "10km", "25km", "50km", "100km", "250km", "500km" };
short preference[] = { 0, 0, 1, 1, 1 };
short preference_values[] = { 1, 0, 1, 0, 6, 0, 1, 0, 0 };
const String preference_label[] = { "Store position", "Send Wifi position", "Continuous sending" };
// Zoom selected
int c_zoom = 0;
struct offset_m display_offset;


String str = "";
// Substring parsed from full GPS sentences
String parsed;

String tmp_name;
int char_position = 0;
HardwareSerial GPSRaw(1);

void setup() {

  sv = 0;
  //mark_p = 0;
  menu_y = 0;
  menu_x = 2;
  // track =  (points*) ps_malloc( 2000 * sizeof(points));
  myposition.lat = 52.556034;
  myposition.lon = 13.425194;

  display_offset.x = 0;
  display_offset.y = 0;
  M5.begin();
  M5.Speaker.setVolume(0);
  Serial.begin(115200);
  Serial.print("Init");
  //M5.Speaker.mute();
  // Serial2.begin(9600, SERIAL_8N1, 16, 17);
  GPSRaw.begin(9600, SERIAL_8N1, 16, 17);
  //M5.Power.begin();
  // sprites
  display.begin();
  display.fillScreen(TFT_BLACK);


  display.fillScreen(TFT_BLACK);
  generate_compass();
  generate_compass_mini();
  lora_begin();
  SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
  if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
    Serial.println("SD card error!");
  }
  sqlite3_initialize();
  if (openDb("/sd/gps_track.db", &db1)) {
    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.print("setupSDCard PASS . SIZE = ");
    Serial.print(cardSize);
    Serial.println(" MB");
    // preference[0]=1;
  } else {
    Serial.print("No valid SD Card dedected");
    preference[0] = 0;
  }

  read_waypoints();
  read_position();
  read_groups();

  xTaskCreatePinnedToCore(
    LoraReceiver, /* Function to implement the task */
    "Task1",      /* Name of the task */
    10000,        /* Stack size in words */
    NULL,         /* Task input parameter */
    0,            /* Priority of the task */
    &Task1,       /* Task handle. */
    0);           /* Core where the task should run */
  xTaskCreatePinnedToCore(
    parser_task, /* Function to implement the task */
    "Task2",     /* Name of the task */
    10000,       /* Stack size in words */
    NULL,        /* Task input parameter */
    0,           /* Priority of the task */
    &Task1,      /* Task handle. */
    0);          /* Core where the task should run */
  //read_track();
}
void clear_screen() {
  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(0, 140);
}



void refresh() {
  // timing1 = micros();
  switch (page) {
    case 0:
      print_track(0, 20, 220, 200);  // ( x,y,width,height)
      print_top_bar();
      print_menu();
      print_info();  //
      //print_compass_mini();
      print_go_tbt();
      //   Serial.println(" 1");
      break;
    case 1:
      print_go_wp(0, 20);
      print_top_bar();
      print_menu();
      print_track(220, 125, 100, 100);  // ( x,y,width,height)
      print_info();
      break;
    case 2:
      // Gps Stat
      sat_circle(0, 20);
      print_top_bar();
      print_menu();
      sat_stats();
      print_info();
      break;
    case 3:
      // Position informations
      print_position(0, 20);
      print_top_bar();
      print_menu();
      print_info();
      print_go_tbt();
      break;
    case 4:
      // Peers list
      print_peers_position(0, 20);
      print_top_bar();
      print_menu();
      print_info();
      print_go_tbt();
      break;
    default:
      break;
  }
  //timing2 = micros();
  //Serial.print("Timing: ");
  //Serial.println(timing2 - timing1);
}

void loop() {
  if (mygps.fix > 0) {
    myposition.lat = current.lat;
    myposition.lon = current.lon;
  }


  if (data_ready) {

    data_ready = false;
    refresh();
  }
  int i;
  /*
        Input from core buttons and from face keyboard

  */
  M5.update();
  // M5.BtnA.pressedFor(std::uint32_t ms)
  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(200)) {
    button_top_a();
    refresh();
  } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(200)) {
    button_top_b();
    refresh();
  } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(200)) {
    button_top_c();
    refresh();
  }

  if (newdata) {
    refresh();
    add_point();
    newdata = false;
  }
  delay(50);
}
