/***************************************************
 *  This sketch handle the serial UI commands
 *
 *  Author: Brun
 *
 ****************************************************/


void message(String msg) {

  Serial.print(msg);
}

void message(byte loglevel, String msg) {

  if ( loglevel == ERROR ) {
    Serial.print(F("ERROR: "));
    Serial.println(msg);
  }
  else {
    for (int i = 0; i <= 7; i++) {
      if ( (bitRead(loglevel, i)) && (bitRead(debug, i)) ) {
        Serial.print(msg);
      }
    }
  }
}



void serial_parse() {

  String tmp;
  int n;

  if ( inputString.startsWith(F("debug=")) ) {
    tmp = getValue( inputString, '=', 1 );
    if ( tmp.toInt() == 0 ) {
      debug = 0;
    }
    else {
      bitSet(debug, tmp.toInt()-1);
    }
    Serial.print(F("Debug Level is: "));
    Serial.println(debug, DEC);
  }
  else if ( inputString.startsWith(F("print")) ) {
    Serial.println(F("PRINT found"));
    serial_print_status();
    //print_port_values();
  }
  else if ( inputString.startsWith(F("i2c scan")) ) {
    Serial.println(F("i2c scan found"));
    i2c_init();
  }
  else if ( inputString.startsWith(F("features")) ) {
    Serial.println(F("Features:"));
    serial_print_features();
  }
  else if ( inputString.startsWith(F("status")) ) {
    Serial.println(F("Status:"));
    serial_print_status();
  }
  else if ( inputString.startsWith(F("tail")) ) {
    if (!tail) {
      tail = true;
      Serial.println(F("tail is on"));
    }
    else {
      tail = false;
      Serial.println(F("tail is off"));
    }
  }
  else if ( inputString.startsWith(F("config")) ) {
    Serial.println(F("Config:"));
    serial_print_config();
  }
  else if ( inputString.startsWith(F("ports")) ) {
    Serial.println(F("Ports:"));
    serial_print_ports();
  }
  else if ( inputString.startsWith(F("save")) ) {
    Serial.println(F("Save configuratgion:"));
    check_plausibility();
    save_config();
  }
  else if ( inputString.startsWith(F("check")) ) {
    Serial.println(F("Check configuratgion:"));
    check_plausibility();
  }
  else if ( inputString.startsWith(F("help")) ) {
    Serial.println(F("Help:"));
    serial_print_help();
  }



  else if ( inputString.startsWith(F("tinygsm_init")) ) {
    Serial.println(F("tinygsm_init:"));
    tinygsm_init();
  }
  else if ( inputString.startsWith(F("tinygsm_info")) ) {
    Serial.println(F("tinygsm_info:"));
    tinygsm_info();
  }
  else if ( inputString.startsWith(F("gsm_info")) ) {
    Serial.println(F("gsm_info:"));
    tinygsm_info();
  }
  else if ( inputString.startsWith(F("tinygsm_gps_init")) ) {
    Serial.println(F("tinygsm_gps_init:"));
    tinygsm_gps_init();
  }
  else if ( inputString.startsWith(F("sdcard_init")) ) {
    Serial.println(F("sdcard_init:"));
    sdcard_init();
  }
  else if ( inputString.startsWith(F("mount")) ) {
    Serial.println(F("sdcard_init:"));
    sdcard_init();
  }

  else if ( inputString.startsWith(F("go_online")) ) {
    Serial.println(F("going online for 120s"));
    if (tinygsm_go_online()) {
      online = true;
      online_intervall_timer = millis() + 120000;
      Serial.println(F("we are online"));
    }
  }
  else if ( inputString.startsWith(F("go_offline")) ) {
    if (tinygsm_go_offline()) {
      online = false;
      online_intervall_timer = millis() + online_interval * 60000;
      Serial.println(F("we are offline"));
    }
  }

  else if ( inputString.startsWith(F("free")) ) {
    Serial.print(F("free RAM: "));
    Serial.print(freeRam(), DEC);
    Serial.println(F(" Bytes"));
    sdcard_free();

  }
  else if ( inputString.startsWith(F("dump=")) ) {
    tmp = getValue( inputString, '=', 1 );
    dump_file(tmp.toInt());
  }
  else if ( inputString.startsWith(F("dump")) ) {
    debug = 0;
    dump_sd_card();
  }
  else if ( inputString.startsWith(F("dir")) ) {
    sdcard_ls();
  }

  else if ( inputString.startsWith(F("logo")) ) {
    MainMenuPos=0;
  }
  else if ( inputString.startsWith(F("error")) ) {
    notify(DISPLAY_WARNING+SOUND_INFO, F("Licht angelassen?"));
  }
  else if ( inputString.startsWith(F("readsd")) ) {
    sdcard_open_config();
  }
  else if ( inputString.startsWith(F("readflash")) ) {
    read_virtual_eeprom();
  }



  else if ( inputString.startsWith(F("debug")) ) {

    Serial.print(F("Debug Level is: "));
    Serial.println(debug, DEC);
    Serial.println(F("#############"));
    Serial.println(F("debug=0   ->  all off"));
    Serial.println(F("     =1   ->  INFO"));
    Serial.println(F("     =2   ->  DEBUG"));
    Serial.println(F("     =3   ->  DEBUG_TINYGSM"));
    Serial.println(F("     =4   ->  DEBUG_SD"));
    Serial.println(F("     =5   ->  DEBUG_IO"));
    Serial.println(F("     =6   ->  nothing"));
    Serial.println(F("     =7   ->  TRACE_TINYGSM"));
    Serial.println(F("     =8   ->  TRACE"));


  }
  else {
    parse_config_string(inputString);
  }

  Serial.print(F("#fiz-o-matic > "));
}


