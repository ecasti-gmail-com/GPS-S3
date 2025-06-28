// An Arduino based framework for the Lilygo T-Watch 2020
// Much of the code is based on the sample apps for the
// T-watch that were written and copyrighted by Lewis He.
//(Copyright (c) 2019 lewis he)



// https://www.instructables.com/id/Lilygo-T-Watch-2020-Arduino-Framework/
// By DanGeiger

#include <BleMouse.h>
#include "config.h"
#include <soc/rtc.h>
#include <esp_now.h>
#include <WiFi.h>


bool ble_enabled = false;
bool ble_exit = false;
BleMouse bleMouse("TTGO BT Mouse", "LilyGO");
TTGOClass *ttgo;
AXP20X_Class *power;
//watch = TTGOClass::getWatch();

char buf[128];

bool touch_down = false;
bool moved = true;

int16_t x, y;
int16_t xx = 120, yy = 120;



TFT_eSprite *spr = NULL;  // Sprite for meter reading
TFT_eSprite *spr2 = NULL;
TFT_eSprite *top_bar = NULL;
TFT_eSPI *tft = NULL;
esp_now_peer_info_t peerInfo;
uint32_t targetTime = 0;  // for next 1 second display update
// uint32_t clockUpTime = 0;      // track the time the clock is displayed

struct data_info {
  float dist;
  int azimuth;
  int wp_azimuth;
  int battery;
  int sat;
  int fixed;
  int speed;
  // Speed

  // trip

  String wp_name;
};

data_info data_ts;


uint8_t hh, mm, ss, mmonth, dday;  // H, M, S variables
uint16_t yyear;                    // Year is 16 bit int
uint16_t palette[4];
uint16_t palette_m[4];

int azimuth;
// The basic Time Display GUI
// if you are just updating the colon, fullUpdate =0
// if you want to update the complete display, fullUpdate =1
// This helps reduce flicker

#include <time.h>

int getTnum();
void prtTime(byte digit);
void setMenuDisplay(int mSel);

byte xcolon = 0;  // location of the colon

void displayTime(boolean fullUpdate) {

  byte xpos = 40;  // Stating position for the display
  byte ypos = 90;

  // Get the current data
  RTC_Date tnow = ttgo->rtc->getDateTime();

  hh = tnow.hour;
  mm = tnow.minute;
  ss = tnow.second;
  dday = tnow.day;
  mmonth = tnow.month;
  yyear = tnow.year;

  ttgo->tft->setTextSize(1);

  if (fullUpdate) {
    // Font 7 is a 7-seg display but only contains
    // characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .

    ttgo->tft->setTextColor(0x39C4, TFT_BLACK);  // Set desired color
    ttgo->tft->drawString("88:88", xpos, ypos, 7);
    ttgo->tft->setTextColor(0xFBE0, TFT_BLACK);  // Orange

    if (hh < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, 7);
    xpos += ttgo->tft->drawNumber(hh, xpos, ypos, 7);
    xcolon = xpos + 3;
    xpos += ttgo->tft->drawChar(':', xcolon, ypos, 7);
    if (mm < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, 7);
    ttgo->tft->drawNumber(mm, xpos, ypos, 7);
  }

  if (ss % 2) {  // Toggle the colon every second
    ttgo->tft->setTextColor(0x39C4, TFT_BLACK);
    xpos += ttgo->tft->drawChar(':', xcolon, ypos, 7);
    ttgo->tft->setTextColor(0xFBE0, TFT_BLACK);
  } else {
    ttgo->tft->drawChar(':', xcolon, ypos, 7);
  }
  ttgo->tft->setTextSize(3);
  ttgo->tft->setCursor(10, 210);

  ttgo->tft->print(mmonth);
  ttgo->tft->print("/");
  ttgo->tft->print(dday);
  ttgo->tft->print("/");
  ttgo->tft->print(yyear);
}


// Show the accelerometer working


//
// Make the following updates:
// 1) Update maxApp to the total number of apps.
// 2) Update appName to add the title of the app.
// 3) in the main routine in TWatch_framework, add a case to the switch statement to call your app routine.


