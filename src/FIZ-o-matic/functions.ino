/***************************************************
    This scetch is used to hold all functions to keep the main sketch clean

    Author: Brun

 ****************************************************/

void trip() {

  if ( trip_timer < millis() ) {
    trip_timer = millis() + 1000;
    //TRACE_PRINTLN(F("#run trip calculation "));

    if ( engine_running ) {
      trip_time = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear()) - engine_start;
      message(DEBUG_MSG, F("Trip - time: "));
      message(DEBUG_MSG, String(trip_time));
      trip_distance = gps_distance;
      message(DEBUG_MSG, F("sec, distance: "));
      message(DEBUG_MSG, String(trip_distance));
      message(DEBUG_MSG, F("m \n"));
    }
    else {
      trip_time = trip_time_last;
    }
  }
}



/*void update_vars() {
  if ( update_vars_timer < millis() ) {
    update_vars_timer = millis() + UPDATE_VARS_TIMER;

    if ( engine_running ) {
      engine_running_sec = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear()) - engine_start;
    }
  }
}*/

/*void print_status() {
#ifdef PRINT_STATUS
  if ( print_status_timer < millis() ) {
    //TRACE_PRINTLN(F("#print_task_callback()"));
    print_status_timer = millis() + PRINT_STATUS_TIMER;

    Serial.write(27);       // ESC command
    Serial.print("[2J");    // clear screen command
    Serial.write(27);
    Serial.print("[H");     // cursor to home command

    Serial.println(F(""));
    Serial.println(F("================"));

    Serial.print(F("DATE: "));
    sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
    Serial.println(buf);

    Serial.print(F("GPS Fix: "));
    Serial.println(gps_fixstatus, DEC);
    Serial.print(F("Latitude: "));
    Serial.println(gps_latitude, DEC);
    Serial.print(F("Longitude: "));
    Serial.println(gps_longitude, DEC);
    Serial.print(F("Altitude: "));
    Serial.println(gps_altitude, DEC);
    Serial.print(F("Speed: "));
    Serial.println(gps_speed, 2);
    Serial.print(F("Course: "));
    Serial.println(gps_course, DEC);
    Serial.print(F("view Satallites: "));
    Serial.println(gps_view_satellites, DEC);
    Serial.print(F("used Satellites: "));
    Serial.println(gps_used_satellites, DEC);
    Serial.print(F("Distance: "));
    Serial.println(gps_distance, DEC);

    Serial.print(F("Port 1 (V): "));
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
    Serial.println(a5_hz, 2);

    Serial.print(F("Speed (km/h): "));
    Serial.println(speed, DEC);

    for (int i = 0; i <= (sizeof(values) / sizeof(values[0])) - 1; i++) {
      Serial.print(values[i].name);
      Serial.print(F(": "));
      Serial.println(*values[i].value);
    }

    Serial.println(F("================"));
  }
#endif
}*/



