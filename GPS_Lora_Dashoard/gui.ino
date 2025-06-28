

const char* wp_type[] = { "Home", "Food", "POI", "Transport", "Hotel", "Office", "Shopping", "Petrol Pump", "Traffic Signal", "School" };
const char* points_page[] = { "Marked Points", "Waypoints" };
const char* menu_a[] = { "Compass", "Satellites", "Position", "List", "Grid", "-", "Confirm", "Confirm", "Confirm", "Back" };
const char* menu_b[] = { "Zoom in", "Zoom in", "-", "-", "Previous Peer", "-", "Delete", "-", "-", "Zoom in" };
const char* menu_c[] = { "Zoom out", "Zoom out", "-", "-", "Next Peer", "-", "Cancel", "Cancel", "Cancel", "Zoom out" };

void print_position(int x, int y) {
  canvas1.setColorDepth(2);
  canvas1.createSprite(220, 205);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_YELLOW);
  // canvas1.setFreeFont(&fonts::Font2);

  canvas1.setColor(0);
  canvas1.fillRect(0, 20, 220, 205);
  canvas1.setColor(2);
  canvas1.drawLine(0, 10, 220, 10);
  canvas1.drawLine(0, 95, 220, 95);
  canvas1.setFont(&fonts::FreeSans9pt7b);
  canvas1.setCursor(5, 15);
  canvas1.setTextColor(3, 0);
  canvas1.print("Local: ");
  canvas1.setCursor(5, 40);
  canvas1.setTextColor(3, 0);
  canvas1.print("Latitude: ");
  canvas1.setTextColor(1, 0);
  canvas1.print(myposition.lat, 6);
  canvas1.setCursor(5, 65);
  canvas1.setTextColor(3, 0);
  canvas1.print("Longitude: ");
  canvas1.setTextColor(1, 0);
  canvas1.print(myposition.lon, 6);

  canvas1.setCursor(5, 105);
  canvas1.setTextColor(3, 0);
  canvas1.print("Remote: ");
  canvas1.setCursor(5, 130);
  canvas1.setTextColor(3, 0);
  canvas1.print("Latitude: ");
  canvas1.setTextColor(1, 0);
  canvas1.print(peers_position[peer_selected].lat, 6);
  canvas1.setCursor(5, 155);
  canvas1.setTextColor(3, 0);
  canvas1.print("Longitude: ");
  canvas1.setTextColor(1, 0);
  canvas1.print(peers_position[peer_selected].lon, 6);

  canvas1.pushSprite(x, y);
  canvas1.deleteSprite();
}



void print_peers_position(int x, int y) {
  offset_m off;


  canvas1.setColorDepth(2);
  canvas1.createSprite(220, 205);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_GREEN);
  canvas1.setPaletteColor(3, TFT_ORANGE);
  // canvas1.setFreeFont(&fonts::Font2);
  canvas1.setColor(0);
  canvas1.fillRect(0, 20, 220, 205);

  for (int w = 0; w < maxpeers; w++) {
    if (peers_stats[w].ts > millis() - 120000) {
      canvas1.setCursor(5, (w * 20) + 20);
      if (w == peer_selected) {
        canvas1.setTextColor(2, 0);
      } else {
        canvas1.setTextColor(3, 0);
      }
      //canvas1.print(" ");
      canvas1.print(peers_position[w].device_name);
      canvas1.setCursor(120, (w * 20) + 20);
      canvas1.setTextColor(1, 0);
      off = calc_movement(peers_position[w], myposition);
      if (off.dist < 5000) {
        canvas1.print(int(off.dist));
        canvas1.print("m");
      } else if (off.dist < 10000) {
        canvas1.print(off.dist / 1000);
        canvas1.print("Km");

      } else {
        canvas1.print(int(off.dist / 1000));
        canvas1.print("Km");
      }
    }
  }





  /*
  canvas1.setColor(2);
  canvas1.drawLine(0, 10, 220, 10);
  canvas1.drawLine(0, 95, 220, 95);
  canvas1.setFont(&fonts::FreeSans9pt7b);
  canvas1.setCursor(5, 15);
  canvas1.setTextColor(3, 0);
  canvas1.print("Local: ");
  canvas1.setCursor(5, 40);
  canvas1.setTextColor(3, 0);
  canvas1.print("Latitude: ");
  canvas1.setTextColor(1, 0);
  canvas1.print(myposition.lat, 6);
  canvas1.setCursor(5, 65);
  canvas1.setTextColor(3, 0);
  canvas1.print("Longitude: ");
  canvas1.setTextColor(1, 0);
  canvas1.print(myposition.lon, 6);

  canvas1.setCursor(5, 105);
  canvas1.setTextColor(3, 0);
  canvas1.print("Remote: ");
  canvas1.setCursor(5, 130);
  canvas1.setTextColor(3, 0);
  canvas1.print("Latitude: ");
  canvas1.setTextColor(1, 0);
  canvas1.print(position_ts.lat, 6);
  canvas1.setCursor(5, 155);
  canvas1.setTextColor(3, 0);
  canvas1.print("Longitude: ");
  canvas1.setTextColor(1, 0);
  canvas1.print(position_ts.lon, 6);

*/
  canvas1.pushSprite(x, y);
  canvas1.deleteSprite();
}





