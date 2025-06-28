#define APIKEY "< YOUR API KEY>"


void handleRoot(const char *db_name, const char *sql) {
  String temp;
  temp = "<html><head>\
      <title>ESP32 Sqlite Web Console</title>\
      <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: large; Color: #000088; }\
      </style>\
      <meta http-equiv=\"Pragma\" content=\"no-cache\">\
  </head>\
  <body>\
      <h2>GPSCoreS3 Sqlite Web Console</h2>\
      <hr>\ 
      <table cellspacing='0' cellpadding='0' border='0'>\
      <tr>\
      <td><form><input type=\"button\" onclick=\"window.location.href='/wp';\" value=\"Manage Waypoints\" /></form>\</td>\
      <td><form><input type=\"button\" onclick=\"window.location.href='/track';\" value=\"Visualize Track\" /></form>\</td>\
      <td><form><input type=\"button\" onclick=\"window.location.href='/track';\" value=\"Import GPX Waypoints\" /></form>\</td>\
      <td><form><input type=\"button\" onclick=\"window.location.href='/track';\" value=\"Import GPX Track\" /></form>\</td>\
      </tr>\
      </table>\
      <hr>\ 
      <h3>Insert the SQL code to execute on the database</h3>\
      <h4>To see all the tables:</h4>\
      <h4><em>SELECT * FROM sqlite_master WHERE type='table'</em></h4>\
      <form name = 'params' method = 'POST' action = 'exec_sql' >\
      <textarea style = 'font-size: medium; width:100%' rows = '4' placeholder = 'Enter SQL Statement' name = 'sql'></textarea >\
      <br><br><input type = submit style = 'font-size: large' value = 'Execute' /></ form><hr /> ";
  server.send(200, "text/html", temp.c_str());
}

void insertwp() {
  char sqlquery[200];
  String resp;
  String wpid = server.arg("wpid");
  String wpname = server.arg("wpname");
  String lat = server.arg("lat");
  String lon = server.arg("lon");
  String high = server.arg("high");
  String type = server.arg("type");
  String group = server.arg("group");
  if (wpid.toInt() > 0) {
    sprintf(sqlquery, "update waypoints set name = '%s',lat = %s, lon = %s, alt = %s,  type = %s, groups = %s  where id = %s;", wpname, lat, lon, high, type, group, wpid);
  } else {
    sprintf(sqlquery, "insert into waypoints (name ,lat, lon , alt, type , groups ) values ('%s',%s,%s,%s,%s,%s);", wpname, lat, lon, high, type, group);
  }
  rc = sqlite3_prepare_v2(db1, sqlquery, 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db1);
    resp += "<br><br><a href='/'>back</a>";
    server.sendContent(resp);
    return;
  }
  sqlite3_step(res) == SQLITE_ROW;
  server.sendHeader("Location", "/wp");
  server.send(301);
}

void deletewp() {
  /*
  Delete a waypoint record from waypoint table using wpid
  */
  char sqlquery[200];
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  String wpid = server.arg("wpid");
  String temp;
  sprintf(sqlquery, "delete from  waypoints where id = %s;", wpid);
  Serial.println(sqlquery);
  rc = sqlite3_prepare_v2(db1, sqlquery, 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db1);
    resp += "<br><br><a href='/'>back</a>";
    server.sendContent(resp);
    return;
  }
  sqlite3_step(res) == SQLITE_ROW;
  server.sendHeader("Location", "/wp");
  server.send(301);
}


