#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "SSD1306.h"
#include <esp_now.h>
#include <WiFi.h>
#include "config.h"




TaskHandle_t Task1;


const uint8_t vbatPin = 35;
unsigned int counter = 0;
float VBAT;  // battery voltage from ESP32 ADC reada
unsigned long sendTime = 0;
unsigned long last_message = 3600;
SSD1306 display(0x3c, 21, 22);
String rssi = "RSSI --";
String packSize = "--";
String packet;
esp_now_peer_info_t peerInfo;
float batt1 = 0;
float gpsbatt = 0;
bool lorasendready = false;
bool lorareceivedready = false;
void setup() {



  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("------------------------------------");
  Serial.println();
  Serial.println("LoRa Sender Test");

  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  LoRa.setTxPower(22);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  LoRa.setSpreadingFactor(12);
  LoRa.setPreambleLength(16);
  // LoRa.onReceive(cbk);
  // LoRa.receive();
  Serial.println("init ok");
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  delay(1500);

  my_esp_now_init();
  xTaskCreatePinnedToCore(
    LoraReceiver, /* Function to implement the task */
    "Task1",      /* Name of the task */
    10000,        /* Stack size in words */
    NULL,         /* Task input parameter */
    10,           /* Priority of the task */
    &Task1,       /* Task handle. */
    0);           /* Core where the task should run */
}

void loop() {
  int tt = touchRead(12);

  if (tt < 40) {

    get_battery();
    display.displayOn();
    display.clear();
    display.drawLine(0, 0, 127, 0);
    display.drawLine(0, 63, 127, 63);
    display.drawLine(0, 0, 0, 63);
    display.drawLine(127, 0, 127, 63);
    display.drawLine(0, 16, 127, 16);
    display.drawRect(2, 2, 20, 10);
    display.fillRect(2, 2, batt1, 10);
    display.drawString(80, 2, "GPS");
    display.fillRect(22, 5, 2, 4);
    display.drawRect(103, 2, 20, 10);
    display.fillRect(103, 2, gpsbatt, 10);
    display.fillRect(123, 5, 2, 4);

    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 25, "Last sent: ");
    if (sendTime > 0) {
      display.drawString(50, 25, String(last_message));
      display.drawString(90, 25, " S ago");
    } else {
      display.drawString(50, 25, " Never");
    }
    display.display();

  } else {
    display.displayOff();
  }
  delay(20);
  if (lorasendready) {
    lorasend();
    lorasendready = false;
    Serial.println("Sending via Lora");
    hexDump(& position_ts,sizeof(position_ts));
  }

  if (lorareceivedready) {
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&position_ls, sizeof(position_ls));
    lorareceivedready = false;
    Serial.println(" ++++++++++++ Received from lora +++++++++++++");
  }
}