void print_sat_info(int x, int y) {
  canvas1.setColorDepth(2);
  canvas1.createSprite(100, 100);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_GREEN);
  canvas1.clear(TFT_BLACK);
  canvas1.setColor(3);
  canvas1.setTextColor(1, 0);
  canvas1.setCursor(5, 20);
  canvas1.print("Sat: ");
  canvas1.print(mygps.nsat);
  canvas1.setCursor(5, 40);
  canvas1.print("Fixed: ");
  canvas1.print(mygps.nsat_t);
  canvas1.pushSprite(x, y);
  canvas1.deleteSprite();
}


void generate_compass() {
  canvas2.setColorDepth(2);
  canvas2.setTextSize(2);
  canvas2.createSprite(200, 200);
  canvas2.setPaletteColor(0, TFT_BLACK);
  canvas2.setPaletteColor(1, TFT_WHITE);
  canvas2.setPaletteColor(2, TFT_RED);
  canvas2.setColor(1);
  canvas2.fillCircle(100, 100, 100);
  canvas2.setColor(2);
  canvas2.fillCircle(100, 100, 85);
  canvas2.setColor(1);
  canvas2.fillCircle(100, 100, 82);
  canvas2.setColor(0);
  canvas2.fillCircle(100, 100, 65);
  canvas2.setColor(1);
  canvas2.fillCircle(100, 100, 60);
  canvas2.setColor(0);
  canvas2.drawLine(100, 0, 100, 15);
  canvas2.fillTriangle(0, 100, 15, 95, 15, 105);       //W
  canvas2.fillTriangle(200, 100, 185, 95, 185, 105);   //E
  canvas2.fillTriangle(100, 200, 95, 185, 105, 185);   //S
  canvas2.fillTriangle(169, 169, 163, 157, 157, 163);  // SE
  canvas2.fillTriangle(169, 31, 163, 43, 157, 37);     //NE
  canvas2.fillTriangle(31, 31, 37, 43, 43, 37);        //NW
  canvas2.fillTriangle(31, 169, 37, 157, 43, 163);     // SW
  canvas2.setTextColor(0);
  canvas2.setCursor(95, 167);
  canvas2.print("S");
  canvas2.setCursor(22, 94);
  canvas2.print("W");
  canvas2.setCursor(170, 94);
  canvas2.print("E");
  canvas2.setColor(2);
  canvas2.fillTriangle(100, 0, 93, 15, 107, 15);
  canvas2.setTextColor(2);
  canvas2.setCursor(95, 20);
  canvas2.print("N");
}
void generate_compass_mini() {
  compass_mini.setColorDepth(2);
  compass_mini.createSprite(100, 100);
  compass_mini.setPaletteColor(0, TFT_BLACK);
  compass_mini.setPaletteColor(1, TFT_WHITE);
  compass_mini.setPaletteColor(2, TFT_RED);

  compass_mini.setColor(1);
  compass_mini.fillCircle(50, 50, 50);
  compass_mini.setColor(2);
  compass_mini.fillCircle(50, 50, 45);
  compass_mini.setColor(1);
  compass_mini.fillCircle(50, 50, 43);
  compass_mini.setColor(0);
  compass_mini.fillCircle(50, 50, 35);
  compass_mini.setColor(1);
  compass_mini.fillCircle(50, 50, 33);
  compass_mini.setColor(0);
  //compass_mini.drawLine(100, 0, 100, 15);
  compass_mini.fillTriangle(0, 50, 15, 42, 15, 58);    //W
  compass_mini.fillTriangle(100, 50, 85, 42, 85, 58);  //E
  compass_mini.fillTriangle(50, 100, 42, 85, 58, 85);  //S

  compass_mini.setColor(2);
  compass_mini.fillTriangle(50, 0, 42, 15, 58, 15);
}




