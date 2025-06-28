#include <M5CoreS3.h>
#include <Arduino.h>
#include <Wire.h>

#include <SPI.h>
#include <SD.h>
#include <sqlite3.h>
#define KEYBOARD_I2C_ADDR 0X08
#define KEYBOARD_INT 7
//#define KEYBOARD_GAMEBOY
#define KEYBOARD_CUSTOM
#include <M5GFX.h>
#include "image_array.h"
#include <WiFiClient.h>
#include <WebServer.h>

//#include "Free_Fonts.h"  // Include the header file attached to this sketch

#include <esp_now.h>
#include <WiFi.h>

M5Canvas canvas1(&CoreS3.Display);
M5Canvas sidet(&CoreS3.Display);
M5Canvas canvas2(&canvas1);
M5Canvas compass_mini(&canvas1);
M5Canvas grid(&canvas1);

#define SD_SPI_SCK_PIN 36
#define SD_SPI_MISO_PIN 35
#define SD_SPI_MOSI_PIN 37
#define SD_SPI_CS_PIN 4
#define num_pref sizeof(preference_values) / sizeof(short)
TaskHandle_t Task1;
struct preference_field {
  int min;
  int max;
  int default_value;
  String label;
};
// variables
const int zoom[] = { 1, 2, 4, 10, 20, 40, 100, 200, 400, 1000, 2000, 4000, 10000, 20000 };
const int zoom_c[] = { 25, 50, 100, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000, 100000, 250000, 500000 };
const String zoom_l[] = { "25m", "50m", "100m", "250m", "500m", "1km", "2.5km", "5km", "10km", "25km", "50km", "100km", "250km", "500km" };
short preference_values[] = { 1, 0, 1, 0, 6, 0, 1, 0, 0 };
//const String preference_label[] = { "Store position", "Send Wifi position", "Add points to track", "Enable DB WebClient", "Enable WebMap", "Debug Logs" };
//

preference_field preferences[] = {
  { 0, 1, 1, "Store position" },
  { 0, 1, 0, "Send Wifi position" },
  { 0, 1, 1, "Add points to track" },
  { 0, 1, 0, "Enable WebServer" },
  { 1, 32000, 1, "Current trackId" },
  { 0, 1, 0, "Debug logs" },
  { 0, 1, 1, "Increase trackid on startup" },
  { 0, 32000, 0, "Turn-by-Turn on TrackId" },
  { 0, 1, 0, "Turn-by-Turn reverse order" },
};


const char* wp_type[] = { "Home", "Food", "POI", "Transport", "Hotel", "Office", "Shopping", "Petrol Pump", "Traffic Signal", "School","","","","","","Peer" };
const char* points_page[] = { "Marked Points", "Waypoints" };
const char* menu_a[] = { "Track", "Altitude", "Position Info", "Position Info", "Rename WP", "-", "Confirm", "Confirm", "Confirm", "Back", "-" };
const char* menu_b[] = { "Settings", "Zoom in", "Settings", "-", "Move WP", "-", "Delete", "-", "-", "Zoom in", "-" };
const char* menu_c[] = { "Compass", "Zoom out", "Sat info", "Sat Info", "Delete WP", "-", "Cancel", "Cancel", "Cancel", "Zoom out", "-" };
const char* Pages[] = { "Stat infos", "Track", "Compass", "List WP", "Print Position", "Rename WP", "Delete WP", "List Groups", "Altitute", "Preference" };

struct status {
  bool espnow;
  bool sqlconsole;
};
struct status current_status;
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
WebServer server(80);
sqlite3* db1;
sqlite3_stmt* res;
int rec_count = 0;
const char* tail;
char current_db[255];
struct gps mygps;
struct sat mysat[32];
esp_now_peer_info_t peerInfo;
unsigned long timing1, timing2, timing3, timing4;

// Waypoints loaded from SD
struct points waypoints[50];
struct points peerspoints[10];
// Groups of waypoints, loaded from SD
struct groups_type groups[20];

int groups_t = 0;
int tmp_group = 0;
// New data from the GPS parser available
bool data_ready = false;
// number of points to keep in memory, suitable from 12H
const int maxpoints = 4400;
// Contains in memory the current track
static points track[maxpoints];
// Pointer to latest stored position inside the track
bool overflow_track = false;
int track_pos = -1;  ///-1

// Latest position received from GPS
struct points current;

// Latest movement
struct offset_m movement;

// Position on last 10 seconds
// Used to get more precision on movement
struct points lasts[10];

struct offset_m wp_distance[50];

// sortered pointer to the waypoints
short wp_distance_p[50];
//track distance
//struct offset_m track_distance[maxpoints];
//struct offset_m* track_distance;
struct offset_m* distance_tbt;
struct points* track_tbt;
// sortered pointer to the waypoints
// track_distance_p
short points_tbt[maxpoints];
bool loaded_tbt = false;
int tbt_track_pos = -1;

