/****************************************************
 * FIZ-o-matic
 * https://fiz-o-matic.net/
 *
 * This sketch is for TinyGSM and BLYNK communication.
 * https://github.com/vshymanskyy/TinyGSM
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/


#ifdef TinyGSM

//#define TINY_GSM_YIELD() { delay(10); yield(); }

#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 2048
#endif

/*
 * Blynk definations
 */

#ifdef DEBUG
  #define TINY_GSM_DEBUG Serial
  #define BLYNK_PRINT Serial // Defines the object that is used for printing
  #define BLYNK_DEBUG        // Optional, this enables more detailed prints
#endif

//#define TINY_GSM_YIELD() { delay(10); yield(); }

// Select your modem:
//#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_M590

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>


TinyGsm modem(Serial1);



boolean blynk = false;
byte i = 0;


//unsigned long gps_timer = 0;



void tinygsm_init()
{
  // Module baud rate
  uint32_t rate = 0; // Set to 0 for Auto-Detect

  message(TINYGSM_MSG, F("#Initializing TinyGSM\n"));
  if ( !tinygsm_enabled ) {
    message(TINYGSM_MSG, F("TinyGSM disabled\n"));
    return;
  }

  pinMode(TinyGSM_PWRKEY, OUTPUT);
  digitalWrite(TinyGSM_PWRKEY, LOW);
  message(TINYGSM_MSG, F("Power down\n"));
  delay(1000);
  digitalWrite(TinyGSM_PWRKEY, HIGH);
  message(TINYGSM_MSG, F("Power up\n"));
  delay(3000);
  //digitalWrite(TinyGSM_PWRKEY, LOW);
  //message(TINYGSM_MSG, F("#Initializing TinyGSM\n"));
  //delay(20000);


  Serial1.begin(115200);
  if (!modem.init()) {
    message(TINYGSM_MSG, F("Cannot initialize modem!"));

    rate = TinyGsmAutoBaud(Serial1);

    Serial1.begin(rate);
    //delay(10000);
    message(TINYGSM_MSG, F("Modem Baud Rate: "));
    message(TINYGSM_MSG, String(rate, DEC));
    message(TINYGSM_MSG, F("\n"));

    if ( rate != 115200 ) {
      tinygsm_factory_reset();
    }

    Serial1.begin(115200);
  }

  /*
  Serial1.begin(rate);
  //delay(10000);
  message(F("#Modem Baud Rate: "));
  message(String(rate, DEC));
  message(F("\n"));

  if (!modem.init()) {
    message(F("**********************************************************\n"));
    message(F(" Cannot initialize modem!\n"));
    message(F("**********************************************************\n"));


    digitalWrite(TinyGSM_PWRKEY, HIGH);
    delay(1500);
    digitalWrite(TinyGSM_PWRKEY, LOW);
    delay(1500);
    //digitalWrite(TinyGSM_PWRKEY, HIGH);
    //delay(1000);

    TinyGsmAutoBaud(Serial1);
  }
  */


  // we have a connection so we do a reset to the modem
  modem.restart();

  switch (modem.getSimStatus()) {
    case 0: {
      message(F("#SIM ERROR\n"));
      notify(DISPLAY_INFO, F("SIM ERROR"));
      break;
    }
    case 1: {
      message(F("#SIM READY\n"));
      tinygms_ok = true;
      break;
    }
    case 2: {
      message(F("#SIM LOCKED\n"));
      notify(DISPLAY_INFO, F("SIM LOCKED"));
      break;
    }
    default: message(F("#UNKNOWN SIM ERROR\n")); break;
  }

  modem.begin();


  // go online if we defined it
  #ifdef ONLINE_ON_STANDBY
  stay_online = true;
  notify(BOOTMSG, F("going online"));
  #endif

  if ( stay_online ) {
    if ( tinygsm_go_online() ) {
      blynk_msg(F("Blynk is initialized"));
      online = true;
      notify(BOOTMSG, F("online"));
      Blynk.notify(F("online"));
    }
  }

  online_intervalll_timer = millis() + 10000;

  tinygsminit = true;

}