int getBatteryLevel() {
  return M5.Power.getBatteryLevel();
  /*Wire.beginTransmission(0x75);
  Wire.write(0x78);
  if (Wire.endTransmission(false) == 0 && Wire.requestFrom(0x75, 1)) {
    switch (Wire.read() & 0xF0) {
      case 0xE0: return 25;
      case 0xC0: return 50;
      case 0x80: return 75;
      case 0x00: return 100;
      default: return 0;
    }
  }
  return -1;
  */
}

void print_top_bar() {
  int level = 0;  //100 + rssi; //getBatteryLevel();
  level = getBatteryLevel();
  sidet.setColorDepth(2);
  //sidet.setTextSize(2);
  sidet.setTextSize(1);
  sidet.createSprite(320, 20);
  sidet.setPaletteColor(0, TFT_BLACK);
  sidet.setPaletteColor(1, TFT_WHITE);
  sidet.setPaletteColor(2, TFT_RED);
  sidet.setPaletteColor(3, TFT_GREEN);
  sidet.setFont(&fonts::FreeSans9pt7b);
  sidet.clear(TFT_BLACK);

  sidet.fillSprite(TFT_BLACK);
  sidet.setColor(3);
  // battery
  sidet.drawRect(270, 3, 45, 14);
  if (level < 40) { sidet.setColor(2); };
  sidet.fillRect(272, 5, int((41 * level) / 100), 10);
  sidet.fillRect(315, 7, 3, 6);
  // fixed sats

  if (mygps.fix == 1) {
    sidet.setColor(3);
    sidet.setTextColor(3, 0);
    sidet.drawCircle(90, 10, 2);
    sidet.drawLine(86, 6, 94, 14);
    sidet.drawLine(84, 8, 88, 4);
    sidet.drawLine(92, 16, 96, 12);
  } else {
    sidet.setColor(2);
    sidet.setTextColor(2, 0);
    sidet.drawCircle(90, 10, 2);
    sidet.drawLine(86, 6, 94, 14);
    sidet.drawLine(84, 8, 88, 4);
    sidet.drawLine(92, 16, 96, 12);
  }
  //sidet.setTextSize(1.3);
  sidet.setCursor(100, 2);
  sidet.print(mygps.nsat_t);  ///
                              // sidet.setCursor(135, 6);
  sidet.print("/");
  // sidet.setCursor(143, 10);
  sidet.print(mygps.nsat);

  // Lora signal
  if ((millis() - lora_data.ts) < 60000) {
    sidet.setColor(3);
    sidet.setTextColor(3, 0);
    sidet.drawCircle(160, 10, 8);
    sidet.drawLine(160, 18, 160, 2);
    sidet.drawLine(152, 10, 168, 10);
    sidet.drawLine(156, 6, 164, 6);
  } else {
    sidet.setColor(2);
    sidet.setTextColor(2, 0);
    sidet.drawCircle(160, 10, 8);
    sidet.drawLine(160, 18, 160, 2);
    sidet.drawLine(152, 10, 168, 10);
    sidet.drawLine(156, 6, 164, 6);
  }
  //  sidet.setTextSize(1);
  sidet.setCursor(175, 2);
  if (lora_data.ts > 0) {
    sidet.print(lora_data.rssi);
  } else {
    sidet.print("---");
  }

  // Date - time
  sidet.setTextColor(1, 0);
  //  sidet.setTextSize(1);
  sidet.setCursor(0, 0);
  // sidet.drawString(mygps.parsedtime,0,0,&fonts::FreeMono9pt7b);
  sidet.print(mygps.parsedtime);

  sidet.pushSprite(0, 0);
  sidet.deleteSprite();
}



//   Draw compass heading to moving direction

