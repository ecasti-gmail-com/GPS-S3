void stop_monitor() {

  if (preference_values[1] == 0) {
    setCpuFrequencyMhz(20);
  } else {
    setCpuFrequencyMhz(80);
  }
  CoreS3.Lcd.setBrightness(0);
  CoreS3.Lcd.powerSaveOn();

  
  //}
}

void start_monitor() {
  CoreS3.Lcd.powerSaveOff();
  CoreS3.Lcd.setBrightness(128);
  if (preference_values[1] == 0) {
    setCpuFrequencyMhz(160);
  } else {
    setCpuFrequencyMhz(240);
  }
}
/*
Calculate the distance from all waypoints

*/
void calc_distance() {
  long t1, t2;
  for (short i = 0; i < wp_t; i++) {
    wp_distance[i] = calc_movement(waypoints[i], current);
  }
}

void calc_distance_track() {
  long t1, t2;

  for (short i = 0; i < tbt_track_pos; i++) {
    distance_tbt[i] = calc_movement(track_tbt[i], current);
  }
}

void reset_track_distance() {
  for (short j = 0; j < tbt_track_pos; j++) {
    points_tbt[j] = j;
  }
}
void reset_wp_distance() {
  for (short j = 0; j < wp_t; j++) {
    wp_distance_p[j] = j;
  }
}
/*
calculate average direction of the next n points for the turn by turn track
*/
offset_m get_average(short sp, short n) {
  struct points avg_tbr;
  struct offset_m avg_dist;
  avg_tbr.lat = 0;
  avg_tbr.lon = 0;
  for (int i = sp; i < sp + n; i++) {
    avg_tbr.lat += track_tbt[i].lat;
    avg_tbr.lon += track_tbt[i].lon;
  }
  avg_tbr.lat /= n;
  avg_tbr.lon /= n;
  avg_dist = calc_movement(avg_tbr, current);
  return avg_dist;
}

/* sort the pointers to the track points according to the distance from current location.
As the order is not supposed to change frequently, a simple bubblesort with stop condition is enought 
Will complete most of the runs with complexity = N
*/
void sort_distance_track() {
  bool sorted = true;
  short tmp;
  // initially plan for n*n loops
  for (short j = 0; j < tbt_track_pos; j++) {
    for (short i = 0; i < tbt_track_pos - 1; i++) {
      if (distance_tbt[points_tbt[i]].dist > distance_tbt[points_tbt[i + 1]].dist) {
        sorted = false;
        tmp = points_tbt[i];
        points_tbt[i] = points_tbt[i + 1];
        points_tbt[i + 1] = tmp;
      }
    }
    if (sorted) { break; };
  }
}
/* 
Sort the pointers to the waypoint according to the distance from current location.
As the order is not supposed to change frequently, a simple bubblesort with stop condition is enought 
Will complete most of the runs with complexity = N
*/
void sort_distance() {
  bool sorted = true;
  short tmp;
  // initially plan for n*n loops
  for (short j = 0; j < wp_t; j++) {
    for (short i = 0; i < wp_t - 1; i++) {
      if (wp_distance[wp_distance_p[i]].dist > wp_distance[wp_distance_p[i + 1]].dist) {
        sorted = false;
        tmp = wp_distance_p[i];
        wp_distance_p[i] = wp_distance_p[i + 1];
        wp_distance_p[i + 1] = tmp;
      }
    }
    if (sorted) { break; };
  }
}

void button_top_a() {
  switch (page) {
    case 0:  // Gps Stat
      page++;
      break;
    case 1:  //track
      page = 9;
      break;
    case 2:  // Compass
      bg_page = page;
      page = 5;
      break;
    case 3:  // Go Waypoint
      bg_page = page;
      page = 5;
      break;
    case 4:  // Waypoints
      tmp_name = waypoints[wp_pos].name;
      page = 6;
      break;
    case 5:  //
      page = bg_page;
      bg_page = 0;
      break;
    case 6:  //
      rename_wp(waypoints[wp_pos].id, tmp_name);
      read_waypoints();
      page = 4;
      break;
    case 7:
      delete_wp(waypoints[wp_pos].id);
      read_waypoints();
      page = 4;
      break;
    case 8:
      move_wp(waypoints[wp_pos].id, tmp_group);
      menu_x = tmp_group;
      read_waypoints();
      page = 4;
      break;
    case 9:
      page = 1;
      break;

    default:
      break;
  }
}


void button_top_b() {
  switch (page) {
    case 0:  // Gps Stat
      page = 10;
      break;
    case 1:  //track
      c_zoom--;
      display_offset.x *= 2;
      display_offset.y *= 2;
      if (c_zoom < 0) {
        c_zoom = 0;
        display_offset.x /= 2;
        display_offset.y /= 2;
      };
      break;
    case 2:  // Compass

      break;
    case 3:  // Go Waypoint

      break;
    case 4:  // Waypoints
      tmp_group = menu_x;
      page = 8;
      break;
    case 5:  // ?

      break;
    case 6:  //Rename WP
      tmp_name = "Aaaaaaaaaaaaa";
      break;
    case 9:  //track
      c_zoom--;
      display_offset.x *= 2;
      display_offset.y *= 2;
      if (c_zoom < 0) {
        c_zoom = 0;
        display_offset.x /= 2;
        display_offset.y /= 2;
      };
      break;
    default:
      break;
  }
}

