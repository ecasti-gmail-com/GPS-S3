void generate_compass(int de) {
  spr2->setColorDepth(2);
  spr2->createSprite(200, 200);  // 8bpp requires 64 * 30 = 1920 bytes
  palette[0] = TFT_BLACK;
  palette[1] = TFT_WHITE;
  palette[2] = TFT_RED;
  palette[3] = TFT_BLUE;
  spr2->createPalette(palette);
  spr2->fillSprite(TFT_BLACK);
  spr2->setPivot(100, 100);  // Set pivot relative to top left corner of Sprite
  spr2->fillCircle(100, 100, 100, 1);
  spr2->fillCircle(100, 100, 85, 2);
  spr2->fillCircle(100, 100, 82, 1);
  spr2->fillCircle(100, 100, 65, 0);
  spr2->fillCircle(100, 100, 60, 1);
  spr2->fillTriangle(0, 100, 15, 95, 15, 105, 0);       //W
  spr2->fillTriangle(200, 100, 185, 95, 185, 105, 0);   //E
  spr2->fillTriangle(100, 200, 95, 185, 105, 185, 0);   //S
  spr2->fillTriangle(169, 169, 163, 157, 157, 163, 0);  // SE
  spr2->fillTriangle(169, 31, 163, 43, 157, 37, 0);     //NE
  spr2->fillTriangle(31, 31, 37, 43, 43, 37, 0);        //NW
  spr2->fillTriangle(31, 169, 37, 157, 43, 163, 0);     // SW
  spr2->fillTriangle(100, 0, 93, 15, 107, 15, 2);       // N
  spr2->pushRotated(de, 3);
  //spr2->deleteSprite();
  //spr2->pushSprite(100,100);
}

void print_top_bar() {
  top_bar->setColorDepth(2);
  top_bar->createSprite(240, 20);  // 8bpp requires 64 * 30 = 1920 bytes
  top_bar->setTextDatum(BR_DATUM);
  palette_m[0] = TFT_BLACK;
  palette_m[1] = TFT_WHITE;
  palette_m[2] = TFT_RED;
  palette_m[3] = TFT_GREEN;
  top_bar->createPalette(palette_m);
  top_bar->setPivot(0, 0);  // Set pivot relative to top left corner of Sprite
  top_bar->setScrollRect(20, 0, 220, 20, TFT_WHITE);
  top_bar->fillSprite(TFT_BLACK);
  // top_bar->setTextSize(1);
  if (data_ts.fixed > 3) {
    top_bar->setTextColor(3);
    top_bar->drawCircle(10, 10, 2, 3);
    top_bar->drawLine(6, 6, 14, 14, 3);
    top_bar->drawLine(4, 8, 8, 4, 3);
    top_bar->drawLine(12, 16, 16, 12, 3);
  } else {
    top_bar->setTextColor(2);
    top_bar->drawCircle(10, 10, 2, 2);
    top_bar->drawLine(6, 6, 14, 14, 2);
    top_bar->drawLine(4, 8, 8, 4, 2);
    top_bar->drawLine(12, 16, 16, 12, 2);
  }
  top_bar->setCursor(22, 0);
  top_bar->setTextFont(2);
  top_bar->print(data_ts.fixed);
  top_bar->print("/");
  top_bar->print(data_ts.sat);
  // speed
  top_bar->setTextColor(1);
  top_bar->setCursor(100, 0);
  if (data_ts.speed < 300) {
    top_bar->print(data_ts.speed);
    top_bar->print("Km/h");
  } else {
    top_bar->print("---");
  };

  // battery
  //top_bar->setColor(3);
  top_bar->drawRect(190, 3, 45, 14, 3);
  if (data_ts.battery < 40) {
    top_bar->fillRect(192, 5, int((41 * data_ts.battery) / 100), 10, 2);
    top_bar->fillRect(235, 7, 3, 6, 2);
  } else {
    top_bar->fillRect(192, 5, int((41 * data_ts.battery) / 100), 10, 3);
    top_bar->fillRect(235, 7, 3, 6, 3);
  };


  // top_bar->deleteSprite();
  // END
  top_bar->pushSprite(0, 0);
}
void print_bottom_bar() {
  top_bar->setColorDepth(2);
  top_bar->createSprite(240, 20);  // 8bpp requires 64 * 30 = 1920 bytes
  top_bar->setTextDatum(BR_DATUM);
  palette_m[0] = TFT_BLACK;
  palette_m[1] = TFT_WHITE;
  palette_m[2] = TFT_RED;
  palette_m[3] = TFT_GREEN;
  top_bar->createPalette(palette_m);
  top_bar->setPivot(0, 0);  // Set pivot relative to top left corner of Sprite
  top_bar->setScrollRect(0, 0, 220, 20, TFT_WHITE);
  top_bar->fillSprite(TFT_BLACK);
  // top_bar->setTextSize(1);

  top_bar->setTextColor(1);
  top_bar->setCursor(0, 5);
  top_bar->print(data_ts.wp_name);
  top_bar->setCursor(160, 5);
  top_bar->setTextFont(2);
  if (data_ts.dist < 5000) {
    top_bar->print(data_ts.dist);
    top_bar->print("m");
  } else if (data_ts.dist < 100000) {
    top_bar->print(data_ts.dist / 1000);
    top_bar->print("Km");
  } else {
    top_bar->print(int(data_ts.dist / 1000));
    top_bar->print("Km");
  }


  top_bar->pushSprite(0, 220);
  // top_bar->deleteSprite();
}