/* */
void print_compass(int xs, int ys) {
  float dx, dy;
  int x1, x2, x3, x4, y1, y2, y3, y4;
  int xh1, yh1, xh2, yh2, xh3, yh3, xh4, yh4;
  int north;
  char buffer[10];
  north = -movement.azimuth;
  canvas1.setColorDepth(2);
  canvas1.createSprite(220, 200);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  //generate_compass();
  canvas2.pushRotateZoom(100, 100, -movement.azimuth, 1, 1);
  canvas1.setTextColor(0, 1);
  canvas1.setTextSize(1.5);
  canvas1.setCursor(60, 90);
  //canvas1.print(movement.azimuth);
  //canvas1.print("\"");
  sprintf(buffer, "%d\"", movement.azimuth);
  canvas1.drawCenterString(buffer, 100, 90);
  // canvas1.pushRotateZoom(100, 100, 0, 1, 1);
  canvas1.pushSprite(xs, ys);
  canvas1.deleteSprite();
}

void print_compass_mini() {
  float dx, dy;
  int x = 270;
  int y = 115;
  char buffer[10];
  int north;
  north = -movement.azimuth;
  canvas1.setColorDepth(2);
  canvas1.createSprite(100, 100);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  //generate_compass();
  compass_mini.pushRotateZoom(50, 50, -movement.azimuth, 1, 1);
  //canvas1.setTextColor(0, 1);
  //canvas1.setTextSize(1);
  //canvas1.setCursor(30, 45);
  //canvas1.print(movement.azimuth);
  //sprintf(buffer, "%dM", int(current.high));
  //canvas1.drawCenterString(buffer, 50, 45);
  //canvas1.print("\"");
  // canvas1.pushRotateZoom(100, 100, 0, 1, 1);
  canvas1.pushSprite(220, 125);
  canvas1.deleteSprite();
}



/*
   Draw the direction of the waypoint, heading the direction of moving
*/
void print_go_wp(int xs, int ys) {
  offset_m off;
  String tmp;
  off = calc_movement(peers_position[peer_selected], myposition);
  tmp = "Peer";

  int x, y, x1, y1, x2, y2, dx, dy;
  int to_azimut = off.azimuth;
  canvas1.setColorDepth(2);
  canvas1.createSprite(220, 200);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_GREEN);

  canvas2.pushRotateZoom(100, 100, 0, 1, 1);

  dx = int(float(90) * sin(float(to_azimut / 57.2958)));
  dy = int(float(90) * cos(float(to_azimut / 57.2958)));
  x = 100 + dx;
  y = 100 - dy;
  x1 = 100 + dy / 9;
  y1 = 100 + dx / 9;
  x2 = 100 - dy / 9;
  y2 = 100 - dx / 9;
  canvas1.setColor(3);
  canvas1.fillTriangle(x, y, x1, y1, x2, y2);
  canvas1.setColor(2);
  canvas1.drawLine(100, 100, x, y);
  canvas1.setColor(0);
  canvas1.fillCircle(100, 100, 10);
  canvas1.pushSprite(xs, ys);
  canvas1.deleteSprite();
}