void editwp() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  String wpid = server.arg("wpid");
  String temp;
  String resp;
  char sqlquery[200];
  temp = "<html><head>\
      <title>GPSCoreS3 Web Console</title>\
      <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: large; Color: #000088; }\
      </style>\
      <meta http-equiv=\"Pragma\" content=\"no-cache\">\
  </head>\
  <body>\
      <h2>GPSCoreS3 WP Editor</h2>";
  server.send(200, "text/html", temp.c_str());
  sprintf(sqlquery, "select * from waypoints where id = %s;", wpid);
  rc = sqlite3_prepare_v2(db1, sqlquery, 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db1);
    resp += "<br><br><a href='/'>back</a>";
    server.sendContent(resp);
    return;
  }
  sqlite3_step(res) == SQLITE_ROW;
  resp = "";
  resp += "</h3><table cellspacing='1' cellpadding='1' border='1'>";
  resp += "<form  method='POST' action=\"/insertwp\">";
  resp += "<input type=\"hidden\" id=\"wpid\" name=\"wpid\" value=\"";
  resp += wpid;
  resp += "\">";
  resp += "<tr><td><label for=\"wpname\">WP Name:</label></td>";
  resp += "<td><input type=\"text\" id=\"wpname\" name=\"wpname\" value=\"";
  resp += (const char *)sqlite3_column_text(res, 4);
  resp += "\"></td></tr>";
  resp += "<tr><td><label for=\"lat\">Latitude:</label></td>";
  resp += "<td><input type=\"text\" id=\"lat\" name=\"lat\"  value=\"";
  resp += (const char *)sqlite3_column_text(res, 1);
  resp += "\"></td></tr>";
  resp += "<tr><td><label for=\"lon\">Longitude:</label></td>";
  resp += "<td><input type=\"text\" id=\"lon\" name=\"lon\" value=\"";
  resp += (const char *)sqlite3_column_text(res, 2);
  resp += "\"></td></tr>";
  resp += "<tr><td><label for=\"high\">Altitude:</label></td>";
  resp += "<td><input type=\"text\" id=\"high\" name=\"high\" value=\"";
  resp += (const char *)sqlite3_column_text(res, 3);
  resp += "\"></td></tr>";
  resp += "<tr><td><label for=\"type\">Type:</label></td>";
  resp += "<td><input type=\"text\" id=\"type\" name=\"type\" value=\"";
  resp += (const char *)sqlite3_column_text(res, 6);
  resp += "\"></td></tr>";

  resp += "<tr><td><label for=\"group\">Group:</label></td>";
  resp += "<td><input type=\"text\" id=\"group\" name=\"group\" value=\"";
  resp += (const char *)sqlite3_column_text(res, 7);
  resp += "\"></td></tr>";

  resp += "</table>";
  resp += "<br><input type = \"submit\" value = \"Submit\">";
  resp += "</ form>";

  resp += "<hr><hr>\
            <form>\
            <input type =\"button\" onclick=\"window.location.href='/deletewp?wpid=";
  resp += wpid;
  resp += "';\" value=\"Delete this Waypoint!\" >\
      </form></body>";
  server.sendContent(resp);
}


void newwp() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);

  String temp;
  String resp;

  temp = "<html><head>\
      <title>GPSCoreS3 Web Console</title>\
      <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: large; Color: #000088; }\
      </style>\
      <meta http-equiv=\"Pragma\" content=\"no-cache\">\
  </head>\
  <body>\
      <h2>GPSCoreS3 WP Editor</h2>";
  server.send(200, "text/html", temp.c_str());
  resp = "";
  resp += "</h3><table cellspacing='1' cellpadding='1' border='1'>";
  resp += "<form  method='POST' action=\"/insertwp\">";
  resp += "<tr><td><label for=\"wpname\">WP Name:</label></td>";
  resp += "<td><input type=\"text\" id=\"wpname\" name=\"wpname\"></td></tr>";
  resp += "<tr><td><label for=\"lat\">Latitude:</label></td>";
  resp += "<td><input type=\"text\" id=\"lat\" name=\"lat\"></td></tr>";
  resp += "<tr><td><label for=\"lon\">Longitude:</label></td>";
  resp += "<td><input type=\"text\" id=\"lon\" name=\"lon\"></td></tr>";
  resp += "<tr><td><label for=\"high\">Altitude:</label></td>";
  resp += "<td><input type=\"text\" id=\"high\" name=\"high\"></td></tr>";
  resp += "<tr><td><label for=\"type\">Type:</label></td>";
  resp += "<td><input type=\"text\" id=\"type\" name=\"type\"></td></tr>";
  resp += "<tr><td><label for=\"group\">Group:</label></td>";
  resp += "<td><input type=\"text\" id=\"group\" name=\"group\"></td></tr>";
  resp += "</table>";
  resp += "<input type = \"submit\" value = \"Submit\">";
  resp += "</ form>";
  server.sendContent(resp);
}