String tinygsm_info() {

  String msg;

  msg = "=== TinyGSM Info ===\n";

  msg += "CCID: " + modem.getSimCCID() + "\n";
  msg += "IMEI: " + modem.getIMEI() + "\n";
  msg += "Operator: " + modem.getOperator() + "\n";
  IPAddress local = modem.localIP();
  msg += "Local IP: " + String(local[0]) + "." + String(local[1]) + "." + String(local[2]) + "." + String(local[3]) + "\n";
  msg += "Signal quality: " + String(modem.getSignalQuality(), 0) + "\n";
  msg += "GSM location: " + modem.getGsmLocation() + "\n";

  return msg;

  /*
  String ccid = modem.getSimCCID();
  message(F("CCID: "));
  message(ccid);

  String imei = modem.getIMEI();
  message(F("\nIMEI: "));
  message(imei);

  String cop = modem.getOperator();
  message(F("\nOperator: "));
  message(cop);

  IPAddress local = modem.localIP();
  message(F("\nLocal IP: "));
  message(String(local[0]) + "." + String(local[1]) + "." + String(local[2]) + "." + String(local[3]));

  int csq = modem.getSignalQuality();
  message(F("\nSignal quality: "));
  message(String(csq, DEC));

  // This is only supported on SIMxxx series
  String gsmLoc = modem.getGsmLocation();
  message(F("\nGSM location: "));
  message(gsmLoc);
  message(F("\n\n"));*/

}

void tinygsm_gps_init() {

  if ( !gps_enabled ) {
    notify(BOOTMSG, F("#GPS is disabled"));
    return;
  }
  notify(BOOTMSG, F("#enable GPS"));
  if ( !modem.enableGPS() ) {
    //INFO_PRINTLN(F("can't enable GPS"));
    message(TINYGSM_MSG, F("#can't enable GPS\n"));
    //notify(2, F("GPS failed"));
    notify(DISPLAY_INFO, F("GPS failed"));
    tinygsm_gps_ok = false;
  }
  else {
    //TINYGSM_DEBUG_PRINTLN(F("GPS enabled"));
    message(TINYGSM_MSG, F("#GPS is enabled\n"));
    tinygsm_gps_ok = true;
  }
  reg_port(0x10, TYPE_kmh);

}


void tinygsm_loop()
{

  tinygsm_gps_loop();


  if ( !tinygms_ok ) return;
  if ( tinygsm_lock ) {
    delay(100);
    return;
  }
  /**
  tinygsm_gps_loop();
  if ( tinygsm_timer > millis() ) {
    yield();
    return;
  }
  tinygsm_timer = millis() + TinyGSM_TIMER;
  **/



  // return if timer is not reached
  if ( !timer_check(&tinygsm_timer, TinyGSM_TIMER) ) return;

  // disable functions until the dislay is not active
  /*if ( display_active_timer > millis() &&  alarm_system_triggered == false ) {
    yield();
    return;
  }*/

  tinygsm_lock = true;

  gsm_signal = modem.getSignalQuality();

  // should we go online or offline?
  if ( go_offline ) {
    message(TINYGSM_MSG, F("#go_offline is set\n"));
    tinygsm_go_offline();
  }

  if ( go_online ) {
    message(TINYGSM_MSG, F("#go_online is set\n"));
    //display_loop();
    tinygsm_go_online();
    message(TINYGSM_MSG, F("#go_online is finished\n"));
  }



  // Check SMS
  if ( timer_check(&tinygsm_sms_timer, TinyGSM_SMS_TIMER) ) {
    message(TINYGSM_MSG, F("checking sms"));
    tinygsm_sms();
  }


  if ( tinygsm_blynk_timer < millis() ) {
    message(TINYGSM_MSG, F("tinygsm_blynk_timer"));
    if (online) {
      if ( gps_fixstatus ) {
        //TINYGSM_DEBUG_PRINTLN(F("#update Blynk location"));
        message(TINYGSM_MSG, F("#update Blynk location"));
        // Update position if it's more then 10m
        if ( get_distance(gps_latitude, gps_longitude, gps_latitude_blynk, gps_longitude_blynk) >= 10 ) {
          //myMap.location(1, 52.4161, 9.66569, BLYNK_DEVICE_NAME);
          myMap.location(1, gps_latitude, gps_longitude, BLYNK_DEVICE_NAME);
          message(TINYGSM_MSG, F("#location is set"));
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);

          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, gps_latitude);
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, gps_longitude);
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_altitude, gps_altitude);

          gps_latitude_blynk = gps_latitude;
          gps_longitude_blynk = gps_longitude;
        }
      }

      // send a push message
      if ( blynk_push_msg != "" ){
        Blynk.notify(blynk_push_msg);
        blynk_push_msg = "";
      }


      if (gps_used_satellites != gps_used_satellites_blynk) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
        gps_used_satellites_blynk = gps_used_satellites;
      }
      if (gps_view_satellites != gps_view_satellites_blynk) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);
        gps_view_satellites_blynk = gps_view_satellites;
      }
      if (gps_altitude != gps_altitude_blynk) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_gps_altitude, gps_altitude);
        gps_altitude_blynk = gps_altitude;
      }


      // seting something in the Blynk App
      if ( aux_heater_status == STATE_wait_blynk_off) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_aux_heater, LOW);
        aux_heater_status = STATE_off;
      }

      // seting Temperature and Humidity
      if ( temp_out_port != 0x00) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_temp_out, temp_out);
      }
      if ( hum_in_port != 0x00) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_hum_out, hum_out);
      }
      if ( temp_in_port != 0x00) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_temp_in, temp_in);
      }
      if ( hum_in_port != 0x00) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_hum_in, hum_in);
      }

      Blynk.virtualWrite(BLYNK_VIRTUAL_bord_voltage, bord_voltage);


      Blynk.run();
    }
    tinygsm_blynk_timer = millis() + TinyGSM_BLYNK_TIMER;
  }
  tinygsm_lock = false;
  //message(TRACE_MSG, F("#tinygsm_loop - tinygsm_lock = false\n"));
  //delay(TinyGSM_TIMER);
  yield();
}