void print_info() {
  offset_m off;
  String tmp;
  off = calc_movement(peers_position[peer_selected], myposition);
  //off = calc_movement(position_ts, myposition);
  if (wp_pos > -1) {
    tmp = waypoints[wp_pos].name;
  } else {
    tmp = "-";
  }
  if (off.dist > max_distance) {
    max_distance = off.dist;
  }
  canvas1.setColorDepth(2);
  canvas1.createSprite(100, 105);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_GREEN);
  canvas1.setFont(&fonts::FreeMono9pt7b);  // &fonts::Font0
  canvas1.clear(TFT_BLACK);
  canvas1.setTextColor(1, 0);
  // canvas1.setTextSize(1.5);
  canvas1.setCursor(0, 0);
  canvas1.print("Dist: ");
  canvas1.setCursor(0, 18);
  canvas1.setTextColor(3, 0);
  canvas1.setFont(&fonts::FreeMono12pt7b);  // &fonts::Font0
  if (off.dist < 5000) {
    canvas1.print(int(off.dist));
    canvas1.print("m");
  } else if (off.dist < 10000) {
    canvas1.print(off.dist / 1000);
    canvas1.print("Km");

  } else {
    canvas1.print(int(off.dist / 1000));
    canvas1.print("Km");
  }

  // wp info
  canvas1.setCursor(0, 43);
  canvas1.setTextColor(1, 0);
  canvas1.setFont(&fonts::FreeMono9pt7b);  // &fonts::Font0
  canvas1.print("Max: ");
  canvas1.setFont(&fonts::FreeMono12pt7b);  // &fonts::Font0
  // WP distance
  canvas1.setCursor(0, 58);
  canvas1.setTextColor(3, 0);
  if (max_distance < 5000) {
    canvas1.print(int(max_distance));
    canvas1.print("m");
  } else if (max_distance < 100000) {
    canvas1.print(max_distance / 1000);
    canvas1.print("Km");

  } else {
    canvas1.print(int(max_distance / 1000));
    canvas1.print("Km");
  }
  canvas1.setColor(1);
  canvas1.fillRect(5, 90, 30, 10);
  canvas1.setColor(0);
  canvas1.drawFastHLine(5, 95, 30);
  canvas1.drawFastVLine(10, 90, 10);
  canvas1.drawFastVLine(30, 90, 10);
  // canvas1.setTextSize(1.5);
  canvas1.setFont(&fonts::FreeMono9pt7b);  // &fonts::Font0
  canvas1.setCursor(40, 87);
  canvas1.setTextColor(1, 0);
  canvas1.print(zoom_l[c_zoom]);

  canvas1.pushSprite(220, 20);
  canvas1.deleteSprite();
}



void print_go_tbt() {

  offset_m off;

  off = calc_movement(peers_position[peer_selected], myposition);
  int x, y, x1, y1, x2, y2, dx, dy;
  int to_azimut = off.azimuth;
  canvas1.setColorDepth(2);
  canvas1.createSprite(100, 100);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_GREEN);

  canvas2.pushRotateZoom(50, 50, 0, 0.5, 0.5);

  dx = int(float(45) * sin(float(to_azimut / 57.2958)));
  dy = int(float(45) * cos(float(to_azimut / 57.2958)));
  x = 50 + dx;
  y = 50 - dy;
  x1 = 50 + dy / 9;
  y1 = 50 + dx / 9;
  x2 = 50 - dy / 9;
  y2 = 50 - dx / 9;
  canvas1.setColor(3);
  canvas1.fillTriangle(x, y, x1, y1, x2, y2);
  canvas1.setColor(2);
  canvas1.drawLine(50, 50, x, y);
  canvas1.setColor(0);
  canvas1.fillCircle(50, 50, 5);
  canvas1.pushSprite(220, 125);
  canvas1.deleteSprite();
}




void list_points() {
  int i, j, i_initial, i_max;
  struct points* tmp_point;

  offset_m off;
  display.fillRect(0, 0, 320, 240, BLACK);
  display.drawLine(0, 30, 320, 30, RED);
  display.drawLine(220, 0, 220, 240, RED);
  display.setCursor(10, 18);
  display.setTextColor(GREEN);
  display.print(groups[menu_x].name);
  display.setCursor(225, 18);
  display.print("Details");
  //display.setCursor(40,i*20+50);
  tmp_point = waypoints;
  point_select = &wp_pos;
  point_max = &wp_t;

  off = calc_movement(tmp_point[*point_select], current);
  i_initial = 0;
  i_max = *point_max;
  if ((*point_max > 10) && (*point_select > 5)) {
    i_initial = *point_select - 5;
    i_max = *point_select + 5;
    if (i_max > *point_max) {
      i_max = *point_max;
    };
  }
  for (i = i_initial; i < i_max; i++) {
    j = tmp_point[i].type;

    if (i == *point_select) {
      display.setTextColor(GREEN);
      display.drawXBitmap(10, (i - i_initial) * 18 + 45, symbols_bitmap[j], 10, 10, GREEN, BLACK);
    } else {
      display.setTextColor(WHITE);
      display.drawXBitmap(10, (i - i_initial) * 18 + 45, symbols_bitmap[j], 10, 10, ORANGE, BLACK);
    }

    display.setCursor(30, (i - i_initial) * 18 + 50);
    display.print(tmp_point[i].name);
  }
  display.setTextColor(WHITE);
  display.setCursor(225, 48);
  display.print("Latitude:");
  display.setCursor(225, 66);
  display.setTextColor(RED);
  display.print(tmp_point[*point_select].lat, 6);
  display.setCursor(225, 85);
  display.setTextColor(WHITE);
  display.print("Longitude:");
  display.setCursor(225, 103);
  display.setTextColor(RED);
  display.print(tmp_point[*point_select].lon, 6);
  display.setCursor(225, 122);
  display.setTextColor(WHITE);
  display.print("Altitude:");
  display.setCursor(225, 140);
  display.setTextColor(RED);
  display.print(tmp_point[*point_select].high, 0);
  display.print(" M");
  display.setCursor(225, 159);
  display.setTextColor(WHITE);
  display.print("Type:");
  display.setCursor(225, 177);
  display.setTextColor(RED);
  j = tmp_point[*point_select].type;
  display.print(wp_type[j]);


  display.setTextColor(WHITE);
  display.setCursor(225, 196);
  display.print("Distance:");
  display.setCursor(225, 214);
  display.setTextColor(RED);
  if (off.dist < 5000) {
    display.print(off.dist);
    display.print(" M");
  } else if (off.dist < 100000) {
    display.print(off.dist / 1000);
    display.print(" Km");

  } else {
    display.print(int(off.dist / 1000));
    display.print(" Km");
  }
  display.setTextColor(WHITE);
}