void serial_print_status() {
    /*
    Serial.write(27);       // ESC command
    Serial.print("[2J");    // clear screen command
    Serial.write(27);
    Serial.print("[H");     // cursor to home command
    */


    //Serial.println(F(""));
    Serial.println(F("!================"));

    Serial.print(F("time:"));
    sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
    Serial.println(buf);

    Serial.print(F("date:"));
    sprintf(buf, "%02d.%02d.20%02d", rtc.getDay(), rtc.getMonth(), rtc.getYear());
    Serial.println(buf);

    Serial.print(F("gsm_signal:"));
    Serial.println(gsm_signal, DEC);
    Serial.print(F("gps_fixstatus:"));
    Serial.println(gps_fixstatus, DEC);
    Serial.print(F("gps_latitude:"));
    Serial.println(gps_latitude, DEC);
    Serial.print(F("gps_longitude:"));
    Serial.println(gps_longitude, DEC);
    Serial.print(F("gps_altitude:"));
    Serial.println(gps_altitude, DEC);
    Serial.print(F("gps_speed:"));
    Serial.println(gps_speed, 2);
    Serial.print(F("gps_course:"));
    Serial.println(gps_course, DEC);
    Serial.print(F("gps_view_satellites:"));
    Serial.println(gps_view_satellites, DEC);
    Serial.print(F("gps_used_satellites:"));
    Serial.println(gps_used_satellites, DEC);
    Serial.print(F("gps_distance:"));
    Serial.println(gps_distance, DEC);

    /*Serial.print(F("Port 1 (V): "));
    Serial.println(a0_V, 2);
    Serial.print(F("Port 2 (V): "));
    Serial.println(a1_V, 2);
    Serial.print(F("Port 3 (V): "));
    Serial.println(a2_V, 2);
    Serial.print(F("Port 4 (V): "));
    Serial.println(a3_V, 2);
    Serial.print(F("Port 5 (Hz): "));
    Serial.println(a4_hz, 2);
    Serial.print(F("Port 6 (Hz): "));
    Serial.println(a5_hz, 2);*/

    Serial.print(F("speed:"));
    Serial.println(speed, DEC);

    for (int i = 0; i <= (sizeof(values) / sizeof(values[0])) - 1; i++) {
      Serial.print(values[i].name);
      Serial.print(F(":"));
      Serial.println(*values[i].value);
    }

    Serial.println(F("=== Port Register ==="));

    for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
      if ( port_values[i].port == 0x00 ) break;

      Serial.print(F("0x"));
      if (port_values[i].port<16) {
        Serial.print(F("0"));
      }
      Serial.print(String(port_values[i].port, HEX));
      Serial.print(F(":"));
      Serial.println(String(port_values[i].value, 2));
      //Serial.print(F("\n"));
    }
    Serial.println(F("#================"));
}