void handletrack() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  String temp;
  char tmpstr[128];
  char linkurl[150];
  String strlat;
  String strlon;
  String strzoom;
  temp = "<html><head>\
      <title>ESP32 Sqlite Web Console</title>\
      <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: large; Color: #000088; }\
      </style>\
      <meta http-equiv=\"Pragma\" content=\"no-cache\">\
  </head>\
  <body>\
      <h2>GPSCoreS3 Sqlite Web Console</h2>\
       <table cellspacing='0' cellpadding='0' border=''>\
      <tr>\
      <td><form><input type=\"button\" onclick=\"window.location.href='/exportkml';\" value=\"Export as KML\" /></form></td>\
      <td><form><input type=\"button\" onclick=\"window.location.href='/exportgpx';\" value=\"Export as GPX\" /></form></td>\
      </tr></table>\
      <hr><hr>\
      <p>Points from this track: </p>";
  server.send(200, "text/html", temp.c_str());
  sprintf(tmpstr, "select * from track where trackid = %d", preference_values[4]);

  rc = sqlite3_prepare_v2(db1, tmpstr, 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db1);
    resp += "<br><br><a href='/'>back</a>";
    server.sendContent(resp);
    return;
  }
  rec_count = 0;
  String resp = "<h2>Result:</h2><h3>";
  resp += tmpstr;
  resp += "</h3><table cellspacing='1' cellpadding='1' border='1'>";
  server.sendContent(resp);
  bool first = true;
  while (sqlite3_step(res) == SQLITE_ROW) {
    strzoom = "14.7";
    strlat = (const char *)sqlite3_column_text(res, 1);
    strlon = (const char *)sqlite3_column_text(res, 2);
    strlon.replace("-", "");
    // https://image.maps.hereapi.com/mia/v3/base/mc/center:52.515224,13.454021;zoom=18/256x256/png?apiKey=BAAjzSmoyrlzt735vrRWyFGqTq2NLH2XUYYKrSqR4E4
    sprintf(linkurl, "https://image.maps.hereapi.com/mia/v3/base/mc/center:%s,%s;zoom=%s/600x600/png?apiKey=%s", strlat, strlon, strzoom, APIKEY);
    //   sprintf//(linkurl, "https://image.maps.ls.hereapi.com/mia/1.6/mapview?apiKey=%s&c=%s,%s&h=600&w=600&z=%s", APIKEY, strlat, strlon, strzoom);
    resp = "";
    resp += "<tr>";
    resp += "<td>";
    resp += "<a href=";
    resp += linkurl;
    resp += ">";
    resp += (const char *)sqlite3_column_text(res, 0);
    resp += "</a></td>";
    resp += "<td>";
    resp += strlat;
    resp += "</td>";
    resp += "<td>";
    resp += strlon;
    resp += "</td>";
    resp += "<td>";
    resp += (const char *)sqlite3_column_text(res, 3);
    resp += "</td>";
    resp += "</tr>";

    server.sendContent(resp);
  }
  resp = "</table><br><br>Number of records: ";

  resp += ".<br><br><input type=button onclick='location.href=\"/\"' value='back'/>";
  server.sendContent(resp);
  sqlite3_finalize(res);
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
/*
Visualize all waypoints, ordered by ID

*/
void handleWP() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  String temp;
  char linkurl[150];
  char linkwp[150];
  String strlat;
  String strlon;
  String strwpid;
  String strzoom;
  temp = "<html><head>\
      <title>ESP32 Sqlite Web Console</title>\
      <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: large; Color: #000088; }\
      </style>\
      <meta http-equiv=\"Pragma\" content=\"no-cache\">\
      </head>\
      <body>\
      <h2>GPSCoreS3 Sqlite Web Console</h2>\
      <form>\
      <input type=\"button\" onclick=\"window.location.href='/newwp';\" value=\"New Waypoint\" />\
      </form>\
      <form>\
      <input type=\"button\" onclick=\"window.location.href='/export_wp';\" value=\"Export\" />\
      </form>\
      <p><h3> List of Waypoints : </ h3></ p> ";
  server.send(200, "text/html", temp.c_str());
  String sql = "select lat,lon,name,type,groups,id from waypoints;";
  rc = sqlite3_prepare_v2(db1, sql.c_str(), 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db1);
    resp += "<br><br><a href='/'>back</a>";
    server.sendContent(resp);
    return;
  }
  rec_count = 0;
  String resp = "";

  resp += "<table cellspacing='1' cellpadding='1' border='1'>";
  server.sendContent(resp);
  bool first = true;
  while (sqlite3_step(res) == SQLITE_ROW) {
    strzoom = "15.5";
    strlat = (const char *)sqlite3_column_text(res, 0);
    strlon = (const char *)sqlite3_column_text(res, 1);
    strwpid = (const char *)sqlite3_column_text(res, 5);
    // sprintf(linkurl, "https://image.maps.ls.hereapi.com/mia/1.6/mapview?apiKey=%s&c=%s,%s&h=1024&w=1280&z=%s", APIKEY, strlat, strlon, strzoom);
    sprintf(linkurl, "https://image.maps.hereapi.com/mia/v3/base/mc/center:%s,%s;zoom=%s/600x600/png?apiKey=%s", strlat, strlon, strzoom, APIKEY);
    sprintf(linkwp, "/editwp?wpid=%s", strwpid);
    resp = "";
    resp += "<tr>";
    resp += "<td>";
    resp += "<a href=";
    resp += linkwp;
    resp += ">";
    resp += (const char *)sqlite3_column_text(res, 2);
    resp += "</a></td>";
    resp += "<td>";
    resp += "<a href=";
    resp += linkurl;
    resp += ">";
    resp += strlat;
    resp += "</a></td>";
    resp += "<td>";
    resp += "<a href=";
    resp += linkurl;
    resp += ">";
    resp += strlon;
    resp += "</a></td>";
    resp += "<td>";
    resp += (const char *)sqlite3_column_text(res, 3);
    resp += "</td>";
    resp += "</tr>";

    server.sendContent(resp);
  }
  resp = "</table>";

  resp += "<br><br><input type=button onclick='location.href=\"/\"' value='back'/>";
  server.sendContent(resp);
  sqlite3_finalize(res);
}