void print_wp(int m) {
  grid.setColorDepth(2);
  grid.createSprite(m, m);
  grid.setPaletteColor(0, TFT_BLACK);
  grid.setPaletteColor(1, TFT_WHITE);
  grid.setPaletteColor(2, TFT_ORANGE);
  grid.setPaletteColor(3, TFT_GREEN);
  // grid.clear(TFT_GREEN);
  grid.setColor(0);
  grid.clear(TFT_BLACK);

  int x, y, i, j, old_x, old_y;
  offset_m off;
  for (i = 0; i < wp_t; i++) {
    off = calc_movement(waypoints[i], current);
    x = (m / 2) + int((off.x / zoom[c_zoom])) + display_offset.x;
    y = (m / 2) + int((off.y / zoom[c_zoom])) + display_offset.y;
    if ((x >= 0) && (x <= m) && (y >= 0) && (y <= m)) {
      if (i == wp_pos) {
        j = waypoints[i].type;
        grid.drawXBitmap(x, y, symbols_bitmap[j], 10, 10, 3, 0);
        // M5.Lcd.fillCircle(x, y , 7, RED);

      } else {
        j = waypoints[i].type;
        grid.drawXBitmap(x, y, symbols_bitmap[j], 10, 10, 2, 0);
      }
    }
  }
}




void prepare_grid(int m) {
  int i;
  //int m = 300;
  grid.setColorDepth(2);
  grid.createSprite(m, m);
  grid.setPaletteColor(0, TFT_BLACK);
  grid.setPaletteColor(1, TFT_WHITE);
  grid.setPaletteColor(2, TFT_RED);
  grid.setPaletteColor(3, TFT_BLUE);
  // grid.clear(TFT_GREEN);
  grid.setColor(1);
  grid.fillRect(0, 0, m, m);

  //
  grid.setColor(0);
  for (i = m / 2; i < m; i += 25) {
    grid.drawLine(i, 0, i, m);
  }
  for (i = m / 2; i > 0; i -= 25) {
    grid.drawLine(i, 0, i, m);
  }

  for (i = m / 2; i < m; i += 25) {
    grid.drawLine(0, i, m, i);
  }
  for (i = m / 2; i > 0; i -= 25) {
    grid.drawLine(0, i, m, i);
  }



  grid.drawLine(0, (m / 2) - 1, m, (m / 2) - 1);
  grid.drawLine((m / 2) - 1, 0, (m / 2) - 1, m);
  grid.fillTriangle(m / 2, int(m / 5), (m / 2) - 5, int(m / 5) + 10, (m / 2) + 5, int(m / 5) + 10, 2);
}