// Menu offsets
int menu_x;
int menu_y;
int pref_x = 0;
int pref_y = 0;
int* point_select;
int* point_max;
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
  distance_tbt = (offset_m*)malloc(maxpoints * sizeof(offset_m));
  assert(distance_tbt != NULL);
  track_tbt = (points*)malloc(maxpoints * sizeof(points));
  assert(track_tbt != NULL);
  sv = 0;
  //mark_p = 0;
  menu_y = 0;
  menu_x = 0;
  // track =  (points*) ps_malloc( 2000 * sizeof(points));

  display_offset.x = 0;
  display_offset.y = 0;

  auto cfg = M5.config();
  CoreS3.begin(cfg);

  CoreS3.Power.begin();
  CoreS3.Power.setBatteryCharge(true);
 // CoreS3.In_I2C.begin(2,1);
  Wire.end();
  Wire.begin(2, 1);
  Serial.begin(115200);

  /*Serial.println("Init");
  Serial.println(CoreS3.Power.getBatteryVoltage());

  Serial.println("Display Init");
  delay(1000);
  Serial.println("Preparing for Serial Init");
*/
  GPSRaw.begin(9600, SERIAL_8N1, 18, 17);
  //Serial.println("GPSRAW Init");

  pinMode(KEYBOARD_INT, INPUT_PULLUP);

  // Serial.println("WIRE Init");

  generate_compass();
  generate_compass_mini();
  if (preference_values[1] == 1) {
    my_esp_now_init();
  }

  // sqlite_db_init();

  SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
  if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
    Serial.println("SD card error!");
  }
  sqlite3_initialize();
  if (openDb("/sd/gps_track.db", &db1)) {
    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    // Serial.print("setupSDCard PASS . SIZE = ");
    // Serial.print(cardSize);
    // Serial.println(" MB");
    // preference[0]=1;
  } else {
    Serial.println("No valid SD Card dedected");
    preference_values[0] = 0;
  }
  read_preferences();
  read_waypoints();
  read_position();
  read_groups();
  if (preference_values[6] == 1) {
    preference_values[4]++;
    store_preferences();
  }
  read_track(preference_values[4]);


  /// Serial.println("Init Completed");
  refresh();
  // delay(1000);
  // Init task
  xTaskCreatePinnedToCore(
    parser_task, /* Function to implement the task */
    "Task1",     /* Name of the task */
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
      // Gps Stat
      sat_circle(0, 20);
      print_top_bar();
      print_menu();
      sat_stats();
      print_info();
      break;
    case 1:
      // TBT part
      if (preference_values[7] > 0) {
        if (loaded_tbt == false) {
          read_tbt(preference_values[7]);
          loaded_tbt = true;
        };
        calc_distance_track();
        sort_distance_track();
        print_tbt_info();
        print_go_tbt();
      } else {
        print_info();  //
        print_compass_mini();
      }

      //track
      print_track(0, 20, 220, 200);  // ( x,y,width,height)
      print_top_bar();
      print_menu();

      break;
    case 2:
      // Compass
      print_compass(0, 20);
      print_top_bar();
      print_menu();
      print_info();
      print_track(220, 125, 100, 100);  // ( x,y,width,height)
      break;
    case 3:
      print_go_wp(0, 20);
      print_top_bar();
      print_menu();
      print_track(220, 125, 100, 100);  // ( x,y,width,height)
      print_info();
      break;
    case 4:
      //list_wp();
      list_points();
      print_menu();

      break;
    case 5:
      // Position informations
      print_position(0, 20);
      print_top_bar();
      print_menu();
      print_info();
      break;
    case 6:
      rename_wp();
      print_menu();
      break;
    case 7:
      delete_wp();
      print_menu();
      break;
    case 8:
      list_groups();
      print_menu();
      break;
    case 9:
      // Altitube
      print_altitude(0, 20, 220, 200);
      print_top_bar();
      print_menu();
      print_info();
      print_track(220, 125, 100, 100);  // ( x,y,width,height)
      break;
    case 10:
      // Preference
      settings();
      print_menu();
      break;
    case 11:
      // wp distance

      calc_distance();

      sort_distance();

      print_top_bar();
      print_sort_wp();
      print_menu();
      //timing3 = micros();
      //  calc_distance_track();
      //sort_distance_track();
      //timing4 = micros();
      //  Serial.print(track_distance_p[3]);
      //  Serial.print(" --- ");
      //  Serial.println(timing4 - timing3);

      break;
    case 12:
      if ((loaded_tbt == false) && (preference_values[7] > 0)) {
        read_tbt(preference_values[7]);
        loaded_tbt = true;
      };
      break;
    default:

      break;
  }
  //  timing2 = micros();
}
static m5::touch_state_t prev_state;
void loop() {

  int i;
  if (disp_on) {
    CoreS3.update();
    auto t = CoreS3.Touch.getDetail();
    //  if (t.state > 0) {
    //    Serial.print("Touch =");
    //    Serial.println(t.state);
    //  }
    if ((t.state == 2) || (t.state == 6)) {
      if (t.x < 100) {
        button_top_a();
      } else if (t.x < 220) {
        button_top_b();
      } else {
        button_top_c();
      }
      //   Serial.println("Touch");
      refresh();
    }
  }

  if (digitalRead(KEYBOARD_INT) == LOW) {
    get_data();
    refresh();
  }
  if (data_ready) {
    //  send_position();
    if (preference_values[1] == 1) {
      if (!current_status.espnow) {
        my_esp_now_init();
      }
      send_position();
    } else {
      if (current_status.espnow) {
        // Stop ESPNOW
        stop_espnow();
      }
    }
    // web console
    if (preference_values[3] == 1) {
      if (!current_status.sqlconsole) {
        sqlconsole_init();
      }
      sqlconsole_handle();
    } else {
      if (current_status.sqlconsole) {
        // Stop ESPNOW
        sqlconsole_stop();
      }
    }

    if ((mygps.fixtime.endsWith("0")) and (mygps.fix == 1)) {
      if (preference_values[2] == 1) {
        store_position();
      }
      if (preference_values[0] == 1) {
        log_position();
      }
    }
    if (mygps.fix > 0) {

      check_movement();
    }
    if (disp_on) {
      if ((page < 6) || (page > 8)) {
        refresh();
      }
    }
    data_ready = false;
  }
  delay(25);
}