const int maxApp = 7;                                                                               // number of apps
String appName[maxApp] = { "Clock", "Compass", "Accel", "Battery", "Touch", "Set Time", "Mouse" };  // app names

uint8_t modeMenu() {
  int mSelect = 0;  // The currently highlighted app
  int16_t x, y;

  boolean exitMenu = false;  // used to stay in the menu until user selects app

  setMenuDisplay(0);  // display the list of Apps

  while (!exitMenu) {
    if (ttgo->getTouch(x, y)) {  // If you have touched something...

      while (ttgo->getTouch(x, y)) {}  // wait until you stop touching

      if (y >= 160) {  // you want the menu list shifted up
        mSelect += 1;
        if (mSelect == maxApp) mSelect = 0;
        setMenuDisplay(mSelect);
      }

      if (y <= 80) {  // you want the menu list shifted down
        mSelect -= 1;
        if (mSelect < 0) mSelect = maxApp - 1;
        setMenuDisplay(mSelect);
      }
      if (y > 80 && y < 160) {  // You selected the middle
        exitMenu = true;
      }
    }
  }
  //Return with mSelect containing the desired mode
  ttgo->tft->fillScreen(TFT_BLACK);
  return mSelect;
}

void setMenuDisplay(int mSel) {

  int curSel = 0;
  // Display mode header
  ttgo->tft->fillScreen(TFT_BLUE);
  ttgo->tft->fillRect(0, 80, 239, 80, TFT_BLACK);

  // Display apps
  if (mSel == 0) curSel = maxApp - 1;
  else curSel = mSel - 1;

  ttgo->tft->setTextSize(2);
  ttgo->tft->setTextColor(TFT_GREEN);
  ttgo->tft->setCursor(50, 30);
  ttgo->tft->println(appName[curSel]);

  ttgo->tft->setTextSize(3);
  ttgo->tft->setTextColor(TFT_RED);
  ttgo->tft->setCursor(40, 110);
  ttgo->tft->println(appName[mSel]);

  if (mSel == maxApp - 1) curSel = 0;
  else curSel = mSel + 1;

  ttgo->tft->setTextSize(2);
  ttgo->tft->setTextColor(TFT_GREEN);
  ttgo->tft->setCursor(50, 190);
  ttgo->tft->print(appName[curSel]);
}

// Check out the touch screen
// Will display the x and y coordinates of where you touch
// for 10 seconds and then return to time

// Set the time - no error checking, you might want to add it

// prtTime will display the current selected time and highlight
// the current digit to be updated in yellow



// getTnum takes care of translating where we pressed into
// a number that was pressed. Returns -1 for no press
// and 13 for DONE



// Display Jupiters 4 moons
// It uses the current date time and corrects to get UTC time
// Make sure you set the correct Time Zone below

#define APP_TIME_ZONE -4  // I am East Coast in Daylight Savings


