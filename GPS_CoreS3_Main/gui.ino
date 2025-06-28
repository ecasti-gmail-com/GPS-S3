


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
  int perc = 0;
  float vv = 4.2;

  vv = CoreS3.Power.getBatteryVoltage();
  perc = ((vv / 1000) - 3.2) * 100;
  if (perc > 100) { perc = 100; };
  if (perc < 0) { perc = 0; };
  return perc;
}

void print_top_bar() {
  int level = getBatteryLevel();
  int speed;
  speed = int(movement.dist * 0.36);  // metter / second into KM / hour
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

  if (mygps.fix > 0) {
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
  sidet.setCursor(100, 2);
  sidet.print(mygps.nsat_t);  ///
  sidet.print("/");
  sidet.print(mygps.nsat);

  // Dynamic compass
  if (movement.dist < 10) {
    sidet.setColor(2);
    sidet.setTextColor(2, 0);
    sidet.drawCircle(160, 10, 8);
    sidet.fillTriangle(152, 10, 164, 6, 160, 18);
    sidet.setColor(0);
    sidet.fillTriangle(153, 11, 159, 11, 159, 17);
  } else {
    sidet.setColor(3);
    sidet.setTextColor(1, 0);
    sidet.drawCircle(160, 10, 8);
    sidet.fillTriangle(152, 10, 164, 6, 160, 18);
    sidet.setColor(0);
    sidet.fillTriangle(153, 11, 159, 11, 159, 17);
  }
  //  sidet.setTextSize(1);
  sidet.setCursor(175, 2);
  if (speed < 300) {

    sidet.print(speed);

    sidet.print(" Km/h");
  } else {
    sidet.print(" -- ");
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

  char buffer[10];

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

  canvas1.setColorDepth(2);
  canvas1.createSprite(100, 100);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  //generate_compass();
  compass_mini.pushRotateZoom(50, 50, -movement.azimuth, 1, 1);
  canvas1.setTextColor(0, 1);
  canvas1.setTextSize(1);
  //canvas1.setCursor(30, 45);
  //canvas1.print(movement.azimuth);
  sprintf(buffer, "%dM", int(current.high));
  canvas1.drawCenterString(buffer, 50, 45);
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
  off = calc_movement(waypoints[wp_pos], current);
  tmp = waypoints[wp_pos].name;

  int x, y, x1, y1, x2, y2, dx, dy;
  int to_azimut = off.azimuth - movement.azimuth;
  canvas1.setColorDepth(2);
  canvas1.createSprite(220, 200);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_GREEN);

  canvas2.pushRotateZoom(100, 100, -movement.azimuth, 1, 1);

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
  off = calc_movement(waypoints[wp_pos], current);
  if (wp_pos > -1) {
    tmp = waypoints[wp_pos].name;
  } else {
    tmp = "-";
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
  canvas1.print("Trip: ");
  canvas1.setCursor(0, 18);
  canvas1.setTextColor(3, 0);
  canvas1.setFont(&fonts::FreeMono12pt7b);  // &fonts::Font0
  if (distance < 5000) {
    canvas1.print(int(distance));
    canvas1.print("m");
  } else if (distance < 100000) {
    canvas1.print(distance / 1000);
    canvas1.print("Km");

  } else {
    canvas1.print(int(distance / 1000));
    canvas1.print("Km");
  }
  // wp info
  canvas1.setCursor(0, 43);
  canvas1.setTextColor(1, 0);
  canvas1.setFont(&fonts::Font0);  // &fonts::Font0
  canvas1.println(tmp.substring(0, 16));
  canvas1.setFont(&fonts::FreeMono12pt7b);  // &fonts::Font0
  // WP distance
  canvas1.setCursor(0, 58);
  canvas1.setTextColor(3, 0);
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

/*
draw a mini compass with the direction of the next 5 points (average)
*/

void print_go_tbt() {
  float distance_next_point = distance_tbt[points_tbt[0] + 1].dist;
  //int azimut = distance_tbt[points_tbt[0] + 1].azimuth;
  int azimut = get_average(points_tbt[0] + 1, 10).azimuth;
  offset_m off;
  String tmp;
  tmp = waypoints[wp_pos].name;

  int x, y, x1, y1, x2, y2, dx, dy;
  int to_azimut = azimut - movement.azimuth;
  canvas1.setColorDepth(2);
  canvas1.createSprite(100, 100);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_GREEN);

  canvas2.pushRotateZoom(50, 50, -movement.azimuth, 0.5, 0.5);

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

void print_tbt_info() {
  offset_m off;
  String tmp;
  int percent_tbt = int(100.0 * float(points_tbt[0]) / float(tbt_track_pos));
  float distance_point = distance_tbt[points_tbt[0] + 1].dist;

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
  canvas1.print("Percent:");
  canvas1.setCursor(0, 18);
  canvas1.setTextColor(3, 0);
  canvas1.setFont(&fonts::FreeMono12pt7b);  // &fonts::Font0
  canvas1.print(percent_tbt);
  canvas1.print("%");

  //
  canvas1.setCursor(0, 43);
  canvas1.setTextColor(1, 0);
  canvas1.setFont(&fonts::FreeMono9pt7b);  // &fonts::Font0
  canvas1.println("Dist next:");
  canvas1.setFont(&fonts::FreeMono12pt7b);  // &fonts::Font0
  //
  canvas1.setCursor(0, 58);
  canvas1.setTextColor(3, 0);
  if (distance_point < 5000) {
    canvas1.print(int(distance_point));
    canvas1.print("m");
  } else if (distance_point < 10000) {
    canvas1.print(distance_point / 1000);
    canvas1.print("Km");

  } else {
    canvas1.print(int(distance_point / 1000));
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



void list_points() {
  int i, j, i_initial, i_max;
  struct points* tmp_point;

  offset_m off;

  CoreS3.Display.fillRect(0, 0, 320, 240, BLACK);
  CoreS3.Display.drawLine(0, 30, 320, 30, RED);
  CoreS3.Display.drawLine(220, 0, 220, 240, RED);
  CoreS3.Display.setCursor(10, 18);
  CoreS3.Display.setTextColor(GREEN);
  CoreS3.Display.print(groups[menu_x].name);
  CoreS3.Display.setCursor(225, 18);
  CoreS3.Display.print("Details");
  //CoreS3.Display.setCursor(40,i*20+50);

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
      CoreS3.Display.setTextColor(GREEN);
      CoreS3.Display.drawXBitmap(10, (i - i_initial) * 18 + 45, symbols_bitmap[j], 10, 10, GREEN, BLACK);
    } else {
      CoreS3.Display.setTextColor(WHITE);
      CoreS3.Display.drawXBitmap(10, (i - i_initial) * 18 + 45, symbols_bitmap[j], 10, 10, ORANGE, BLACK);
    }

    CoreS3.Display.setCursor(30, (i - i_initial) * 18 + 50);
    CoreS3.Display.print(tmp_point[i].name);
  }
  CoreS3.Display.setTextColor(WHITE);
  CoreS3.Display.setCursor(225, 48);
  CoreS3.Display.print("Latitude:");
  CoreS3.Display.setCursor(225, 66);
  CoreS3.Display.setTextColor(RED);
  CoreS3.Display.print(tmp_point[*point_select].lat, 6);
  CoreS3.Display.setCursor(225, 85);
  CoreS3.Display.setTextColor(WHITE);
  CoreS3.Display.print("Longitude:");
  CoreS3.Display.setCursor(225, 103);
  CoreS3.Display.setTextColor(RED);
  CoreS3.Display.print(tmp_point[*point_select].lon, 6);
  CoreS3.Display.setCursor(225, 122);
  CoreS3.Display.setTextColor(WHITE);
  CoreS3.Display.print("Altitude:");
  CoreS3.Display.setCursor(225, 140);
  CoreS3.Display.setTextColor(RED);
  CoreS3.Display.print(tmp_point[*point_select].high, 0);
  CoreS3.Display.print(" M");
  CoreS3.Display.setCursor(225, 159);
  CoreS3.Display.setTextColor(WHITE);
  CoreS3.Display.print("Type:");
  CoreS3.Display.setCursor(225, 177);
  CoreS3.Display.setTextColor(RED);
  j = tmp_point[*point_select].type;
  CoreS3.Display.print(wp_type[j]);


  CoreS3.Display.setTextColor(WHITE);
  CoreS3.Display.setCursor(225, 196);
  CoreS3.Display.print("Distance:");
  CoreS3.Display.setCursor(225, 214);
  CoreS3.Display.setTextColor(RED);
  if (off.dist < 5000) {
    CoreS3.Display.print(off.dist);
    CoreS3.Display.print(" M");
  } else if (off.dist < 100000) {
    CoreS3.Display.print(off.dist / 1000);
    CoreS3.Display.print(" Km");

  } else {
    CoreS3.Display.print(int(off.dist / 1000));
    CoreS3.Display.print(" Km");
  }
  CoreS3.Display.setTextColor(WHITE);
}





void print_sort_wp() {
  short i, j, p;
  canvas1.setColorDepth(4);
  canvas1.createSprite(320, 205);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_BLUE);
  canvas1.setPaletteColor(4, TFT_GREEN);
  canvas1.setPaletteColor(5, TFT_ORANGE);
  canvas1.setPaletteColor(6, TFT_RED);
  canvas1.setPaletteColor(7, TFT_BLUE);
  canvas1.setFont(&fonts::FreeMono9pt7b);
  canvas1.setColor(0);
  canvas1.fillRect(0, 0, 320, 205);
  canvas1.setColor(2);
  canvas1.drawLine(0, 1, 320, 1);
  canvas1.drawLine(0, 20, 320, 20);
  canvas1.drawLine(220, 0, 220, 205);
  canvas1.setCursor(3, 3);
  canvas1.setTextColor(4);
  canvas1.print(groups[menu_x].name);
  canvas1.setCursor(225, 3);
  canvas1.print("Details");
  for (i = 0; i < wp_t; i++) {
    p = wp_distance_p[i];
    j = waypoints[p].type;
    if (i > 8) { break; };
    canvas1.setTextColor(1);
    canvas1.drawXBitmap(3, i * 20 + 25, symbols_bitmap[j], 10, 10, 5, 0);

    canvas1.setCursor(17, i * 20 + 25);
    canvas1.print(waypoints[p].name);
    canvas1.setCursor(225, i * 20 + 25);
    if (wp_distance[p].dist < 1000) {
      canvas1.print(wp_distance[p].dist);
      canvas1.print(" M");
    } else if (wp_distance[p].dist < 100000) {
      canvas1.print(wp_distance[p].dist / 1000);
      canvas1.print(" Km");

    } else {
      canvas1.print(int(wp_distance[p].dist / 1000));
      canvas1.print(" Km");
    }
  }


  canvas1.pushSprite(0, 20);
  canvas1.deleteSprite();
}








void settings() {
  int i, j, i_initial, i_max;
  struct points* tmp_point;

  offset_m off;
  //CoreS3.Display.setFont(&fonts::FreeMono9pt7b);
   //CoreS3.Display.setFreeFont(FSB9);
  CoreS3.Display.fillRect(0, 0, 320, 240, BLACK);
  CoreS3.Display.drawLine(0, 30, 320, 30, RED);
  CoreS3.Display.drawLine(220, 0, 220, 240, RED);
  CoreS3.Display.setCursor(10, 18);
  CoreS3.Display.setTextColor(GREEN);
  CoreS3.Display.print("Function");
  CoreS3.Display.setCursor(225, 18);
  CoreS3.Display.print("Value");
  //CoreS3.Display.setCursor(40,i*20+50);

  int point_select = pref_y;

  i_initial = 0;

  //i_max = sizeof(preference) / sizeof(short);
  i_max = num_pref;
  for (i = i_initial; i < i_max; i++) {


    if (i == point_select) {
      CoreS3.Display.setTextColor(GREEN);
      CoreS3.Display.drawXBitmap(10, (i - i_initial) * 18 + 45, symbols_bitmap[9], 10, 10, GREEN, BLACK);
    } else {
      CoreS3.Display.setTextColor(WHITE);
      CoreS3.Display.drawXBitmap(10, (i - i_initial) * 18 + 45, symbols_bitmap[9], 10, 10, ORANGE, BLACK);
    }

    CoreS3.Display.setCursor(30, (i - i_initial) * 18 + 50);
    CoreS3.Display.print(preferences[i].label);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setCursor(230, ((i - i_initial) * 18) + 50);
    if (preferences[i].max == 1) {
      if (i == point_select) {
        CoreS3.Display.setTextColor(GREEN);

        if (preference_values[i] == 0) {
          CoreS3.Display.setTextColor(RED);
          CoreS3.Display.print("Off");
        } else {
          CoreS3.Display.setTextColor(GREEN);
          CoreS3.Display.print("On");
        }

      } else {
        if (preference_values[i] == 0) {

          CoreS3.Display.print("Off");
        } else {
          CoreS3.Display.print("On");
        }
      }
    } else {
      CoreS3.Display.setTextColor(GREEN);
      CoreS3.Display.print(preference_values[i]);
    }
  }
}

void print_wp(int m) {

  grid.setColorDepth(4);
  grid.createSprite(m, m);
  grid.setPaletteColor(0, TFT_BLACK);
  grid.setPaletteColor(1, TFT_WHITE);
  grid.setPaletteColor(2, TFT_RED);
  grid.setPaletteColor(3, TFT_BLUE);
  grid.setPaletteColor(4, TFT_GREEN);
  grid.setPaletteColor(5, TFT_ORANGE);
  grid.setPaletteColor(6, TFT_DARKGREEN);
  grid.setPaletteColor(7, TFT_YELLOW);
  // grid.clear(TFT_GREEN);
  grid.setColor(0);
  grid.clear(TFT_BLACK);
  short flag = 0;
  int x, y, i, j, old_x, old_y, old_j;
  offset_m off;
  for (i = 0; i < wp_t; i++) {
    off = calc_movement(waypoints[i], current);
    x = (m / 2) - int((off.x / zoom[c_zoom])) + display_offset.x;
    y = (m / 2) + int((off.y / zoom[c_zoom])) + display_offset.y;
    if ((x >= 0) && (x <= m) && (y >= 0) && (y <= m)) {
      if (i == wp_pos) {
        j = waypoints[i].type;
        old_x = x;
        old_y = y;
        old_j = j;
        flag = 1;
        grid.drawXBitmap(x, y, symbols_bitmap[j], 10, 10, 6, 0);
        // M5.Lcd.fillCircle(x, y , 7, RED);

      } else {
        j = waypoints[i].type;
        grid.drawXBitmap(x, y, symbols_bitmap[j], 10, 10, 5, 0);
      }
    }
  }
  // print again selected waypoint to ensure is on front
  if (flag == 1) {
    grid.drawXBitmap(old_x, old_y, symbols_bitmap[old_j], 10, 10, 6, 0);
  }
}




void prepare_grid(int m) {
  int i;
  //int m = 300;
  grid.setColorDepth(4);
  grid.createSprite(m, m);
  grid.setPaletteColor(0, TFT_BLACK);
  grid.setPaletteColor(1, TFT_WHITE);
  grid.setPaletteColor(2, TFT_RED);
  grid.setPaletteColor(3, TFT_BLUE);
  grid.setPaletteColor(4, TFT_GREEN);
  grid.setPaletteColor(5, TFT_ORANGE);
  grid.setPaletteColor(6, TFT_DARKGREEN);
  grid.setPaletteColor(7, TFT_YELLOW);
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
  canvas1.setColorDepth(4);
  canvas1.createSprite(width, height);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_BLUE);
  canvas1.setPaletteColor(4, TFT_GREEN);
  canvas1.setPaletteColor(5, TFT_ORANGE);
  canvas1.setPaletteColor(6, TFT_DARKGREEN);
  canvas1.setPaletteColor(7, TFT_YELLOW);
  //grid.setColor(0);
  //grid.fillRect(0, 0, width, height);
  //canvas1.clear(TFT_BLACK);

  off = calc_movement(current, track[track_pos]);
  x = -int((off.x / zoom[c_zoom])) + display_offset.x;
  y = int((off.y / zoom[c_zoom])) + display_offset.y;
  grid.setColor(2);
  grid.fillCircle((m / 2) + x, (m / 2) + y, 4);
  if ((overflow_track) && (track_pos < maxpoints - 2)) {
    for (i = track_pos + 1; i < maxpoints; i++) {
      off = calc_movement(track[i], track[track_pos]);
      /*
  GPS maps and coordinates are cartesian 
  Y^
   |
   |
   |
   +--------->X
  Display has Y inverted 
  +---------->X
  |
  |
  |
 Yv
  */
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
    off = calc_movement(track[i], track[track_pos]);
    x = m / 2 - int((off.x / zoom[c_zoom])) + display_offset.x;
    y = m / 2 + int((off.y / zoom[c_zoom])) + display_offset.y;
    if ((x > 0) && (x < m) && (y > 0) && (y < m)) {
      grid.setColor(3);
      grid.fillCircle(x, y, 2);
      if (i > 0) {
        grid.setColor(2);
        grid.drawLine(x, y, old_x, old_y);
      }
    }
    old_x = x;
    old_y = y;
  }
  // print turn by turn data
  if (preference_values[7] > 0) {
    //Serial.println(points_tbt[0]);
    for (i = 0; i <= tbt_track_pos; i++) {
      //  Serial.print(distance_tbt[points_tbt[i]].dist);
      //  Serial.print("|");
      off = calc_movement(track_tbt[i], track[track_pos]);
      x = m / 2 - int((off.x / zoom[c_zoom])) + display_offset.x;
      y = m / 2 + int((off.y / zoom[c_zoom])) + display_offset.y;
      if ((x > 0) && (x < m) && (y > 0) && (y < m)) {
        if (i < points_tbt[1]) {
          grid.setColor(6);
        } else {
          grid.setColor(7);
        }
        if (i == points_tbt[0] + 1) {
          grid.setColor(5);
          grid.fillCircle(x, y, 5);
        } else {
          grid.fillCircle(x, y, 2);
        }
        if (i > 0) {
          grid.setColor(2);
          grid.drawLine(x, y, old_x, old_y);
        }
      }
      old_x = x;
      old_y = y;
    }
  }

  // end tbt

  grid.pushRotateZoom(width / 2, height / 2, -movement.azimuth, 1, 1, TFT_BLACK);
  grid.deleteSprite();

  print_wp(m);
  grid.pushRotateZoom(width / 2, height / 2, -movement.azimuth, 1, 1, TFT_BLACK);
  canvas1.pushSprite(xs, ys);
  canvas1.deleteSprite();
  grid.deleteSprite();
}


/* Draw the grip and stored points */
void print_altitude(int xs, int ys, int width, int height) {
  int x, y, i, j, min_index, old_x, old_y;
  int max_alt, min_alt, avg_alt;
  float m;
  offset_m off;

  canvas1.setColorDepth(2);
  canvas1.createSprite(width, height);
  canvas1.setPaletteColor(0, TFT_BLACK);
  canvas1.setPaletteColor(1, TFT_WHITE);
  canvas1.setPaletteColor(2, TFT_RED);
  canvas1.setPaletteColor(3, TFT_GREEN);
  // canvas1.clear(TFT_GREEN);
  canvas1.fillSprite(TFT_BLACK);
  canvas1.fillRect(0, int(height / 2), width, int(height / 2), 1);
  for (j = 4; j < 9; j++) {
    canvas1.drawFastHLine(0, int(j * height / 8), width, 0);
  }
  for (j = 0; j < 9; j++) {
    canvas1.drawFastVLine(int(j * width / 8), int(height / 2), height, 0);
  }

  if (track_pos > -1) {
    if (track_pos < width) {
      min_index = 0;
    } else {
      min_index = track_pos - width;
    }
    x = 0;
    max_alt = 0;
    min_alt = 2000;
    avg_alt = 0;
    if ((overflow_track) && (track_pos < width)) {
      j = 140 + track_pos;  // 360 - ( 220 - track_pos)
      for (i = j; i < maxpoints; i++) {
        x++;
        m = track[i].high;
        if (m > 2000) { m = 2000; };
        y = height - int(((height / 2) * m) / 2000.0);
        if (m > max_alt) { max_alt = int(m); };
        if (m < min_alt) { min_alt = int(m); };
        avg_alt += int(m);
        //  canvas1.fillCircle(x, y, 2, 3);
        canvas1.drawFastVLine(x, y, height - y, 2);
      }
    }
    for (i = min_index; i <= track_pos; i++) {
      x++;
      m = track[i].high;
      if (m > 2000) { m = 2000; };
      y = height - int(((height / 2) * m) / 2000.0);
      if (m > max_alt) { max_alt = int(m); };
      if (m < min_alt) { min_alt = int(m); };
      avg_alt += int(m);
      //  canvas1.fillCircle(x, y, 2, 3);
      canvas1.drawFastVLine(x, y, height - y, 2);
    }
    canvas1.setTextColor(1, 0);
    canvas1.setTextSize(1);
    canvas1.setFont(&fonts::FreeMono9pt7b);  // &fonts::Font0
    canvas1.setCursor(0, 10);
    canvas1.print("Current: ");
    canvas1.setCursor(0, 28);
    canvas1.setTextColor(3, 0);
    //canvas1.setTextSize(2);
    canvas1.setFont(&fonts::FreeMono18pt7b);  // &fonts::Font0
    canvas1.print(int(current.high));
    canvas1.setFont(&fonts::FreeMono9pt7b);  // &fonts::Font0
    canvas1.print("M");

    canvas1.setTextColor(1, 0);
    canvas1.setTextSize(1);
    canvas1.setCursor(int(width / 2), 10);
    canvas1.print("Max:");
    //canvas1.setCursor(int(width/2), 18);
    canvas1.setTextColor(3, 0);
    canvas1.setTextSize(1);
    canvas1.print(int(max_alt));
    canvas1.print("M");
    canvas1.setTextColor(1, 0);
    canvas1.setCursor(int(width / 2), 30);
    canvas1.print("Avg:");
    //canvas1.setCursor(int(width/2), 53);
    canvas1.setTextColor(3, 0);
    canvas1.setTextSize(1);
    canvas1.print(int(avg_alt / x));
    canvas1.print("M");
    canvas1.setTextColor(1, 0);
    canvas1.setCursor(int(width / 2), 50);
    canvas1.print("Min:");
    // canvas1.setCursor(int(width/2), 88);
    canvas1.setTextColor(3, 0);
    canvas1.setTextSize(1);
    canvas1.print(int(min_alt));
    canvas1.print("M");
  } else {
    canvas1.setTextColor(1, 0);
    canvas1.setTextSize(2);
    canvas1.setCursor(0, 10);
    canvas1.print("No data");
  }
  canvas1.pushSprite(xs, ys);
  canvas1.deleteSprite();
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
/*
void print_to_wp(float dist) {
  M5.Lcd.fillRect(220, 111, 320, 129, TFT_LIGHTGREY);
  M5.Lcd.setCursor(225, 125);
  M5.Lcd.print("WP: ");
  M5.Lcd.print(dist);
}
*/

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
  if (page == 10) {
    if (WiFi.getMode() == WIFI_MODE_AP) {
      canvas1.print(WiFi.softAPSSID());
    } else {
      canvas1.print(WiFi.SSID());
    }
  } else {
    canvas1.print(menu_a[page]);
  }
  canvas1.setCursor(130, 0);
  if (page == 10) {
    switch (WiFi.getMode()) {
      case 0:
        canvas1.print("Not connected");
        break;
      case 1:
        canvas1.print("Client");
        break;
      case 2:
        canvas1.print("AP");
        break;
      default:
        canvas1.print("-");
        break;
    }
  } else {
    canvas1.print(menu_b[page]);
  }
  canvas1.setCursor(230, 0);
  if (page == 10) {
    canvas1.setCursor(200, 0);
    switch (WiFi.getMode()) {
      case 0:
        break;
      case 1:
        canvas1.print(WiFi.localIP());
        break;
      case 2:
        canvas1.print(WiFi.softAPIP());
        break;
      default:
        canvas1.print("-");
        break;
    }
  } else {
    canvas1.setCursor(230, 0);
    canvas1.print(menu_c[page]);
  }
  canvas1.pushSprite(0, 225);

  canvas1.deleteSprite();
}