/*  
   * Handle the generic SQL query
   *Allow to operate directly with the internal database via standard commands
   */
void handleExec() {
  String db_name = server.arg("db_name");
  String sql = server.arg("sql");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  handleRoot(db_name.c_str(), sql.c_str());
  rc = sqlite3_prepare_v2(db1, sql.c_str(), 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db1);
    resp += "<br><br><a href='/'>back</a>";
    server.sendContent(resp);
    // Serial.println(resp.c_str());
    return;
  }

  rec_count = 0;
  String resp = "<h2>Result:</h2><h3>";

  resp += "</h3><table cellspacing='1' cellpadding='1' border='1'>";
  server.sendContent(resp);
  bool first = true;
  while (sqlite3_step(res) == SQLITE_ROW) {
    resp = "";
    if (first) {
      int count = sqlite3_column_count(res);
      if (count == 0) {
        resp += "<tr><td>Statement executed successfully</td></tr>";
        rec_count = sqlite3_changes(db1);
        break;
      }
      resp += "<tr>";
      for (int i = 0; i < count; i++) {
        resp += "<td>";
        resp += sqlite3_column_name(res, i);
        resp += "</td>";
      }
      resp += "</tr>";
      first = false;
    }
    int count = sqlite3_column_count(res);
    resp += "<tr>";
    for (int i = 0; i < count; i++) {
      resp += "<td>";
      resp += (const char *)sqlite3_column_text(res, i);
      resp += "</td>";
    }
    resp += "</tr>";
    server.sendContent(resp);
    rec_count++;
  }
  resp = "</table><br><br>Number of records: ";
  resp += rec_count;
  resp += ".<br><br><input type=button onclick='location.href=\"/\"' value='back'/>";
  server.sendContent(resp);
  sqlite3_finalize(res);
}
/*
  Read wifi settings from table wifisettings and try to setup the wifi, 
  higher id have priority
  Note: Access point mode must have a password lenght at least 8 chars, otherwise the setting will be ignored


  */