void serial_print_ports(){
  Serial.println(F("# Port Register"));
  Serial.println(F("#================"));
  for (int i = 0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++){
    Serial.print(F("# "));
    Serial.println(ports[i].desc);
    Serial.print(ports[i].name);
    Serial.print(F("=0x"));
    if (*ports[i].port<16) {
      Serial.print(F("0"));
    }
    Serial.println(*ports[i].port, HEX);
  }

}

void serial_print_features(){
  for (int i = 0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++){
    Serial.print(F("# "));
    Serial.println(features[i].desc);
    Serial.print(features[i].name);
    Serial.print(F("="));
    if (*features[i].feature) {
      Serial.println(F("true"));
    }
    else {
      Serial.println(F("false"));
    }
  }
}

void serial_print_config(){
  Serial.println(F("#SIM APN"));
  Serial.print(F("sim_pin="));
  Serial.println(sim_pin);
  Serial.println(F("#Provider APN"));
  Serial.print(F("sim_apn="));
  Serial.println(sim_apn);
  Serial.println(F("#username for the APN"));
  Serial.print(F("sim_user="));
  Serial.println(sim_user);
  Serial.println(F("#password for the APN"));
  Serial.print(F("sim_pass="));
  Serial.println(sim_pass);
  Serial.println(F("#BLYNK authentication key"));
  Serial.print(F("blynk_key="));
  Serial.println(blynk_key);
  Serial.println(F("#SMS Keyword for authentication"));
  Serial.print(F("sms_keyword="));
  Serial.println(sms_keyword);

  for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
    Serial.print(F("# "));
    Serial.println(config[i].desc);
    Serial.print(config[i].name);
    Serial.print(F("="));
    Serial.println(*config[i].config, DEC);
  }
}



void serial_print_info() {
  Serial.println(F("FIZ-o-matic ERROR 404"));
}


void serial_print_help() {
  Serial.println(F("FIZ-o-matic"));
  Serial.println(F("available commands:"));
  Serial.println(F("  print      -> print all actual values"));
  Serial.println(F("  tail       -> repeat the print every 2s"));
  Serial.println(F("  config     -> print the configuration"));
  Serial.println(F("  ports      -> print the port configuration"));
  Serial.println(F("  features   -> print the features state"));
  Serial.println(F("  debug      -> set the debug level"));
  Serial.println(F("  i2c scan   -> scan the I2C Bus"));
  Serial.println(F("  go_online  -> go online for 120s"));
  Serial.println(F("  go_offline -> go offline"));
  Serial.println(F("  free       -> print free RAM and SD"));
  Serial.println(F("  mount      -> mount the SD Card"));
  Serial.println(F("  readsd     -> read config from SD"));
  Serial.println(F("  gsm_info   -> print infos about the connection"));




  Serial.println(F("\nFIZ-o-matic"));

  Serial.print( F("Version: ") );
  Serial.println( F(VERSION) );

  Serial.print( F("Buid: ") );
  Serial.print( F(__DATE__) );
  Serial.print( F(", ") );
  Serial.println( F(__TIME__) );
}