void tinygsm_gps_loop() {
  if ( !tinygsm_gps_ok ) return;

  if ( tinygsm_lock ) {
    message(TRACE_MSG, F("#tinygsm_lock = locked"));
    delay(100);
    return;
  }

  //message(INFO_MSG, "tinygsm_gps_loop");

  if ( !timer_check(&gps_timer, GPS_TIMER) ) return;
  //message(INFO_MSG, "GPS_TIMER");

  tinygsm_lock = true;

  float lat2      = 0;
  float lon2      = 0;
  float speed2    = 0;
  float alt2      = 0;
  int   vsat2     = 0;
  int   usat2     = 0;
  float accuracy2 = 0;
  int   year2     = 0;
  int   month2    = 0;
  int   day2      = 0;
  int   hour2     = 0;
  int   min2      = 0;
  int   sec2      = 0;


  gps_view_satellites = 0;
  gps_used_satellites = 0;

  gps_fixstatus = false;
  //gps_fixstatus = modem.getGPS(&gps_latitude, &gps_longitude, &gps_speed, &gps_altitude, &gps_view_satellites, &gps_used_satellites);

   //Serial.println("RAW:");
  //Serial.println(modem.getGPSraw());
  gps_fixstatus = modem.getGPS(&gps_latitude, &gps_longitude, &speed2, &alt2, &gps_view_satellites, &gps_used_satellites, &accuracy, &gps_year, &gps_month, &gps_day, &gps_hour, &gps_minute, &gps_second);
  //gps_fixstatus = false;
  gps_speed = short(speed2);
  gps_altitude = short(alt2);

  if ( gps_fixstatus ) {

    update_port_value( 0x10, gps_speed);

    //int year, month, day, hour, minute, second;
    /*if ( modem.getGPSTime(&gps_year, &gps_month, &gps_day, &gps_hour, &gps_minute, &gps_second) ) {
      //Sync time if it's diffrent.
      set_time(gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second);
    }*/

    set_time(gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second);

    if (gps_latitude_old == 0) {
      gps_latitude_old = gps_latitude;
      gps_longitude_old = gps_longitude;
    }

    float distance = get_distance(gps_latitude, gps_longitude, gps_latitude_old, gps_longitude_old);

    if (gps_speed > 10) {
      gps_distance += (int)(distance + .5);

      gps_latitude_old = gps_latitude;
      gps_longitude_old = gps_longitude;
    }



    gps_fixerrcnt = 0;

    /*String msg = F("gps fix (cnt: ");
    msg += String(gps_fixerrcnt, DEC);
    msg += F(", view: ");
    msg += String(gps_view_satellites, DEC);
    msg += F(", used: ");
    msg += String(gps_used_satellites, DEC);
    msg += F(" )");
    message(GPS, msg );*/
  }
  else {
    String msg = F("not fix (cnt: ");
    msg += String(gps_fixerrcnt, DEC);
    msg += F(", view: ");
    msg += String(gps_view_satellites, DEC);
    msg += F(", used: ");
    msg += String(gps_used_satellites, DEC);
    msg += F(" )");
    message(GPS, msg );

    /*

    message(TRACE_MSG, F("#gps not fix (cnt: ") );
    message(TRACE_MSG, String(gps_fixerrcnt, DEC));
    message(TRACE_MSG, F(", view: "));
    message(TRACE_MSG, String(gps_view_satellites, DEC));
    message(TRACE_MSG, F(", used: "));
    message(TRACE_MSG, String(gps_used_satellites, DEC));
    message(TRACE_MSG, F(" )\n"));
    */

    if ( gps_view_satellites == 0 ) {
      gps_fixerrcnt++;
    }

  }

  tinygsm_lock = false;
  yield();
}