void wifi_init() {

  sqlite3_stmt *stmt;
  String sql = " SELECT * FROM wifisettings order by id desc;";
  rc = sqlite3_prepare_v2(db1, sql.c_str(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    Serial.print("error: ");
    Serial.println(sqlite3_errmsg(db1));
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db1);
    resp += "<br><br><a href='/'>back</a>";
    server.sendContent(resp);
    return;
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int counter = 0;
    int pref_mode = sqlite3_column_int(stmt, 1);
    char pref_ssid[64];
    sprintf(pref_ssid, "%s", sqlite3_column_text(stmt, 2));

    char pref_pass[64];
    sprintf(pref_pass, "%s", sqlite3_column_text(stmt, 3));
    if (pref_mode == 0) {
      WiFi.mode(WIFI_STA);
      WiFi.begin(pref_ssid, pref_pass);
      while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
        counter++;
        if (counter > 70) {
          break;
        }
      }
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected: ");
        return;
        break;
      }
    } else {
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      delay(300);
      WiFi.mode(WIFI_AP);
      Serial.println("Starting as accesspoint ");
      WiFi.softAP(pref_ssid, pref_pass, 12, 0, 4, false);
      delay(100);
      return;
      break;
    }
  }
}
/*
Export KML path of the currently selected path.
Documetation ofor the kml format on https://developers.google.com/kml/documentation/kml_tut
*/
void export_kml() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  String temp;
  int size_t;
  char linkurl[150];
  char linkwp[150];
  char tmpstr[128];
  server.sendHeader("Content-Type", "text/text");
  sprintf(tmpstr, "attachment; filename= track_%d.kml", preference_values[4]);
  server.sendHeader("Content-Disposition", tmpstr);
  server.sendHeader("Connection", "close");
  server.send(200, "text/text", "");
  sprintf(tmpstr, "select * from track where trackid = %d", preference_values[4]);

  rc = sqlite3_prepare_v2(db1, tmpstr, 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db1);
    resp += "<br><br><a href='/'>back</a>";
    server.sendContent(resp);
    return;
  }
  temp = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<kml xmlns=\"http://www.opengis.net/kml/2.2\">\
  <Document>\
    <name>My Path</name>\
    <description>My Path</description>\
    <Style id=\"yellowLineGreenPoly\">\
      <LineStyle>\
        <color>7f00ffff</color>\
       <width>4</width>\
      </LineStyle>\
      <PolyStyle>\
        <color>7f00ff00</color>\
      </PolyStyle>\
    </Style>\
    <Placemark>\
      <name>Absolute Extruded</name>\
      <description>Transparent green wall with yellow outlines</description>\
      <styleUrl>#yellowLineGreenPoly</styleUrl>\
      <LineString>\
        <extrude>1</extrude>\
        <tessellate>1</tessellate>\
        <altitudeMode>absolute</altitudeMode>\
        <coordinates>";
  // size_t = temp.length();
  server.sendContent_P(temp.c_str());
  while (sqlite3_step(res) == SQLITE_ROW) {
    temp = "";
    temp += (const char *)sqlite3_column_text(res, 2);
    temp += ",";
    temp += (const char *)sqlite3_column_text(res, 1);
    temp += ",";
    temp += (const char *)sqlite3_column_text(res, 3);
    temp += "\n";
    server.sendContent_P(temp.c_str());
  }
  temp = "        </coordinates>\
      </LineString>\
    </Placemark>\
  </Document>\
</kml>";
  server.sendContent_P(temp.c_str());
}

/*
Export GPX path of the currently selected path.
Documetation about the GPX format on https://wiki.openstreetmap.org/wiki/GPX#:~:text=GPX%2C%20or%20GPS%20exchange%20format,and%20convert%20to%20other%20forms.
*/
void export_gpx() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  String temp;
  int size_t;
  char linkurl[150];
  char linkwp[150];
  char tmpstr[128];
  server.sendHeader("Content-Type", "text/text");
  sprintf(tmpstr, "attachment; filename= track_%d.gpx", preference_values[4]);
  server.sendHeader("Content-Disposition", tmpstr);
  server.sendHeader("Connection", "close");
  server.send(200, "text/text", "");
  sprintf(tmpstr, "select * from track where trackid = %d", preference_values[4]);

  rc = sqlite3_prepare_v2(db1, tmpstr, 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db1);
    resp += "<br><br><a href='/'>back</a>";
    server.sendContent(resp);
    return;
  }
  temp = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
  <gpx version=\"1.0\">\n\
	<name>My track gpx</name>\n\
	<trk><name>My track gpx</name><number>1</number><trkseg>\n";
  // size_t = temp.length();
  server.sendContent_P(temp.c_str());
  while (sqlite3_step(res) == SQLITE_ROW) {
    temp = "<trkpt lat=\"";
    temp += (const char *)sqlite3_column_text(res, 1);
    temp += "\" lon=\"";
    temp += (const char *)sqlite3_column_text(res, 2);
    temp += "\"><ele>";
    temp += (const char *)sqlite3_column_text(res, 3);
    temp += "</ele><time>";
    temp += (const char *)sqlite3_column_text(res, 0);
    temp += "Z</time></trkpt>\n";
    server.sendContent_P(temp.c_str());
  }
  temp = " </trkseg></trk>\
  </gpx>";
  server.sendContent_P(temp.c_str());
}



void sqlconsole_init() {

  wifi_init();
  // Setup the web server
  server.on("/", []() {
    handleRoot(NULL, NULL);
  });
  server.on("/exec_sql", handleExec);
  server.on("/wp", handleWP);
  server.on("/track", handletrack);
  server.on("/exportkml", export_kml);
  server.on("/exportgpx", export_gpx);
  server.on("/editwp", editwp);
  server.on("/newwp", newwp);
  server.on("/deletewp", deletewp);
  server.on("/insertwp", HTTP_POST, insertwp);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  current_status.sqlconsole = true;
  current_status.espnow = false;
  preference_values[1] = 0;
}
void sqlconsole_stop() {
  WiFi.mode(WIFI_OFF);
  current_status.sqlconsole = false;
}
void sqlconsole_handle() {
  server.handleClient();
}