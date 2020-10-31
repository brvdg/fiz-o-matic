
String parse_ui_string(String msg) {
  return parse_ui_string(msg, 0);
}



String parse_ui_string(String msg, uint8_t src) {

  String output_msg;


  if ( msg.startsWith(F("help")) ) {
    if ( src != SRC_SMS ) {
      output_msg = print_help();
    }
    else {
      output_msg = print_help_sms();
    }
  }
  else if ( msg.startsWith(F("on")) || msg.startsWith(F("online"))) {
    tinygsm_go_online();
    return "--- ON ---";

  }
  else if ( msg.startsWith(F("off")) || msg.startsWith(F("offline"))) {
    tinygsm_go_offline();
    return "--- OFF ---";
  }
  else if ( msg.startsWith(F("pos")) || msg.startsWith(F("position"))) {
    //Serial.print("OFF");
    output_msg = "--- Position ---\n";
    output_msg += "LAT: " + String(gps_latitude, DEC) + "\n";
    output_msg += "LONG: " + String(gps_longitude, DEC) + "\n";
    output_msg += "ALT: " + String(gps_altitude, 1) + "m\n";
    output_msg += "SPEED: " + String(gps_speed, 0) + "km/h\n";
    output_msg += "https://maps.google.com/?q="+String(gps_latitude, DEC)+","+String(gps_longitude, DEC)+"\n";
    //return output_msg;
  }
  else if ( msg.startsWith(F("status")) ) {
    output_msg = "--- Status ---\n";
    output_msg += "Batt: " + String(batt1_voltage, 1) + " V\n";
    //output_msg += "Batt2: " + String(0, 1) + " V\n";
    output_msg += "Temperatur In: " + String(temp_in, 1) + " °C\n";
    output_msg += "Humidity In: " + String(hum_in, 1) + " %\n";
    output_msg += "https://maps.google.com/?q="+String(gps_latitude, DEC)+","+String(gps_longitude, DEC)+"\n";
    //return output_msg;
  }
  else if ( msg.startsWith(F("print")) ) {
    if ( src == SRC_SMS ) {
      output_msg = "command not found\n";
    }
    else {
      output_msg = print_values();
    }
  }
  else if ( msg.startsWith(F("config")) ) {
    if ( src == SRC_SMS ) {
      output_msg = "command not found\n";
    }
    else {
      output_msg = print_config();
    }
  }
  else if ( msg.startsWith(F("tinygsm_info")) ) {
    if ( src == SRC_SMS ) {
      output_msg = "command not found\n";
    }
    else {
      output_msg = tinygsm_info();
    }
  }


  else if ( msg.startsWith(F("save")) ) {
    output_msg = check_plausibility();
    output_msg += "save config\n";
    save_config();
  }

  else {
    output_msg = "command not found\n";
  }

  return output_msg;

}


String print_help() {
  String msg = "fiz-o-matic\n";
  msg += "available commands:\n";
  msg += "  print      -> print all actual values\n";
  msg += "  config     -> show the configuration\n";
  msg += "  go_online  -> go online for 120s\n";
  msg += "  go_offline -> go offline\n";
  msg += "  gsm_info   -> show TinyGSM infos\n";
  msg += "  check      -> check the configuration\n";
  msg += "  save       -> save the configuration\n";
  msg += "\nfiz-o-matic.net\n\n";
  msg += "Version: " +  String(VERSION) + "\n";
  msg += "Build: " + String(__DATE__) + ", " + String(__TIME__) + "\n";

  return msg;
}

String print_help_sms() {
  String msg = "avail. cmds:\n";
  msg += "pos\n";
  msg += "status\n";
  msg += "online\n";
  msg += "offline\n";
  msg += "\nhttps://fiz-o-matic.net/";

  return msg;
}


String print_values() {
    String message;

    message = "#================\n";

    message += ">time:";
    sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
    message += String(buf) + "\n";

    message += ">date:";
    sprintf(buf, "%02d.%02d.20%02d", rtc.getDay(), rtc.getMonth(), rtc.getYear());
    message += String(buf) + "\n";

    message += ">gsm_signal:" + String(gsm_signal, DEC) + " \n";
    message += ">gps_fixstatus:" + String(gps_fixstatus, DEC) + " \n";
    message += ">gps_latitude:" + String(gps_latitude, DEC) + " \n";
    message += ">gps_longitude:" + String(gps_longitude, DEC) + " \n";
    message += ">gps_altitude:" + String(gps_altitude, DEC) + " \n";
    message += ">gps_speed:" + String(gps_speed, 2) + " \n";
    message += ">gps_course:" + String(gps_course, DEC) + " \n";
    message += ">gps_view_satellites:" + String(gps_view_satellites, DEC) + " \n";
    message += ">gps_used_satellites:" + String(gps_used_satellites, DEC) + " \n";
    message += ">gps_distance:" + String(gps_distance, DEC) + " \n";

    message += ">speed:" + String(speed, DEC) + " \n";

    for (int i = 0; i <= (sizeof(values) / sizeof(values[0])) - 1; i++) {
      message += ">" + String(values[i].name) + ":" + String(*values[i].value, 2) + " \n";
    }

    message += "#=== Port Register ===\n";

    for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
      if ( port_values[i].port == 0x00 ) break;


      message += ">0x";
      if (port_values[i].port<16) {
        message += "0";
      }
      message += String(port_values[i].port, HEX);
      message += ":";
      if ( port_values[i].type == TYPE_System ) {
        message += "na";
      }
      else {
        message += String(port_values[i].value, 2);
      }
      message += " \n";
      //Serial.print(F("\n"));
    }
    message += "#================\n";

    return message;
}



String print_config(){
  String message;

  message = "# fiz-o-matic configuration\n";
  message += "#================\n";
  message += "#SIM APN\n";
  message += "sim_pin=" + sim_pin + " \n";
  message += "#Provider APN\n";
  message += "apn=" + apn + " \n";
  message += "#username for the APN\n";
  message += "apn_user=" + apn_user + " \n";
  message += "#password for the APN\n";
  message += "apn_pass=" + apn_pass + " \n";
  message += "#BLYNK authentication key\n";
  message += "blynk_key=" + blynk_key + " \n";
  message += "#SMS Keyword for authentication\n";
  message += "sms_keyword=" + sms_keyword + " \n";
  message += "#My Mobile Number for Notifications\n";
  message += "my_number=" + my_number + " \n";

  message += "\n";
  for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
    message += "# " + String(config[i].desc) + "\n";
    message += String(config[i].name) + "=" + String(*config[i].config, DEC) + "\n";

  }

  // print all features
  message += "\n";
  message += "#================\n";
  message += "#Features\n";
  message += "#================\n";
  for (int i = 0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++){
    message +="# " + String(features[i].desc) + "\n";
    message += String(features[i].name) + "=";
    if (*features[i].feature) {
      message += "true\n";
    }
    else {
      message += "false\n";
    }

  }

  // print ports config
  message += "\n";
  message += "#================\n";
  message += "#Port configuration\n";
  message += "#================\n";
  for (int i = 0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++){
    message += "# " + String(ports[i].desc) + "\n";
    message += String(ports[i].name) + "=0x";
    if (*ports[i].port<16) {
      message += "0";
    }
    message += String(*ports[i].port, HEX) + "\n";
    Serial.println();
  }
  Serial.println(F("#================#"));

  return message;
}