boolean parse_config_string(String inputString) {

  String tmp;
  boolean found = false;

  inputString.trim();
  message(DEBUG_MSG, F("#"));
  message(DEBUG_MSG, inputString);
  //message(DEBUG_MSG, F("\n"));

  if ( inputString.startsWith("#") ) {
    //SD_DEBUG_PRINTLN(F("found comment"));
    message(TRACE_MSG, F("found comment\n"));
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else if ( inputString.startsWith("sim_pin=") ) {
    sim_pin=getValue( inputString, '=', 1 );
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else if ( inputString.startsWith("apn=") ) {
    apn=getValue( inputString, '=', 1 );
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else if ( inputString.startsWith("apn_user=") ) {
    apn_user=getValue( inputString, '=', 1 );
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else if ( inputString.startsWith("apn_pass=") ) {
    apn_pass=getValue( inputString, '=', 1 );
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else if ( inputString.startsWith("blynk_key=") ) {
    blynk_key=getValue( inputString, '=', 1 );
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else if ( inputString.startsWith("sms_keyword=") ) {
    sms_keyword=getValue( inputString, '=', 1 );
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else {
    for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
      if ( inputString.startsWith(config[i].name) ) {
        tmp = getValue( inputString, '=', 1 );
        *config[i].config = tmp.toInt();
        found = true;
        //message(INFO_MSG, (F("OK\n")));
      }
    }

    for (int i = 0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++){
      if ( inputString.startsWith(ports[i].name) ) {
        tmp = getValue( inputString, '=', 1 );
        *ports[i].port = StrToInt(tmp);
        found = true;
        if ( ports[i].output ) {
          message(DEBUG_MSG, (F(" (as output) ")));
          set_port_output(*ports[i].port);
        }
      }
    }

    for (int i = 0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++){
      if ( inputString.startsWith(features[i].name) ) {
        tmp = getValue( inputString, '=', 1 );
        if ( tmp.startsWith(F("true")) ) {
          *features[i].feature = true;
          //message(INFO_MSG, (F("OK\n")));
        }
        else if ( tmp.startsWith(F("false")) ) {
          *features[i].feature = false;
          //message(INFO_MSG, (F("OK\n")));
        }
        else {
          message(F("#ERROR: use only 'true' or 'false'"));
        }
        found = true;
      }
    }
  }

  if ( found ) {
    message(DEBUG_MSG, (F(" -> OK\n")));
    return true;
  }
  else {
    //Serial.println(F("#error unknown command"));
    return false;
  }

}

int StrToInt(String str)
{

  if ( str.indexOf(F("x")) > 0 ) {
    //Serial.println("it's HEX");
    char tmp[5];
    str.toCharArray(tmp, 5);
    return (int) strtol(tmp, 0, 16);
  }
  else {
    return str.toInt();
  }

}


void check_plausibility() {

  // check BLYNK config if tinygsm is enabled
  if ( tinygsm_enabled ) {
    if ( blynk_key.equals(BLYNK_KEY) ) {
      message( ERROR, F("blynk_key is not set\n"));
      blynk_enabled = false;
    }
    if ( blynk_key.length() < 16) {
      message( ERROR, F("blynk_key valid\n"));
      blynk_enabled = false;
    }
  }
  else {
    blynk_enabled = false;
    sms_enabled = false;
    gps_enabled = false;
    blynk_report = false;
  }

  // if SD Cart isn't enabled, disable logging
  if ( !sd_enabled ) {
    sd_logging = false;
  }

  // checking ports if 1Wire is enabled
  if ( onewire_enabled ) {
    for (int i = 0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++){
      if ( *ports[i].port == ONE_WIRE_BUS_PORT ) {
        message( ERROR, F("1Wire Port isn't free\n"));
        onewire_enabled = false;
      }
    }
  }


}




/*
 *    caclulating distance
 */
float get_distance(float latitude1, float longitude1, float latitude2, float longitude2) {
  float delLat = (latitude2 - latitude1) * 111194.9;
  float delLong = 111194.9 * (longitude2 - longitude1) * cos(radians((latitude2 + latitude1) / 2));
  float distance = sqrt(pow(delLat, 2) + pow(delLong, 2));

  return distance;
}


void save_config() {
  sdcard_save_config();
  write_virtual_eeprom();
}

/*
 * Read the virtual EEPROM
 */
void read_virtual_eeprom() {

  int addr = 0;

  if (EEPROM.isValid()) {
    message(INFO_MSG, F("#read EEPROM!\n"));

    //Serial.print("->");
    message(DEBUG_MSG, F("#config ->"));
    for (int i=0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++) {
      //Serial.print(" ");
      //Serial.print(EEPROM.read(i));
      message(DEBUG_MSG, F(" "));
      message(DEBUG_MSG, String(EEPROM.read(addr), DEC));

      *config[i].config = EEPROM.read(addr);
      addr++;
    }
    //Serial.println();
    /*for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
      *config[i].config = EEPROM.read(i);
    }*/

    message (DEBUG_MSG, F("\n#ports ->"));
    for (int i=0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++) {
      message(DEBUG_MSG, F(" "));
      message(DEBUG_MSG, String(EEPROM.read(addr), DEC));

      *ports[i].port = EEPROM.read(addr);
      addr++;
    }

    message (DEBUG_MSG, F("\n#features ->"));
    for (int i=0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++) {
      message(DEBUG_MSG, F(" "));
      message(DEBUG_MSG, String(EEPROM.read(addr), DEC));

      if ( EEPROM.read(addr) == 1 ) {
        *features[i].feature = true;
      }
      else {
        *features[i].feature = false;
      }
      addr++;
    }

    message (DEBUG_MSG, F("\n"));



    char_config = flash_char_config.read();

    sim_pin = String(char_config.sim_pin);
    apn = String(char_config.apn);
    apn_user = String(char_config.apn_user);
    apn_pass = String(char_config.apn_pass);
    blynk_key = String(char_config.blynk_key);
    sms_keyword = String(char_config.sms_keyword);

    message (DEBUG_MSG, F("\n#sim_pin -> "));
    message (DEBUG_MSG, char_config.sim_pin);
    message (DEBUG_MSG, F("\n#apn -> "));
    message (DEBUG_MSG, char_config.apn);
    message (DEBUG_MSG, F("\n#apn_user -> "));
    message (DEBUG_MSG, char_config.apn_user);
    message (DEBUG_MSG, F("\n#apn_pass -> "));
    message (DEBUG_MSG, char_config.apn_pass);
    message (DEBUG_MSG, F("\n#blynk_key -> "));
    message (DEBUG_MSG, char_config.blynk_key);
    message (DEBUG_MSG, F("\n#sms_keyword -> "));
    message (DEBUG_MSG, char_config.sms_keyword);
    message (DEBUG_MSG, F("\n"));

  } else {
    //display_bootmsg(F("config flash is empty"));
    notify(BOOTMSG, F("config flash is empty"));
  }
}


/*
 * Write virtual EEPROM
 */
void write_virtual_eeprom() {

  int config_i = 0;
  int port_i = 0;
  int addr = 0;
  message(INFO_MSG, F("#Writing config to virtual EEPROM\n"));
  for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
    EEPROM.write(addr, *config[i].config);
    config_i = i;
    addr++;
  }
  config_i++;
  for (int i = 0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++){
    EEPROM.write(addr, *ports[i].port);

    port_i = i;
    addr++;
  }
  port_i++;
  for (int i = 0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++){
    if ( *features[i].feature ) {
      EEPROM.write(addr, 1);
    }
    else {
      EEPROM.write(addr, 0);
    }
    addr++;
  }

  char_config = flash_char_config.read();
  sim_pin.toCharArray( char_config.sim_pin, 6);
  apn.toCharArray( char_config.apn, 36);
  apn_user.toCharArray( char_config.apn_user, 12);
  apn_pass.toCharArray( char_config.apn_pass, 8);
  blynk_key.toCharArray( char_config.blynk_key, 36);
  sms_keyword.toCharArray( char_config.sms_keyword, 36);

  flash_char_config.write(char_config);

  EEPROM.commit();
  message(INFO_MSG, F("#Done!\n"));
  if ( EEPROM.isValid() ) {
    message(INFO_MSG, F("#EEPROM is valid\n"));
  }
  //message(INFO_MSG, F("#isValid() returns "));
  //message(INFO_MSG, String(EEPROM.isValid()));
  //message(INFO_MSG, F("\n"));

  read_virtual_eeprom();

}

/*
 * this function prints a notification message on serial and/or on the display
 * all messeages are printed. the type is only for a logo
 */
void notify(byte type, String msg) {
  if (bitRead(type,0)) {
    //DISPLAY_INFO
    info_type = 1;
    info_text = msg;
    display_notify();
    //MsgTimer = millis() + 10000;
    //dimmer_active_timer = MsgTimer;
    //display_active_timer = MsgTimer;


  }
  if (bitRead(type,1)) {
    //DISPLAY_WARNING
    info_type = 2;
    info_text = msg;
    display_notify();
    //MsgTimer = millis() + 10000;
    //dimmer_active_timer = MsgTimer;
    //display_active_timer = MsgTimer;

  }

  if ( bitRead(type,2) && bitRead(type,3) ) {
    //SOUND_ERROR
  }
  else if (bitRead(type,2)) {
    //SOUND_INFO
  }
  else if (bitRead(type,3)) {
    //SOUND_WARNING
  }

  if (bitRead(type,4)) {
    //BLYNK_MSG
    blynk_msg(msg);
  }
  if (bitRead(type,5)) {
    //BLYNK_PUSH
  }
  if (bitRead(type,6)) {
    //SMS
  }
  if (bitRead(type,7)) {
    //BOOTMSG
    message(msg);
    message(F("\n"));

  }
  //#ifdef U8G2_DISPLAY
  //display_update();
  //#endif // U8G2_DISPLAY

}


/*
   check button state and debounce it

   Funktions...
   1 = short pressed
   2 = long pressed
   3 = long pressed release
   4 = long pressed repeat
   5 = double press?
*/

void button() {

  if ( button_timer < millis() ) {
    button_timer = millis() + BUTTON_TIMER;
    if ( !button_timer_lock ) {
      button_timer_lock = true;

      if (digitalRead(BUTTON_PIN_1) == BUTTON_PRESSED) {
        #ifdef FeatherLED8
        digitalWrite(FeatherLED8, HIGH);
        #endif FeatherLED8

        button_1_low++;
        button_1_high = 0;
        //#ifdef DEBUG
        //TRACE_PRINTLN(F("#Button pressed "));
        //TRACE_PRINTLN(button_1_low, DEC);
        //TRACE_PRINTLN(button_1_high, DEC);
        #ifdef U8G2_DISPLAY
        //display_loop();
        #endif
        //online_intervall_timer = online_intervall_timer + 10000;

        //#endif
      }
      else {
        //digitalWrite(8, LOW);
        button_1_high++;
      }
      // long press
      if ((button_1_low == 10) && (button_1_high == 0)) {
        if (no_long_press) {
          button_1 = 1;
        }
        else {
          button_1 = 2;
        }
        button_1_high = 0;
        //#ifdef DEBUG
        //TRACE_PRINTLN(F("#long press"));
        //#endif
      }
      // long press released
      else if ((button_1_low >= 10) && (button_1_high >= 1)) {
        if (no_long_press) {
          button_1 = 1;
        }
        else {
          button_1 = 3;
        }
        //button_1 = 3;
        button_1_high = 0;
        button_1_low = 0;
        //#ifdef DEBUG
        //TRACE_PRINTLN(F("#long press rleased"));
        //#endif
      }
      // long press repeat
      else if ((button_1_low >= 15) && (button_1_high == 0)) {
        button_1 = 4;
        button_1_high = 0;
        //#ifdef DEBUG
        //TRACE_PRINTLN(F("#long press repeat"));
        //#endif
      }


      /*else if ((button_1_low >= 1) && (button_1_high == 1)) {
        if ( button_1_double == 1 ) {
          button_1 = 6;
          button_1_high=0;
          button_1_low=0;
          button_1_double = 0;
          Serial.println(F("double press"));

        }
        else {
          button_1 = 5;
          button_1_double = 1;
          Serial.println(F("double press??"));
        }
        }*/
      else if ((button_1_low >= 1) && (button_1_high >= 1)) {


        if ( ( !running ) && dimmer_active_timer < millis() ) {
          //button_1 = 0;
          dimmer_active_timer = millis() + 30000;
          display_active_timer = millis() + 30000;
        }
        else {
          button_1 = 1;
          dimmer_active_timer = millis() + 30000;
          display_active_timer = millis() + 30000;
        }

        button_1_high = 0;
        button_1_low = 0;
        button_1_double = 0;
        //Serial.println(F("#short press"));
      }

      button_timer_lock = false;
    }
    //else {
    //  Serial.println(F("#button locked..."));
    //}
  }
}

bool next() {
  if ( button_1 == 1 ) {
    button_1 = 0;
    return true;
  }
  else {
    return false;
  }
}

bool enter() {
  if ( button_1 == 2 ) {
    button_1 = 0;
    return true;
  }
  else {
    return false;
  }

}


/*
 *   Get a Value from s String splitted by a seperator
 */
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


/*
 *   time functions
 */
void set_time(int year, int month, int day, int hour, int minute, int second) {
  boolean secdiff = false;
  if ((second - 5) >= rtc.getSeconds()) {
    secdiff = true;
  }
  else if ((second + 5) <= rtc.getSeconds()) {
    secdiff = true;
  }
  else {
    secdiff = false;
  }
  if ( summer_time ) {
    if ( summertime_EU(year, month, day, hour, 1) ) {
      hour++;
    }
  }

  hour += timezone;
  if ( hour >= 24 ) {
    hour -= 24;
    day++;
  }

  if ( secdiff || (minute != rtc.getMinutes()) || (hour != rtc.getHours()) || (day != rtc.getDay()) || (month != rtc.getMonth()) || ((year - 2000) != rtc.getYear()) ) {
    //message(DEBUG_MSG, F("#--->Adjust RTC\n"));
    message(DEBUG_MSG, F("#--->Adjust RTC "));
    message(DEBUG_MSG, String(unixTime(hour, minute, second, day, month, year), DEC));
    message(DEBUG_MSG, F(" -> "));
    message(DEBUG_MSG, String(unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear() + 2000), DEC));
    message(DEBUG_MSG, F("\n"));

    //rtc.adjust(DateTime(gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second));
    // Set the time
    rtc.setHours(hour);
    rtc.setMinutes(minute);
    rtc.setSeconds(second);

    // Set the date
    rtc.setDay(day);
    rtc.setMonth(month);
    rtc.setYear(year - 2000);
  }
}

/*
   Summer time Yes or No
*/
boolean summertime_EU(int year, byte month, byte day, byte hour, byte tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
{
  if (month < 3 || month > 10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (month > 3 && month < 10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  if (month == 3 && (hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7)) || month == 10 && (hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7)))
    return true;
  else
    return false;
}

/*
   converting a normal time into unix time
   based on http://christianscode.blogspot.de/2015/07/arduino-timestamp-function.html

   Function extracted from the library http://playground.arduino.cc/Code/Time
   if you just need the function without compiling few kbs and you need to save
   space, this is the way to go

*/
long unixTime(int hr, int min, int sec, int dy, int mnth, int yr) {
  // assemble time elements into time_t
  // note year argument is offset from 1970 (see macros in time.h to convert to other formats)
  // previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9

  // year can be given as full four digit year or two digts (2010 or 10 for 2010);
  //it is converted to years since 1970
  if ( yr > 99)
    yr = yr - 1970;
  else
    yr += 30;

  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = yr * (SECS_PER_DAY * 365);
  for (i = 0; i < yr; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }

  // add days for this year, months start from 1
  for (i = 1; i < mnth; i++) {
    if ( (i == 2) && LEAP_YEAR(yr)) {
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i - 1]; //monthDay array starts from 0
    }
  }
  seconds += (dy - 1) * SECS_PER_DAY;
  seconds += hr * SECS_PER_HOUR;
  seconds += min * SECS_PER_MIN;
  seconds += sec;
  //TRACE_PRINT(F("#UnixTime: "));
  //TRACE_PRINTLN(seconds);
  return (long)seconds;
}



/*
 *   Calculating free RAM
 */
#ifdef ARDUINO_ARCH_SAMD
extern "C" char *sbrk(int i);

int freeRam () {
  char stack_dummy = 0;
  return &stack_dummy - sbrk(0);
}
#else //ARDUINO_ARCH_SAMD
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif //ARDUINO_ARCH_SAMD