boolean tinygsm_go_online() {

  // reset the request
  go_online = false;

  if ( !blynk_enabled ) return false;

  message(TINYGSM_MSG, F("#going online"));

  blynk_key.trim();
  char tmp_blynk_key[blynk_key.length() + 1];
  blynk_key.toCharArray(tmp_blynk_key, blynk_key.length() + 1);

  //if ( blynk_server.length() >= 6 ) {
    blynk_server.trim();
    char tmp_blynk_server[blynk_server.length() + 1];
    blynk_server.toCharArray(tmp_blynk_server, blynk_server.length() + 1);

    blynk_port.trim();
    int tmp_blynk_port = blynk_port.toInt();

  //}


  apn.trim();
  char tmp_apn[apn.length() + 1];
  apn.toCharArray(tmp_apn, apn.length() + 1);
  //Serial.println(apn.length(), DEC);
  //Serial.print(tmp_apn);

  apn_user.trim();
  char tmp_apn_user[apn_pass.length() + 1];
  apn_user.toCharArray(tmp_apn_user, apn_user.length() + 1);

  apn_pass.trim();
  char tmp_apn_pass[apn_pass.length() + 1];
  apn_pass.toCharArray(tmp_apn_pass, apn_pass.length() + 1);

  /*message(TRACE_MSG, F("#tmp_blynk_key: "));
  message(TRACE_MSG, tmp_blynk_key);
  message(TRACE_MSG, F("\n"));
  message(TRACE_MSG, F("#tmp_apn: "));
  //message(TRACE_MSG, String(tmp_apn));
  message(TRACE_MSG, apn);
  message(TRACE_MSG, F("\n"));
  message(TRACE_MSG, F("#tmp_apn_user: "));
  message(TRACE_MSG, tmp_apn_user);
  message(TRACE_MSG, F("\n"));
  message(TRACE_MSG, F("#tmp_apn_pass: "));
  message(TRACE_MSG, tmp_apn_pass);
  message(TRACE_MSG, F("\n"));*/


  /*if ( !blynk_enabled ) {
    message( ERROR, F("ERROR: Blynk not enabled\n"));
    return false;
  }*/

  message(TINYGSM_MSG, F("Blynk.begin... "));
  message(TINYGSM_MSG, F("If blynk_key isn't correct, it never comes back! \n"));

  if ( blynk_server.length() >= 6 ) {
    Blynk.begin(tmp_blynk_key, modem, tmp_apn, tmp_apn_user, tmp_apn_pass, tmp_blynk_server, tmp_blynk_port);
  }
  else {
    Blynk.begin(tmp_blynk_key, modem, tmp_apn, tmp_apn_user, tmp_apn_pass);
  }
  //Blynk.begin(BLYNK_KEY, modem, SIM_APN, SIM_USER, apn_pass);
  //Blynk.begin(tmp_blynk_key, modem, tmp_apn, tmp_apn_user, tmp_apn_pass, server_address, server_port);
  message(TINYGSM_MSG, F("Blynk.begin finished \n"));


  //message(INFO_MSG, F("#online?\n"));

  if (Blynk.connect()) {
    blynk_msg(F("Blynk is now online"));
    message(TINYGSM_MSG, F("now online"));

    online_LED.on();

    if (flash_watchdog_reset.read()) {
      if (blynk_report) {
        Blynk.notify(F("Watchdog reset..."));
      }
      flash_watchdog_reset.write(false);
    }

    if (startmsg) {
      if (blynk_report) {
        Blynk.notify(F("booted..."));
      }
      startmsg = false;
    }


    online_intervalll_timer = millis() + 10000;
    online = true;
    blynk = true;
    return true;
  }
  else {
    return false;
  }
  message(TINYGSM_MSG, F("go_online finished"));
}

