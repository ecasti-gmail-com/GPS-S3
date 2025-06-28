void handle_ble() {
  ble_exit = false;
  unsigned long startTime;
  setCpuFrequencyMhz(240);

  delay(500);
  if (!ble_enabled) {
    bleMouse.begin();
    int bat_prcntg = power->getBattVoltage() / 100;
    bleMouse.setBatteryLevel(bat_prcntg);
    ble_enabled = false;
  }

  while (true) {

    if (digitalRead(TOUCH_INT) == LOW) {
      if (ttgo->getTouch(x, y)) {

        if (!touch_down) {
          xx = x;
          yy = y;
          touch_down = true;
          moved = false;
          Serial.print("[");
          delay(50);
        } else {
          moved = true;
          // sprintf(buf, "x:%03d  y:%03d", x, y);
          // snprintf(buf, 128, "V:%.2f ", power->getBattVoltage() / 100 );
          // ttgo->tft->drawString(buf, 80, 118);
          if (bleMouse.isConnected()) {
            startTime = millis();
            while (millis() < startTime + 30) {
              bleMouse.move((x - xx), (y - yy));
              delay(5);
            }
            xx = x;
            yy = y;
          }
        }
      } else {
        if (touch_down) {
          if (!moved) {
            if (x < 120) {
              bleMouse.click(MOUSE_LEFT);
            } else {
              bleMouse.click(MOUSE_RIGHT);
            }
          }
          Serial.println("]");
          touch_down = false;
        }
      }
    }

    int bat_prcntg = power->getBattVoltage() / 100;
    bleMouse.setBatteryLevel(bat_prcntg);
    if ( ble_exit ) {
      Serial.println("Going in standby");
      bleMouse.end();
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

    delay(50);
  }
}