/* Draw the grip and stored points */
void print_track(int xs, int ys, int width, int height) {
  int x, y, i, old_x, old_y, m;
  offset_m off;
  m = int(width * 1.5);
  prepare_grid(m);
  canvas1.setColorDepth(2);
  canvas1.createSprite(width, height);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_BLUE);
  // canvas1.clear(TFT_GREEN);

  /*
  if ((overflow_track) && (track_pos < 358)) {
    for (i = track_pos + 1; i < 360; i++) {
      off = calc_movement(track[i], myposition);
      x = m / 2 - int((off.x / zoom[c_zoom])) + display_offset.x;
      y = m / 2 + int((off.y / zoom[c_zoom])) + display_offset.y;
      if ((x >= 0) && (x < m) && (y >= 0) && (y < m)) {
        grid.setColor(3);
        grid.fillCircle(x, y, 5);
        if (i > track_pos) {
          grid.setColor(2);
          grid.drawLine(x, y, old_x, old_y);
        }
      }
      old_x = x;
      old_y = y;
    }
  }

  for (i = 0; i <= track_pos; i++) {
    off = calc_movement(track[i], myposition);
    x = m / 2 - int((off.x / zoom[c_zoom])) + display_offset.x;
    y = m / 2 + int((off.y / zoom[c_zoom])) + display_offset.y;
    if ((x > 0) && (x < m) && (y > 0) && (y < m)) {
      grid.setColor(3);
      grid.fillCircle(x, y, 5);
      if (i > 0) {
        grid.setColor(2);
        grid.drawLine(x, y, old_x, old_y);
      }
    }
    old_x = x;
    old_y = y;
  }
  */
  /* All valid points */
  for (int w = 0; w < maxpeers; w++) {
    if (peers_stats[w].ts > millis() - 120000) {
      off = calc_movement(peers_position[w], myposition);
      x = -int((off.x / zoom[c_zoom])) + display_offset.x;
      y = int((off.y / zoom[c_zoom])) + display_offset.y;
      grid.setColor(2);
      grid.fillCircle((m / 2) + x, (m / 2) + y, 4);
      grid.setCursor((m / 2) + x + 10, (m / 2) + y);
      //grid.setFont(&fonts::FreeMono12pt7b);
      grid.setTextColor(TFT_RED, 1);
      grid.setFont(&fonts::FreeSans9pt7b);
      grid.println(peers_position[w].device_name);
    }
  }
  grid.pushRotateZoom(width / 2, height / 2, -movement.azimuth, 1, 1);


  //   grid.pushRotateZoom(100, 100, 0, 1, 1);
  canvas1.pushSprite(xs, ys);
  // canvas1.pushRotateZoom(100, 100, 0, 1, 1);
  grid.deleteSprite();
  canvas1.deleteSprite();
  grid.deleteSprite();
  canvas1.setColorDepth(2);
  canvas1.createSprite(width, height);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_ORANGE);
  canvas1.setPaletteColor(3, TFT_GREEN);
  print_wp(m);
  grid.pushRotateZoom(width / 2, height / 2, -movement.azimuth, 1, 1, TFT_BLACK);
  canvas1.pushSprite(xs, ys, TFT_BLACK);
  canvas1.deleteSprite();
  grid.deleteSprite();
}



void print_zoom() {
  int x = 220;
  int y = 80;
  canvas1.setColorDepth(2);
  canvas1.createSprite(100, 20);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_BLUE);
  canvas1.clear(TFT_BLACK);
  canvas1.setTextSize(1.5);
  canvas1.setCursor(5, 1);
  canvas1.setTextColor(1, 0);
  canvas1.print("Zoom: ");
  canvas1.print(c_zoom);
  canvas1.pushSprite(x, y);
  canvas1.deleteSprite();
}

void print_to_wp(float dist) {
  M5.Lcd.fillRect(220, 111, 320, 129, TFT_LIGHTGREY);
  M5.Lcd.setCursor(225, 125);
  M5.Lcd.print("WP: ");
  M5.Lcd.print(dist);
}


void print_menu() {
  canvas1.setColorDepth(2);
  canvas1.createSprite(320, 15);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_LIGHTGREY);
  canvas1.setPaletteColor(3, TFT_BLUE);
  canvas1.setFreeFont(&fonts::Font2);
  canvas1.setColor(2);
  canvas1.setTextSize(1);
  canvas1.fillRect(0, 0, 320, 15);
  canvas1.setColor(0);
  canvas1.setTextColor(0, 2);
  canvas1.setCursor(27, 0);
  canvas1.print(menu_a[page]);

  canvas1.setCursor(130, 0);
  canvas1.print(menu_b[page]);

  canvas1.setCursor(230, 0);
  canvas1.print(menu_c[page]);
  canvas1.pushSprite(0, 225);

  canvas1.deleteSprite();
}

