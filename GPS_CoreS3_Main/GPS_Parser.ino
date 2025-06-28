/* Vars containing GPS infos */




void find_term(String str, int f) {
  int i, tmp_patt = 0;
  parsed = "";
  int max = str.length();

  for (i = 0; i < max; i++) {
    if ((str.charAt(i) == ',') || (str.charAt(i) == '*')) {
      tmp_patt++;
    } else if (tmp_patt == f) {

      parsed += str.charAt(i);

    } else if (tmp_patt > f) {
      break;
    }
  }
}

float parse_n(String nmea) {
  int a;
  float t, t1;
  String b;
  a = nmea.indexOf('.');
  //Serial.print(a);
  if (a == 5) {
    b = nmea.substring(0, 3);
    t = b.toFloat();
    b = nmea.substring(3);
  } else {
    b = nmea.substring(0, 2);
    t = b.toFloat();
    b = nmea.substring(2);
  }

  t1 = b.toFloat();
  t += (t1 / 60.0);
  return t;
}

void parser_task(void *) {
  for (;;) {
    if (GPSRaw.available()) {
      int ch = GPSRaw.read();
      // Serial.write(ch);
      char card;
      str.concat(char(ch));
      if (ch == 10) {

        String sent = str.substring(1, 6);



        if ((sent == "GPRMC") || (sent == "GNRMC")) {

          find_term(str, 9);
          mygps.rawdate = parsed.substring(4, 6);
          mygps.rawdate += "-";
          mygps.rawdate += parsed.substring(2, 4);
          mygps.rawdate += "-";
          mygps.rawdate += parsed.substring(0, 2);
          find_term(str, 1);
          mygps.fixtime = parsed.substring(0, 6);
          mygps.rawdate += " ";
          mygps.rawdate += parsed.substring(0, 2);
          mygps.rawdate += ":";
          mygps.rawdate += parsed.substring(2, 4);
          mygps.rawdate += ":";
          mygps.rawdate += parsed.substring(4, 6);
          mygps.parsedtime = parsed.substring(0, 2);
          mygps.parsedtime += ":";
          mygps.parsedtime += parsed.substring(2, 4);
          mygps.parsedtime += ":";
          mygps.parsedtime += parsed.substring(4, 6);


        } else if (sent == "GPGSV") {

          find_term(str, 3);
          mygps.nsat = parsed.toInt();

          //sat 1
          sv++;

          find_term(str, 4);
          mysat[sv].sat_n = parsed.toInt();
          find_term(str, 5);
          mysat[sv].elevation = parsed.toInt();
          find_term(str, 6);
          // Serial.print(parsed);
          mysat[sv].azimuth = parsed.toInt();
          find_term(str, 7);
          mysat[sv].snr = parsed.toInt();
          //sat 2
          sv++;

          find_term(str, 8);
          mysat[sv].sat_n = parsed.toInt();
          find_term(str, 9);
          mysat[sv].elevation = parsed.toInt();
          find_term(str, 10);
          mysat[sv].azimuth = parsed.toInt();
          find_term(str, 11);
          mysat[sv].snr = parsed.toInt();
          //sat 3
          sv++;

          find_term(str, 12);
          mysat[sv].sat_n = parsed.toInt();
          find_term(str, 13);
          mysat[sv].elevation = parsed.toInt();
          find_term(str, 14);
          mysat[sv].azimuth = parsed.toInt();
          find_term(str, 15);
          mysat[sv].snr = parsed.toInt();
          //sat 4
          sv++;

          find_term(str, 16);
          mysat[sv].sat_n = parsed.toInt();
          find_term(str, 17);
          mysat[sv].elevation = parsed.toInt();
          find_term(str, 18);
          mysat[sv].azimuth = parsed.toInt();
          find_term(str, 19);
          mysat[sv].snr = parsed.toInt();


        } else if ((sent == "GPGGA") || (sent == "GNGGA")) {

          find_term(str, 6);
          mygps.fix = parsed.toInt();
          if (mygps.fix > 0) {
            //  log_position();
            find_term(str, 7);
            mygps.nsat_t = parsed.toInt();
            find_term(str, 3);
            card = parsed.charAt(1);
            find_term(str, 2);
            if (card != 'S') {
              current.lat = parse_n(parsed);
            } else {
              current.lat = -parse_n(parsed);
            }
            find_term(str, 5);
            card = parsed.charAt(1);
            find_term(str, 4);
            if (card == 'W') {
              current.lon = -parse_n(parsed);
            } else {
              current.lon = parse_n(parsed);
            }
            find_term(str, 9);
            current.high = parsed.toFloat();
          } else {
            mygps.nsat_t = 0;
          }
          if (true) {
            // store_position();
            //   log_position();
          }
          // put condition
          if (preference_values[1] == 1) {
            // send_position();
          }
          data_ready = true;

          //
          if (disp_on > 0) {
            //  refresh();
          }
          sv = 0;
        } else if (sent == "GPGLL") {
          // print_sat(parsed);
          //refresh();
          // sv=0;
        } else {
          // statements
          // Serial.println(str.substring(1,6));
        }




        str = "";
      }
      //Serial.write(ch);
      //termPutchar(ch);
    }
  }
}