
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
  canvas1.drawLine(0, 120, 220, 120);
  canvas1.setFont(&fonts::FreeSans9pt7b);
  canvas1.setCursor(5, 15);
  canvas1.setTextColor(3,0);
  canvas1.print("Informations: ");
  canvas1.setCursor(5, 40);
  canvas1.print("Altitude: ");
  canvas1.setTextColor(1,0);
  canvas1.print(int(current.high));
  canvas1.print("M");
  canvas1.setCursor(5, 65);
  canvas1.setTextColor(3,0);
  canvas1.print("Latitude: ");
  canvas1.setTextColor(1,0);
  canvas1.print(current.lat, 6);
  canvas1.setCursor(5, 90);
  canvas1.setTextColor(3,0);
  canvas1.print("Longitude: ");
  canvas1.setTextColor(1,0);
  canvas1.print(current.lon, 6);
  canvas1.setCursor(5, 130);
  canvas1.setTextColor(3,0);
  canvas1.print("Waypoint: ");
  canvas1.setCursor(5, 155);
  canvas1.setTextColor(1,0);
  canvas1.print(waypoints[wp_pos].name);
  canvas1.pushSprite(x, y);
  canvas1.deleteSprite();
}


void rename_wp() {

  int x = 60;
  int y = 70;
  String pre_c, post_c;
  char middle_c;

  pre_c = tmp_name.substring(0, char_position);
  middle_c = tmp_name.charAt(char_position);
  if (middle_c == ' ') {
    middle_c = '_';
  };
  post_c = tmp_name.substring(char_position + 1, tmp_name.length());

  CoreS3.Display.fillRect(50, 70, 220, 100, 0xC618);


  CoreS3.Display.setCursor(x + 5, y + 20);
  CoreS3.Display.print("New Name for WP: ");
  CoreS3.Display.print(waypoints[wp_pos].id);
  CoreS3.Display.setCursor(x + 5, y + 60);
  CoreS3.Display.setTextColor(WHITE);
  CoreS3.Display.print(pre_c);
  CoreS3.Display.setTextColor(RED);
  CoreS3.Display.print(middle_c);
  CoreS3.Display.setTextColor(WHITE);
  CoreS3.Display.print(post_c);
}

void delete_wp() {

  int x = 60;
  int y = 70;
  String pre_c, post_c;
  char middle_c;

  pre_c = tmp_name.substring(0, char_position);
  middle_c = tmp_name.charAt(char_position);
  if (middle_c == ' ') {
    middle_c = '_';
  };
  post_c = tmp_name.substring(char_position + 1, tmp_name.length());

  CoreS3.Display.fillRect(50, 70, 220, 100, 0xC618);


  CoreS3.Display.setCursor(x + 5, y + 20);
  CoreS3.Display.print("Confirm to delete WP: ");
  CoreS3.Display.print(waypoints[wp_pos].id);
  CoreS3.Display.setCursor(x + 5, y + 60);
  CoreS3.Display.setTextColor(RED);
  CoreS3.Display.print(waypoints[wp_pos].name);
  CoreS3.Display.setTextColor(WHITE);
}

void list_groups() {

  int x = 60;
  int y = 30;
  int i, initial_i, max_i;
  String pre_c, post_c;
  char middle_c;

  pre_c = tmp_name.substring(0, char_position);
  middle_c = tmp_name.charAt(char_position);
  if (middle_c == ' ') {
    middle_c = '_';
  };
  post_c = tmp_name.substring(char_position + 1, tmp_name.length());

  CoreS3.Display.fillRect(x, y, 220, 180, 0xC618);

  CoreS3.Display.setTextColor(RED);
  CoreS3.Display.setCursor(x + 5, y + 20);
  CoreS3.Display.print("Select new group for WP: ");
  CoreS3.Display.setTextColor(WHITE);
  if ((groups_t > 7) && (tmp_group > 4)) {
    initial_i = tmp_group - 3;
  } else {
    initial_i = 0;
  }
  if (initial_i + 7 > groups_t) {
    max_i = groups_t;
  } else {
    max_i = initial_i + 7;
  }

  for (i = initial_i; i < max_i; i++) {
    if (tmp_group == i) {
      CoreS3.Display.setTextColor(GREEN);
    } else {
      CoreS3.Display.setTextColor(WHITE);
    }

    CoreS3.Display.setCursor(x + 5, y + 40 + (i - initial_i) * 20);
    CoreS3.Display.print(groups[i].name);
  }
}
