/*
      Groups:
      0: Temporary points
      1 : waypoints
      2+: Custom groups


query:   SELECT * FROM sqlite_master WHERE type='table';



*/

void sqlite_db_init() {

  SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
  if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
    Serial.println("SD card error!");
  }
  sqlite3_initialize();
  if (openDb("/sd/gps_track.db", &db1)) {
    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.print("setupSDCard PASS . SIZE = ");
    Serial.print(cardSize);
    Serial.println(" MB");
    // preference[0]=1;
  } else {
    Serial.println("No valid SD Card dedected");
    preference_values[0] = 0;
  }
}

const char *data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName) {
  int i;
  Serial.printf("%s: ", (const char *)data);
  for (i = 0; i < argc; i++) {
    Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  Serial.printf("\n");

  return 0;
}

int openDb(const char *filename, sqlite3 **db) {
  int rc = sqlite3_open(filename, db);
  if (rc) {
    Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
    return rc;
  } else {
    Serial.printf("Opened database successfully\n");
  }
  return rc;
}

char *zErrMsg = 0;
int db_exec(sqlite3 *db, const char *sql) {

  Serial.println(sql);
  long start = micros();
  int rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    Serial.printf("Operation done successfully\n");
  }
  Serial.print(F("Time taken:"));
  Serial.println(micros() - start);

  return rc;
}

void log_position() {

  char stringa[128];
  char tmpstr[18];
  mygps.rawdate.toCharArray(tmpstr, 18);
  sprintf(stringa, "Insert into track(timestamp,lat,lon,alt,trackid) values (\'%s\',%f,%f,%f,%d)", tmpstr, current.lat, current.lon, current.high, preference_values[4]);
  //Serial.println(stringa);
  rc = db_exec(db1, stringa);
  if (rc != SQLITE_OK) {
    Serial.println("Unable to store data");
    M5.Lcd.setCursor(20, 225);
    M5.Lcd.print("Unable to store data: ");
    M5.Lcd.print(rc);
    delay(200);
  }
}

void store_position() {

  track_pos++;
  if (track_pos >= maxpoints) {
    track_pos = 0;
    overflow_track = true;
  }
  offset_m tmp;
  track[track_pos].lat = current.lat;
  track[track_pos].lon = current.lon;
  track[track_pos].high = current.high;
  if (track_pos > 0) {
    tmp = calc_movement(track[track_pos], track[track_pos - 1]);
    distance += tmp.dist;
    // azimuth = tmp.azimuth;
  }
}

void store_wp(points position, int type, int groups) {

  char stringa[128];
  char tmpstr[18];
  mygps.rawdate.toCharArray(tmpstr, 18);
  sprintf(stringa, "Insert into waypoints(lat,lon,alt,name,type,groups) values (%f,%f,%f,\'%s\',%d,%d)", position.lat, position.lon, position.high, tmpstr, type, groups);
  rc = db_exec(db1, stringa);
  if (rc != SQLITE_OK) {
    Serial.println("Unable to store data");
    M5.Lcd.setCursor(20, 225);
    M5.Lcd.print("Unable to store data: ");
    M5.Lcd.print(rc);
  }
  Serial.println(stringa);
}

void rename_wp(int id, String wp_name) {

  char stringa[128];

  wp_name.trim();
  Serial.println(wp_name);
  sprintf(stringa, "Update waypoints set name = '%s\' where id = %d ", wp_name, id);
  rc = db_exec(db1, stringa);
  if (rc != SQLITE_OK) {
    Serial.println("Unable to store data");
    M5.Lcd.setCursor(20, 225);
    M5.Lcd.print("Unable to store data: ");
    M5.Lcd.print(rc);
  }
  Serial.println(stringa);
}


void move_wp(int id, int group_id) {

  char stringa[128];

  sprintf(stringa, "Update waypoints set groups = '%d\' where id = %d ", group_id, id);
  rc = db_exec(db1, stringa);
  if (rc != SQLITE_OK) {
    Serial.println("Unable to store data");
    M5.Lcd.setCursor(20, 225);
    M5.Lcd.print("Unable to store data: ");
    M5.Lcd.print(rc);
  }
  Serial.println(stringa);
}

void delete_wp(int id) {

  char stringa[128];

  sprintf(stringa, "Delete from waypoints where id = %d ", id);
  rc = db_exec(db1, stringa);
  if (rc != SQLITE_OK) {
    Serial.println("Unable to store data");
    M5.Lcd.setCursor(20, 225);
    M5.Lcd.print("Unable to store data: ");
    M5.Lcd.print(rc);
  }
  Serial.println(stringa);
}

void read_waypoints() {

  sqlite3_stmt *stmt;
  char tmpstr[64];
  char sql[128];
  wp_t = 0;
  for (int i=0;i<10;i++) {
    peerspoints[i].id = 0;
  }


  //itoa(menu_x, tmpstr, 10);
  sprintf(sql, "SELECT id,lat,lon,name,type,groups,alt from waypoints where groups = %d order by name", groups[menu_x].group_id);
  int rc = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
    return;
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    waypoints[wp_t].lat = sqlite3_column_double(stmt, 1);
    waypoints[wp_t].lon = sqlite3_column_double(stmt, 2);
    sprintf(tmpstr, "%s", sqlite3_column_text(stmt, 3));
    waypoints[wp_t].name = tmpstr;
    waypoints[wp_t].type = sqlite3_column_int(stmt, 4);
    waypoints[wp_t].groups = sqlite3_column_int(stmt, 5);
    waypoints[wp_t].id = sqlite3_column_int(stmt, 0);
    waypoints[wp_t].high = sqlite3_column_int(stmt, 6);
    wp_t++;
  }
  if (rc != SQLITE_DONE) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
  }
  sqlite3_finalize(stmt);
  reset_wp_distance();
}