boolean tinygsm_go_offline() {

  // reset the request
  go_offline = false;

  message(TINYGSM_MSG, F("going offline"));
  online_LED.off();
  delay(1000);

  Blynk.disconnect();
  if (modem.gprsDisconnect()) {
    online = false;
    online_intervalll_timer = millis() + online_intervall * 60000;
    return true;
  }
  else {
    if (!modem.gprsDisconnect()) {
      if (!modem.gprsDisconnect()) {
        message(TINYGSM_MSG, F("TinyGSM couldn't go offline"));
        return false;
      }
    }
  }
}


/**void tinygsm_sleep() {
  message(INFO_MSG ,F("TinyGSM go into sleep mode...\n"));

  modem.disableGPS();

  delay(5000);
}

void tinygsm_wakeup() {
  message(INFO_MSG ,F("#wakeup TinyGSM\n"));


  modem.enableGPS();



  delay(5000);
}**/

void tinygsm_factory_reset() {
  modem.sendAT(GF("+IPR?"));
  //if (modem.waitResponse() != 1) return;
  //if (modem.waitResponse(10000L, GF(GSM_NL "+IPR?")) != 1) return;
  message(modem.stream.readStringUntil('OK'));


  bool ret = modem.factoryDefault();

  message(F("***********************************************************\n"));
  message(F(" Return settings to Factory Defaults: "));
  message((ret) ? "OK\n" : "FAIL\n");
  message(F("***********************************************************\n"));

  message(F("Set default Baudrate to 115200Baud\n"));
  modem.sendAT(GF("+IPR=115200"));
  delay(2000);
  modem.sendAT(GF("&W_SAVE"));
  //if (modem.waitResponse() != 1) return;
  //if (modem.waitResponse(5000L, GF(GSM_NL "+IPR:")) != 1) return;
  //message(modem.stream.readStringUntil('\n'));
  message(modem.stream.readStringUntil('OK'));
  message(F("check baudrate\n"));
  modem.sendAT(GF("+IPR?"));
  //if (modem.waitResponse() != 1) return;
  //if (modem.waitResponse(10000L, GF(GSM_NL "+IPR?")) != 1) return;
  message(modem.stream.readStringUntil('OK'));
  delay(2000);
  modem.sendAT(GF("&W_SAVE"));
  message(modem.stream.readStringUntil('OK'));

  message(F("\n***********************************************************\n"));

}




void tinygsm_set_baud() {
  //while ( !tinygsm_lock ) yield();
  tinygsm_lock = true;


  message(F("check baudrate"));
  modem.sendAT(GF("&V"));
  //if (modem.waitResponse() != 1) return;
  if (modem.waitResponse(10000L, GF(GSM_NL "+IPR:")) != 1) return;
  message(modem.stream.readStringUntil('\n'));

  modem.sendAT(GF("+IPR=115200"));
  delay(2000);
  modem.sendAT(GF("&W_SAVE"));
  //if (modem.waitResponse() != 1) return;
  //if (modem.waitResponse(5000L, GF(GSM_NL "+IPR:")) != 1) return;
  //message(modem.stream.readStringUntil('\n'));

  //message(F("#check baudrate 2\n"));
  //modem.sendAT(GF("&V"));
  //if (modem.waitResponse() != 1) return;
  //if (modem.waitResponse(10000L, GF(GSM_NL "+IPR:")) != 1) return;
  //message(modem.stream.readStringUntil('\n'));

  tinygsm_lock = false;
}




void blynk_msg( String msg ) {
  sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  terminal.print(buf);
  terminal.print(F(" - "));
  terminal.println(msg);
  terminal.flush();
}

void blynk_msg_float( String msg, float value ) {
  sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  terminal.print(buf);
  terminal.print(F(" - "));
  terminal.print(msg);
  terminal.println(value);
  terminal.flush();
}




#else

void tinygsm_init() {};
void tinygsm_gps_init() {};

void tinygsm_loop() {};
void blynk_msg(String) {};
String tinygsm_info() {};
void tinygsm_set_baud() {};
void tinygsm_alarm() {};

#endif // TinyGSM