void button_top_c() {
  switch (page) {
    case 0:  // Gps Stat
      page = 2;
      break;
    case 1:  //track
      c_zoom++;
      display_offset.x /= 2;
      display_offset.y /= 2;
      if (c_zoom > 13) {
        c_zoom = 13;
        display_offset.x *= 2;
        display_offset.y *= 2;
      };
      break;
    case 2:  // Compass
      page = 0;
      break;
    case 3:  // Go Waypoint
      page = 0;
      break;
    case 4:  // Waypoints
      page = 7;
      break;
    case 5:  //Navigate ?

      break;
    case 6:  //Rename wp
      page = 4;
      break;
    case 7:
      page = 4;
      break;
    case 8:

      page = 4;
      break;
    case 9:  //track
      c_zoom++;
      display_offset.x /= 2;
      display_offset.y /= 2;
      if (c_zoom > 13) {
        c_zoom = 13;
        display_offset.x *= 2;
        display_offset.y *= 2;
      };
      break;
    default:
      break;
  }
}




void button_select() {
  if (disp_on == 0) {
    start_monitor();

    disp_on = 1;
  } else {
    stop_monitor();

    disp_on = 0;
  }
  switch (page) {
    case 0:  // Gps Stat
      break;
    case 1:  //track
      break;
    case 2:  // Compass
      break;
    case 3:  // Go Waypoint
      break;
    case 4:  // Waypoints
      break;
    case 5:  //Navigate ?
      break;
    default:
      break;
  }
}

void button_navigation() {
  switch (page) {
    case 0:  // Gps Stat
      page = 1;
      break;
    case 1:  //track
      page = 3;
      break;
    case 2:  // Compass
      page = 1;
      break;
    case 3:  // Go Waypoint
      page = 11;
      break;
    case 4:  // Waypoints
      page = 1;
      break;
    case 5:  //Position info
      page = 1;
      break;
    case 9:  // altitude
      page = 1;
      break;
    case 11:  // altitude
      page = 1;
      break;
    default:
      page = 1;
      break;
  }
}

void button_wplist() {
  switch (page) {
    case 0:  // Gps Stat
      page = 4;
      break;
    case 1:  //track
      page = 4;
      break;
    case 2:  // Compass
      page = 4;
      break;
    case 3:  // Go Waypoint
      page = 4;
      break;
    case 4:  // Waypoints
      page = 4;
      break;
    case 5:  //Position info
      page = 4;
      break;
    default:
      page = 4;
      break;
  }
}

void button_a() {
  switch (page) {
    case 0:  // Gps Stat
      // mark_point();
      store_wp(current, 2, 0);
      break;
    case 1:  //track
      // mark_point();
      store_wp(current, 2, 0);
      break;
    case 2:  // Compass
      //mark_point();
      store_wp(current, 2, 0);
      break;
    case 3:  // Go Waypoint
      //mark_point();
      store_wp(current, 2, 0);
      break;
    case 4:  // Waypoints
      break;
    case 5:  //Navigate ?
      break;
    default:
      break;
  }
}

void button_up() {
  char middle_c;
  switch (page) {
    case 0:  // Gps Stat
      break;
    case 1:  //track
      display_offset.y += 10;
      break;
    case 2:  // Compass
      break;
    case 3:  // Go Waypoint
      display_offset.y += 10;
      break;
    case 4:  // Waypoints
      *point_select = *point_select - 1;
      if (*point_select < 0) {
        *point_select = 0;
      };
      break;
    case 5:  //Navigate ?
      break;
    case 6:  // ?
      middle_c = tmp_name.charAt(char_position) + 1;
      if (middle_c > 122) {
        middle_c = 122;
      };
      if (middle_c < 32) {
        middle_c = 32;
      };
      tmp_name.setCharAt(char_position, middle_c);
    case 7:
      break;
    case 8:
      if (tmp_group > 0) tmp_group--;
      break;
    case 10:
      if (pref_y > 0) pref_y--;
      break;
    default:
      break;
  }
}

void button_down() {
  char middle_c;
  switch (page) {
    case 0:  // Gps Stat
      break;
    case 1:  //track
      display_offset.y -= 10;
      break;
    case 2:  // Compass
      break;
    case 3:  // Go Waypoint
      display_offset.y -= 10;
      break;
    case 4:  // Waypoints
      *point_select = *point_select + 1;
      Serial.println(*point_select);
      if (*point_select > *point_max) {
        *point_select = *point_max;
      };
      break;
    case 5:  // ?
      break;
    case 6:  // ?
      middle_c = tmp_name.charAt(char_position) - 1;
      if (middle_c < 32) {
        middle_c = 32;
      };
      tmp_name.setCharAt(char_position, middle_c);
      break;
    case 7:
      break;
    case 8:
      if (tmp_group < groups_t - 1) tmp_group++;
      break;
    case 10:
      if (pref_y < num_pref - 1) pref_y++;
      break;
    default:
      break;
  }
}