/*
Read all points from a stored track

*/
void read_track(int track_id) {

  /*
       select * from track order where trackid = %parameter;
  */
  track_pos = -1;
  distance = 0;
  offset_m tmp;
  sqlite3_stmt *stmt;
  char tmpstr[128];
  sprintf(tmpstr, "select * from track where trackid = %d", track_id);

  int rc = sqlite3_prepare_v2(db1, tmpstr, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
    return;
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    track_pos++;
    if (track_pos >= maxpoints) {
      track_pos = 0;
      overflow_track = true;
    }
    track[track_pos].lat = sqlite3_column_double(stmt, 1);
    track[track_pos].lon = sqlite3_column_double(stmt, 2);
    track[track_pos].high = sqlite3_column_double(stmt, 3);
    if (track_pos > 0) {
      tmp = calc_movement(track[track_pos], track[track_pos - 1]);
      distance += tmp.dist;
      // azimuth = tmp.azimuth;
    }
  }
  if (rc != SQLITE_DONE) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
  }
  sqlite3_finalize(stmt);
  reset_track_distance();
 // track[track_pos].lat = current.lat;
 // track[track_pos].lon = current.lon;
}

void read_tbt(int track_id) {

  /*
       select * from track order where trackid = %parameter;
  */
  tbt_track_pos = -1;
  distance = 0;
  offset_m tmp;
  sqlite3_stmt *stmt;
  char tmpstr[128];
   if (preference_values[8] == 0) {
  sprintf(tmpstr, "select * from track where trackid = %d limit 1000", track_id);
   } else
   {
  sprintf(tmpstr, "select * from track where trackid = %d order by timestamp desc limit 1000", track_id);
   }

  int rc = sqlite3_prepare_v2(db1, tmpstr, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
    return;
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    tbt_track_pos++;
    if (tbt_track_pos >= maxpoints) {
      tbt_track_pos = 0;
      overflow_track = true;
    }
    track_tbt[tbt_track_pos].lat = sqlite3_column_double(stmt, 1);
    track_tbt[tbt_track_pos].lon = sqlite3_column_double(stmt, 2);
    track_tbt[tbt_track_pos].high = sqlite3_column_double(stmt, 3);
   /* if (tbt_track_pos > 0) {
      tmp = calc_movement(track_tbt[tbt_track_pos], track_tbt[tbt_track_pos - 1]);
      distance += tmp.dist;
      // azimuth = tmp.azimuth;
    }
    */
  }
  if (rc != SQLITE_DONE) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
  }
  sqlite3_finalize(stmt);
  reset_track_distance();
}





void read_position() {

  /*
      select * from track order by timestamp desc limit 1;
  */
  sqlite3_stmt *stmt;
  char tmpstr[128];

  // const char *sql = "select * from track  order by timestamp desc limit 1";
  sprintf(tmpstr, "select * from track  order by timestamp desc limit 1");
  int rc = sqlite3_prepare_v2(db1, tmpstr, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
    return;
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    current.lat = sqlite3_column_double(stmt, 1);
    current.lon = sqlite3_column_double(stmt, 2);
    current.high = sqlite3_column_double(stmt, 3);
    preference_values[4] = sqlite3_column_double(stmt, 4);
  }
  if (rc != SQLITE_DONE) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
  }
  sqlite3_finalize(stmt);
}

/*
       select * from track order by timestamp desc limit 1;
  */
void read_groups() {
  sqlite3_stmt *stmt;
  char tmpstr[64];
  const char *sql = "select * from groups order by id";
  int rc = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
    return;
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    sprintf(tmpstr, "%s", sqlite3_column_text(stmt, 1));
    groups[groups_t].name = tmpstr;
    //sprintf(tmpstr, "%s", sqlite3_column_text(stmt, 2));
    //groups[groups_t].description = tmpstr;
    groups[groups_t].group_id = sqlite3_column_int(stmt, 0);
    groups_t++;
  }
  if (rc != SQLITE_DONE) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
  }
  sqlite3_finalize(stmt);
}

/* Read all preferences from db on the boot*/
void read_preferences() {
  sqlite3_stmt *stmt;
  char tmpstr[64];
  const char *sql = "select * from preferences ";
  int rc = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
    return;
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int i = sqlite3_column_int(stmt, 0);

    if (i < num_pref) {
      preference_values[i] = sqlite3_column_int(stmt, 1);
    }
  }
  if (rc != SQLITE_DONE) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
  }
  sqlite3_finalize(stmt);
}

/*
Store all the members of the array preference_values,
called every time there is a change in any value
*/
void store_preferences() {
  char stringa[128];
  int i;

  for (i = 0; i < num_pref; i++) {
    sprintf(stringa, "INSERT OR REPLACE INTO preferences(id,value,name) values (%d,%d,\'%s\')", i, preference_values[i], preferences[i].label);
    //Serial.println(stringa);
    rc = db_exec(db1, stringa);
    if (rc != SQLITE_OK) {
      Serial.println("Unable to store data");
      M5.Lcd.setCursor(20, 225);
      M5.Lcd.print("Unable to store data: ");
      M5.Lcd.print(rc);
      delay(200);
    }
  }
}