void compass() {
  int16_t xx, yy;

  int x, y, x1, y1, x2, y2, dx, dy;
  int old_az, old_wp_az;
  float old_dist;
  setCpuFrequencyMhz(240);
  my_esp_now_init();
  RTC_Date tnow = ttgo->rtc->getDateTime();
  int to_azimut = 0;
  palette[0] = TFT_BLACK;
  palette[1] = TFT_WHITE;
  palette[2] = TFT_RED;
  palette[3] = TFT_BLUE;
  old_az = -1;
  old_wp_az = -1;
  old_dist = -1;

  while (!ttgo->getTouch(xx, yy)) {  // Wait for touch to exit
    if ((old_az != data_ts.azimuth) || (old_wp_az != data_ts.wp_azimuth) || (old_dist != data_ts.dist)) {
      //  print_top_bar();
      dx = int(float(90) * sin(float(to_azimut / 57.2958)));
      dy = int(float(90) * cos(float(to_azimut / 57.2958)));
      x = 100 + dx;
      y = 100 - dy;
      x1 = 100 + dy / 9;
      y1 = 100 + dx / 9;
      x2 = 100 - dy / 9;
      y2 = 100 - dx / 9;
      hh = tnow.hour;
      mm = tnow.minute;
      ss = tnow.second;
      dday = tnow.day;
      mmonth = tnow.month;
      yyear = tnow.year;
      spr->setColorDepth(2);  // Create an 8bpp Sprite of 60x30 pixels

      spr->createSprite(200, 200);  // 8bpp requires 64 * 30 = 1920 bytes
      spr->createPalette(palette);
      spr->setPivot(100, 100);     // Set pivot relative to top left corner of Sprite
      spr->fillSprite(TFT_BLACK);  // Fill the Sprite with black
      //spr->fillCircle(100, 100, 30, 1);
      //spr->drawFastHLine(0, 100, 200, 2);
      generate_compass(-data_ts.azimuth);
      spr->fillTriangle(x, y, x1, y1, x2, y2, 3);
      spr->fillCircle(100, 100, 9, 3);
      spr->pushRotated(data_ts.wp_azimuth - data_ts.azimuth, 0);
      print_top_bar();
      print_bottom_bar();
    }

    old_az = data_ts.azimuth;
    old_wp_az = data_ts.wp_azimuth;
    old_dist = data_ts.dist;
    delay(50);  //go->tft->
  }
  while (ttgo->getTouch(xx, yy)) {}  // Wait for release
  ttgo->tft->fillScreen(0);
  ttgo->bl->adjust(0);
  ttgo->displaySleep();
  WiFi.mode(WIFI_OFF);
  setCpuFrequencyMhz(20);
  esp_err_t erret;
  erret = gpio_wakeup_enable((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
  erret = esp_sleep_enable_gpio_wakeup();
  //esp_deep_sleep_start();
  esp_light_sleep_start();
}





void setup() {
  //initSetup();
  Serial.begin(115200);
  Serial.println("Starting");
  pinMode(35, INPUT_PULLUP);
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  ttgo->tft->setTextFont(1);
  ttgo->tft->fillScreen(TFT_BLACK);
  ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);  // Note: the new fonts do not draw the background colour
 // pp = ttgo->getWatch()->power;
  tft = ttgo->tft;
  spr = new TFT_eSprite(tft);
  spr2 = new TFT_eSprite(tft);
  top_bar = new TFT_eSprite(tft);
  tft->setPivot(120, 120);
  //Check if the RTC clock matches, if not, use compile time
  ttgo->rtc->check();

  //Synchronize time to system time
  ttgo->rtc->syncToSystem();

  displayTime(true);  // Our GUI to show the time
  ttgo->openBL();     // Turn on the backlight
                      // my_esp_now_init();
  setCpuFrequencyMhz(20);

  pinMode(AXP202_INT, INPUT_PULLUP);
  attachInterrupt(
    AXP202_INT, [] {
      ble_exit = true;
    },
    FALLING);


  // Must be enabled first, and then clear the interrupt status,
  // otherwise abnormal
  power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ,
                   true);

  //  Clear interrupt status
  power->clearIRQ();
}

void loop() {

  if (targetTime < millis()) {
    targetTime = millis() + 1000;
    displayTime(ss == 0);  // Call every second but only update time every minute
  }

  int16_t x, y;
  if (ttgo->getTouch(x, y)) {
    while (ttgo->getTouch(x, y)) {}  // wait for user to release
    ttgo->displayWakeup();
    ttgo->bl->on();
    ttgo->bl->adjust(255);
    // This is where the app selected from the menu is launched
    // If you add an app, follow the variable update instructions
    // at the beginning of the menu code and then add a case
    // statement on to this switch to call your paticular
    // app routine.

    switch (modeMenu()) {  // Call modeMenu. The return is the desired app number
      case 0:              // Zero is the clock, just exit the switch
        break;
      case 1:
        compass();
        break;
      case 2:
        appAccel();
        break;
      case 3:
        appBattery();
        break;
      case 4:
        appTouch();
        break;
      case 5:
        appSetTime();
        break;
      case 6:
        handle_ble();
        break;
    }
    displayTime(true);
  }
}