void sat_circle(int xs, int ys) {
  int i, x, y;

  canvas1.setColorDepth(2);
  canvas1.createSprite(220, 200);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_BLUE);
  canvas1.clear(TFT_BLACK);
  canvas1.setColor(2);
  canvas1.fillCircle(100, 100, 95);
  canvas1.setColor(0);
  canvas1.fillCircle(100, 100, 92);
  canvas1.setColor(3);
  for (i = 1; i <= mygps.nsat; i++) {
    x = 100 + int(float(mysat[i].elevation) * cos(float(mysat[i].azimuth / 57.2958)));
    y = 100 + int(float(mysat[i].elevation) * sin(float(mysat[i].azimuth / 57.2958)));
    canvas1.setColor(3);
    canvas1.drawRect(x - 8, y - 8, 16, 16);
    canvas1.setTextColor(1, 0);
    canvas1.fillRect(x - 7, y + 7 - int(mysat[i].snr / 7.0), 14, int(mysat[i].snr / 7.0));
    /* 
    canvas1.setCursor(x - 6, y - 4);
    canvas1.print(mysat[i].sat_n);
    */
  }
  canvas1.pushSprite(xs, ys);
  //canvas1.pushRotateZoom(100, 100, 0, 1, 1);

  canvas1.deleteSprite();
}

void sat_stats() {

  canvas1.setColorDepth(2);
  canvas1.createSprite(100, 100);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_GREEN);
  canvas1.clear(TFT_BLACK);
  canvas1.setColor(3);
  canvas1.drawRect(5, 5, 10, 35);
  canvas1.fillRect(5, 40 - int(mysat[1].snr / 3.5), 10, int(mysat[1].snr / 3.5));
  canvas1.drawRect(20, 5, 10, 35);
  canvas1.fillRect(20, 40 - int(mysat[2].snr / 3.5), 10, int(mysat[2].snr / 3.5));
  canvas1.drawRect(35, 5, 10, 35);
  canvas1.fillRect(35, 40 - int(mysat[3].snr / 3.5), 10, int(mysat[3].snr / 3.5));
  canvas1.drawRect(50, 5, 10, 35);
  canvas1.fillRect(50, 40 - int(mysat[4].snr / 3.5), 10, int(mysat[4].snr / 3.5));
  canvas1.drawRect(65, 5, 10, 35);
  canvas1.fillRect(65, 40 - int(mysat[5].snr / 3.5), 10, int(mysat[5].snr / 3.5));
  canvas1.drawRect(80, 5, 10, 35);
  canvas1.fillRect(80, 40 - int(mysat[6].snr / 3.5), 10, int(mysat[6].snr / 3.5));
  canvas1.drawRect(5, 50, 10, 35);
  canvas1.fillRect(5, 85 - int(mysat[7].snr / 3.5), 10, int(mysat[7].snr / 3.5));
  canvas1.drawRect(20, 50, 10, 35);
  canvas1.fillRect(20, 85 - int(mysat[8].snr / 3.5), 10, int(mysat[8].snr / 3.5));
  canvas1.drawRect(35, 50, 10, 35);
  canvas1.fillRect(35, 85 - int(mysat[9].snr / 3.5), 10, int(mysat[9].snr / 3.5));
  canvas1.drawRect(50, 50, 10, 35);
  canvas1.fillRect(50, 85 - int(mysat[10].snr / 3.5), 10, int(mysat[10].snr / 3.5));
  canvas1.drawRect(65, 50, 10, 35);
  canvas1.fillRect(65, 85 - int(mysat[11].snr / 3.5), 10, int(mysat[11].snr / 3.5));
  canvas1.drawRect(80, 50, 10, 35);
  canvas1.fillRect(80, 85 - int(mysat[12].snr / 3.5), 10, int(mysat[12].snr / 3.5));
  canvas1.pushSprite(220, 125);

  canvas1.deleteSprite();
}
