void print_wp_menu() {
  display.fillRect(0, 225, 320, 240, 0xC618);
  display.setTextSize(0);
  display.setTextFont(1);
  display.setTextColor(BLACK);
  display.setCursor(30, 228);
  display.print(menu_a[page]);
  display.setCursor(130, 228);
  display.print(menu_b[page]);
  display.setCursor(230, 228);
  display.print(menu_c[page]);
  display.setTextSize(0);
 // display.setFreeFont(FSB9);
  display.setTextColor(WHITE);
}

void print_position() {
  int x = 60;
  int y = 70;

  display.fillRect(50, 70, 220, 100, 0xC618);

  display.setCursor(x + 5, y + 20);
  display.print("Altitude: ");
  display.print(int(current.high));
  display.print("M");
  display.setCursor(x + 5, y + 40);
  display.print("Latitude: ");
  display.print(current.lat, 6);
  display.setCursor(x + 5, y + 60);
  display.print("Longitude: ");
  display.print(current.lon, 6);




}


void rename_wp() {

  int x = 60;
  int y = 70;
  String pre_c, post_c;
  char middle_c;

  pre_c = tmp_name.substring(0, char_position);
  middle_c = tmp_name.charAt(char_position);
  if (middle_c == ' ') {
    middle_c = '_' ;
  };
  post_c = tmp_name.substring(char_position + 1, tmp_name.length());

  display.fillRect(50, 70, 220, 100, 0xC618);


  display.setCursor(x + 5, y + 20);
  display.print("New Name for WP: ");
  display.print(waypoints[wp_pos].id);
  display.setCursor(x + 5, y + 60);
  display.setTextColor(WHITE);
  display.print(pre_c);
  display.setTextColor(RED);
  display.print(middle_c);
  display.setTextColor(WHITE);
  display.print(post_c);



}

void delete_wp() {

  int x = 60;
  int y = 70;
  String pre_c, post_c;
  char middle_c;

  pre_c = tmp_name.substring(0, char_position);
  middle_c = tmp_name.charAt(char_position);
  if (middle_c == ' ') {
    middle_c = '_' ;
  };
  post_c = tmp_name.substring(char_position + 1, tmp_name.length());

  display.fillRect(50, 70, 220, 100, 0xC618);


  display.setCursor(x + 5, y + 20);
  display.print("Confirm to delete WP: ");
  display.print(waypoints[wp_pos].id);
  display.setCursor(x + 5, y + 60);
  display.setTextColor(RED);
  display.print(waypoints[wp_pos].name);
  display.setTextColor(WHITE);


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
    middle_c = '_' ;
  };
  post_c = tmp_name.substring(char_position + 1, tmp_name.length());

  display.fillRect(x, y, 220, 180, 0xC618);

  display.setTextColor(RED);
  display.setCursor(x + 5, y + 20);
  display.print("Select new group for WP: ");
  display.setTextColor(WHITE);
  if (( groups_t > 7 ) && (tmp_group > 4)) {
    initial_i = tmp_group - 3;
  }
  else {
    initial_i = 0;
  }
  if (initial_i + 7 > groups_t ) {
    max_i = groups_t;
  } else {
    max_i = initial_i + 7;
  }
  
  for (i = initial_i; i < max_i ; i++) {
    if ( tmp_group == i) {
      display.setTextColor(GREEN);
    }
    else {
      display.setTextColor(WHITE);
    }

    display.setCursor(x + 5, y + 40 + (i - initial_i ) * 20);
    display.print( groups[i].name );
  }


}