void button_left() {
  switch (page) {
    case 0:  // Gps Stat
      break;
    case 1:  //track
      display_offset.x += 10;
      break;
    case 2:  // Compass
      break;
    case 3:  // Go Waypoint
      display_offset.x += 10;
      break;
    case 4:  // Waypoints
      menu_x--;
      if (menu_x < 0) {
        menu_x = 0;
      };
      read_waypoints();
      break;
    case 5:  // ?
      break;
    case 6:  // ?

      char_position--;
      if (char_position < 0) {
        char_position = 0;
      };
      break;
    case 10:
      preference_values[pref_y]--;
      if (preference_values[pref_y] < preferences[pref_y].min) {
        preference_values[pref_y] = preferences[pref_y].min;
      }
      if (pref_y == 4) {
        read_track(preference_values[pref_y]);
      }
      if (pref_y == 7) {
        loaded_tbt = false;
      }
      store_preferences();
      break;
    case 11:
      menu_x--;
      if (menu_x < 0) {
        menu_x = 0;
      };
      read_waypoints();
      break;
    default:
      break;
  }
}

void button_right() {
  switch (page) {
    case 0:  // Gps Stat
      break;
    case 1:  //track
      display_offset.x -= 10;
      break;
    case 2:  // Compass
      break;
    case 3:  // Go Waypoint
      display_offset.x -= 10;
      break;
    case 4:  // Waypoints
      menu_x++;
      if (menu_x >= groups_t) {
        menu_x = groups_t - 1;
      };
      read_waypoints();
      break;
    case 5:  //Navigate ?
      break;
    case 6:  // ?
      char_position++;
      if (char_position < 0) {
        char_position = 0;
      };
      break;
    case 10:
      if (preferences[pref_y].max == 1) {
        if (preference_values[pref_y] == 0) {
          preference_values[pref_y] = 1;
        } else {
          preference_values[pref_y] = 0;
        }
      } else {
        preference_values[pref_y]++;
        if (preference_values[pref_y] > preferences[pref_y].max) {
          preference_values[pref_y] = preferences[pref_y].max;
        }
      }
      if (pref_y == 4) {
        read_track(preference_values[pref_y]);
      }
      if (pref_y == 7) {
        loaded_tbt = false;
      }
      store_preferences();
      break;
    case 11:
      menu_x++;
      if (menu_x >= groups_t) {
        menu_x = groups_t - 1;
      };
      read_waypoints();
      break;
    default:
      break;
  }
}

void button_m_reset() {

  switch (page) {
    case 1:  //track
      display_offset.x = 0;
      display_offset.y = 0;

      break;
    case 2:  // Compass
      display_offset.x = 0;
      display_offset.y = 0;
      break;
    case 3:  // Compass
      display_offset.x = 0;
      display_offset.y = 0;
      break;
    case 10:  // Compass
      preference_values[pref_y] = 0;
      break;
    default:
      display_offset.x = 0;
      display_offset.y = 0;
      break;
  }
}
void button_setting() {
  if (page == 10) {
    page = bg_page;
  } else {
    bg_page = page;
    page = 10;
  }
}

void button_info() {
  switch (page) {
    case 0:  // Gps Stat
      page = 9;
      break;
    case 1:  //track
      page = 0;
      break;
    case 2:  // Compass
      page = 5;
      break;
    case 3:  // Go Waypoint
      page = 0;
      break;
    case 4:  // Waypoints
      page = 0;
      break;
    case 5:  //Navigate ?
      page = 0;
      break;
    case 9:  // altitude
      page = 2;
      break;
    default:
      page = 0;
      break;
  }
}

offset_m calc_movement(points p2, points p1) {
  float dist, x, y, radius, circ;
  offset_m result;
  // Calculate radius from the axis of the earth to the current latitude
  radius = (6378.0 * cos(p1.lat / 57.2958));
  circ = 2 * radius * 3.1415926535;
  result.y = 1000.0 * ((p1.lat - p2.lat) * 40007) / 360.0;
  result.x = 1000.0 * ((p1.lon - p2.lon) * circ) / 360.0;
  result.dist = sqrt((result.x * result.x) + (result.y * result.y));
  if (result.dist < 10) {
    result.azimuth = 0;
  } else {
    /*    
    compass is rotate 90 degree compared to cartesian reference, this is why dx, -dy 
    */
    result.azimuth = int(atan2(-result.x, -result.y) * 57.2958);
  }
  return result;
}

void check_movement() {
  int l = mygps.fixtime.length() - 1;
  int i = mygps.fixtime[l] - '0';
  movement = calc_movement(current, lasts[i]);
  lasts[i] = current;
}
void set_current_point() {
  waypoints[wp_t].lat = current.lat;
  waypoints[wp_t].lon = current.lon;
  waypoints[wp_t].name = "Temp";
  wp_t++;
}
