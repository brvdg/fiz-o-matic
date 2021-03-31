/****************************************************
 * FIZ-o-matic
 * https://fiz-o-matic.net/
 *
 * This scetch is used to hold all functions to keep the main sketch clean
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/




void trip() {
  if ( timer_check(&trip_timer, 10000) ) {
    if ( engine_running ) {
      trip_time = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear()) - engine_start;
      trip_distance = gps_distance;

      String msg = F("Trip - time: ");
      msg += String(trip_time, DEC);
      msg += F("sec, distance: ");
      msg += String(trip_distance, DEC);
      msg += F("m");
      message(DEBUG_MSG, msg );
    }
    /*else {
      trip_time = trip_time_last;
    }*/
  }
}




boolean parse_config_string(String inputString) {

  String tmp;
  boolean found = false;

  inputString.trim();
  message(DEBUG_MSG, F("#"));
  message(DEBUG_MSG, inputString);

  if ( inputString.startsWith("#") ) {
    message(TRACE_MSG, F("found comment\n"));
    found = true;
  }
  else if ( inputString.startsWith("sim_pin=") ) {
    sim_pin=getValue( inputString, '=', 1 );
    found = true;
  }
  else if ( inputString.startsWith("apn=") ) {
    apn=getValue( inputString, '=', 1 );
    found = true;
  }
  else if ( inputString.startsWith("apn_user=") ) {
    apn_user=getValue( inputString, '=', 1 );
    found = true;
  }
  else if ( inputString.startsWith("apn_pass=") ) {
    apn_pass=getValue( inputString, '=', 1 );
    found = true;
  }
  else if ( inputString.startsWith("blynk_key=") ) {
    blynk_key=getValue( inputString, '=', 1 );
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else if ( inputString.startsWith("blynk_server=") ) {
    blynk_server=getValue( inputString, '=', 1 );
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else if ( inputString.startsWith("blynk_port=") ) {
    blynk_port=getValue( inputString, '=', 1 );
    found = true;
    //message(INFO_MSG, (F("OK\n")));
  }
  else if ( inputString.startsWith("sms_keyword=") ) {
    sms_keyword=getValue( inputString, '=', 1 );
    found = true;
  }
  else if ( inputString.startsWith("my_number=") ) {
    my_number=getValue( inputString, '=', 1 );
    found = true;
  }
  else {
    for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
      if ( inputString.startsWith(config[i].name) ) {
        tmp = getValue( inputString, '=', 1 );
        *config[i].config = tmp.toInt();
        found = true;
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
        }
        else if ( tmp.startsWith(F("false")) ) {
          *features[i].feature = false;
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
    return false;
  }

}

int StrToInt(String str)
{

  if ( str.indexOf(F("x")) > 0 ) {
    char tmp[5];
    str.toCharArray(tmp, 5);
    return (int) strtol(tmp, 0, 16);
  }
  else {
    return str.toInt();
  }

}


String check_plausibility() {

  String msg = "";

  if ( tinygsm_enabled ) {
    if ( blynk_key.equals(BLYNK_KEY) ) {
      //message( ERROR, F("blynk_key is not set\n"));
      msg += "blynk_key is not set -> disable Blynk\n";
      blynk_enabled = false;
    }
    if ( blynk_key.length() < 16) {
      //message( ERROR, F("blynk_key invalid -> disable Blynk\n"));
      msg += "blynk_key invalid -> disable Blynk\n";
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
        //msg += "1Wire Port isn't free\n"
        onewire_enabled = false;
      }
    }
  }


  return msg;
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


void read_config() {
  message(STORAGE, F("read configuration"));

  #ifdef SPIFLASH
  spiflash_open_config();
  #elif SDCARD
  sdcard_open_config();
  #else
  read_virtual_eeprom();
  #endif

}

void save_config() {
  message(STORAGE, F("save configuration"));
  #ifdef SPIFLASH
  message(STORAGE, F("to: SPIFLASH"));
  spiflash_save_config();
  #elif SDCARD
  sdcard_save_config();
  message(STORAGE, F("to: SDCARD"));
  #else
  write_virtual_eeprom();
  message(STORAGE, F("to: VIRTUAL EEPROM"));
  #endif
}




/*
 * this function prints a notification message on serial and/or on the display
 * all messeages are printed. the type is only for a logo
 */
void notify(byte type, String msg) {
  message(ALARM, msg);

  if (bitRead(type,0)) {
    //DISPLAY_INFO
    info_type = 1;
    info_text = msg;
    //display_notify();
    MsgTimer = millis() + 10000;
    display_active_timer = MsgTimer;
    //display_active_timer = MsgTimer;


  }
  if (bitRead(type,1)) {
    //DISPLAY_WARNING
    info_type = 2;
    info_text = msg;
    //display_notify();
    MsgTimer = millis() + 10000;
    display_active_timer = MsgTimer;
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
    go_online=true;
    blynk_push_msg = msg;
    //BLYNK_PUSH
  }
  if (bitRead(type,6)) {

    tinygsm_send_sms(msg);
  }
  if (bitRead(type,7)) {
    //BOOTMSG
    message(NOTIFY, msg);
    //message(F("\n"));

  }
  //#ifdef U8G2_DISPLAY
  //display_update();
  //#endif // U8G2_DISPLAY

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
