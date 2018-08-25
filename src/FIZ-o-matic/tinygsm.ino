/***************************************************
    This sketch is for TinyGSM and BLYNK communication.
    https://github.com/vshymanskyy/TinyGSM

    Author: Brun

 ****************************************************/


//#ifdef TinyGSM

// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30

//#define BLYNK_PRINT Serial // Defines the object that is used for printing
//#define BLYNK_DEBUG        // Optional, this enables more detailed prints
//#define TINY_GSM_DEBUG Serial

//#define DEBUG_PRINT(x)  Serial.print (x)
//#define DEBUG_PRINTLN(x)  Serial.println (x)


// Select your modem:
//#define TINY_GSM_MODEM_SIM800
//#define TINY_GSM_MODEM_SIM808
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_M590

#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 2048
#endif

// Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1

TinyGsm modem(Serial1);

WidgetTerminal terminal(BLYNK_VIRTUAL_terminal);
WidgetMap myMap(BLYNK_VIRTUAL_map);
WidgetLED geo_fence_armed_led(BLYNK_VIRTUAL_geo_fence_armed_led);
WidgetLED geo_fence_led(BLYNK_VIRTUAL_geo_fence_led);
WidgetLED online_LED(BLYNK_VIRTUAL_online_LED);
WidgetLED armed_led(BLYNK_VIRTUAL_armed_led);
WidgetLED alarm_led(BLYNK_VIRTUAL_alarm_led);

boolean blynk = false;
byte i = 0;


unsigned long tinygsm_gps_timer = 0;
unsigned long tinygsm_sms_timer = 0;
unsigned long tinygsm_blynk_timer = 0;





void tinygsm_init()
{

  if ( !tinygsm_enabled ) {
    notify(BOOTMSG, F("TinyGSM disabled"));
    return;
  }


  notify(BOOTMSG, F("Initializing TinyGSM"));
  //TINYGSM_DEBUG_PRINTLN(F("#tinygsm_init"));
  // Set GSM module baud rate
  Serial1.begin(115200);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  modem.restart();


  // Unlock your SIM card with a PIN
  /*message(F("sim_pin: "));
  message(sim_pin);
  message(F(" length: "));
  message(String(sim_pin.length()));
  message(F("\n"));

  if (sim_pin.length() != 0) {
    message(DEBUG_TINYGSM, F("#Unlock SIM card \n"));

    sim_pin.trim();
    char tmp_sim_pin[sim_pin.length() + 1];
    sim_apn.toCharArray(tmp_sim_pin, sim_pin.length() + 1);

    if ( modem.simUnlock(tmp_sim_pin) ) {
      message(DEBUG_TINYGSM, F("#SIM card unlocked\n"));
    }
    else {
      message(DEBUG_TINYGSM, F("#SIM card unlock FAILED\n"));
    }
    delay(3000);
  }*/


  switch (modem.getSimStatus()) {
    case 0: {
      message(F("#SIM ERROR\n"));
      notify(DISPLAY_INFO, F("SIM ERROR"));
      break;
    }
    case 1: {
      message(F("#SIM READY\n"));
      //notify(DISPLAY_INFO, F("SIM ERROR"));
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

  /*message(DEBUG_TINYGSM, F("#Waiting for network...\n"));
  if (!modem.waitForNetwork()) {
    //delay(10000);
    //return;
  }

  if (modem.isNetworkConnected()) {
    message(DEBUG_TINYGSM, F("#Network connected\n"));
  }
  else {
    message(DEBUG_TINYGSM, F("#Network NOT connected\n"));

  }*/


  /*if (modem.getSimStatus() == 0) {
    enable_blynk = 0;
    //sim_ok = false;
    tinygms_ok = false;
    message(F("#SIM ERROR\n"));
    notify(DISPLAY_INFO, F("SIM ERROR"));

    //notify(2, F("TinyGSM failed"));
  }
  else {
    message(F("#SIM OK\n"));
    tinygms_ok = true;
  }*/

  //Blynk.begin(BLYNK_KEY, modem, SIM_APN, SIM_USER, SIM_PASS);

  /*String ccid = modem.getSimCCID();
  DBG("CCID:", ccid);
  message(DEBUG_TINYGSM, F("CCID: "));
  message(DEBUG_TINYGSM, ccid);

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);
  message(DEBUG_TINYGSM, F("\nIMEI: "));
  message(DEBUG_TINYGSM, imei);

  String cop = modem.getOperator();
  DBG("Operator:", cop);
  message(DEBUG_TINYGSM, F("\nOperator: "));
  message(DEBUG_TINYGSM, cop);*/

  /*IPAddress local = modem.localIP();
  DBG("Local IP:", local);
  message(DEBUG_TINYGSM, F("\nLocal IP: "));
  message(DEBUG_TINYGSM, String(local));*/

  /*int csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);
  message(DEBUG_TINYGSM, F("\nSignal quality: "));
  message(DEBUG_TINYGSM, String(csq, DEC));

  // This is only supported on SIMxxx series
  String gsmLoc = modem.getGsmLocation();
  DBG("GSM location:", gsmLoc);
  message(DEBUG_TINYGSM, F("\nGSM location: "));
  message(DEBUG_TINYGSM, gsmLoc);
  message(DEBUG_TINYGSM, F("\n\n"));*/


  //tinygsm_gps_init();
  /*display_bootmsg(F("enable GPS"));
  if ( !modem.enableGPS() ) {
    INFO_PRINTLN(F("can't enable GPS"));
  }
  else {
    TINYGSM_DEBUG_PRINTLN(F("GPS enabled"));
  }*/


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

  online_intervall_timer = millis() + 10000;

  tinygsminit = true;


  /*reg_port(0xcb);
  reg_port(0xcc);
  reg_port(0xA0);*/

}


void tinygsm_info() {

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
  message(F("\n\n"));

}

void tinygsm_gps_init() {

  if ( !gps_enabled ) {
    notify(BOOTMSG, F("GPS is disabled"));
    return;
  }
  notify(BOOTMSG, F("enable GPS"));
  if ( !modem.enableGPS() ) {
    //INFO_PRINTLN(F("can't enable GPS"));
    message(DEBUG_TINYGSM, F("can't enable GPS\n"));
    notify(2, F("GPS failed"));
    tinygsm_gps_ok = false;
  }
  else {
    //TINYGSM_DEBUG_PRINTLN(F("GPS enabled"));
    message(DEBUG_TINYGSM, F("GPS is enabled\n"));
    tinygsm_gps_ok = true;
  }
  reg_port(0x10, TYPE_kmh);

}


void tinygsm_loop()
{
  if ( !tinygms_ok ) return;

  // Check SMS
  if ( ( tinygsm_sms_timer < millis() ) && ( sim_ok ) ) {
    tinygsm_sms_timer = millis() + TinyGSM_SMS_TIMER;
    //TINYGSM_DEBUG_PRINTLN(F("#checking sms"));
    message(TRACE_MSG, F("#checking sms\n"));
    tinygsm_sms();

    gsm_signal= modem.getSignalQuality();
    /*message(F("Signal quality: "));
    message(String(gsm_signal, DEC));
    message(F("\n"));*/
  }


  if ( tinygsm_blynk_timer < millis() ) {
    if (online) {
      if ( gps_fixstatus ) {
        //TINYGSM_DEBUG_PRINTLN(F("#update Blynk location"));
        message(DEBUG_TINYGSM, F("#update Blynk location\n"));
        // Update position if it's more then 10m
        if ( get_distance(gps_latitude, gps_longitude, gps_latitude_blynk, gps_longitude_blynk) >= 10 ) {
          //myMap.location(1, 52.4161, 9.66569, BLYNK_DEVICE_NAME);
          myMap.location(1, gps_latitude, gps_longitude, BLYNK_DEVICE_NAME);
          message(DEBUG_TINYGSM, F("#location is set\n"));
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);

          //Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, 52.4161);
          //Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, 9.6656);
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, gps_latitude);
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, gps_longitude);
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_altitude, gps_altitude);

          gps_latitude_blynk = gps_latitude;
          gps_longitude_blynk = gps_longitude;
        }
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

      Blynk.run();
    }
    tinygsm_blynk_timer = millis() + TinyGSM_BLYNK_TIMER;
  }







  // Check GPS position
  /*if ( tinygsm_gps_timer < millis() ) {
    tinygsm_gps_timer = millis() + TinyGSM_GPS_TIMER;
    TRACE_PRINTLN(F("#get location"));

    gps_view_satellites = 0;
    gps_used_satellites = 0;
    gps_fixstatus = modem.getGPS(&gps_latitude, &gps_longitude, &gps_speed, &gps_altitude, &gps_view_satellites, &gps_used_satellites);
    if ( gps_fixstatus ) {

      int year, month, day, hour, minute, second;
      if ( modem.getGPSTime(&gps_year, &gps_month, &gps_day, &gps_hour, &gps_minute, &gps_second) ) {
        //Sync time if it's diffrent.
        set_time(gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second);
      }

      if (gps_latitude_old == 0) {
        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }

      float distance = get_distance(gps_latitude, gps_longitude, gps_latitude_old, gps_longitude_old);
#ifdef OFFLINE
      gps_distance += (int)(distance + .5);
      //gps_distance_trip += (int)(distance + .5);
#endif

      if (gps_speed > 10) {
        gps_distance += (int)(distance + .5);

        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }

      if (online) {
        if ( tinygsm_blynk_timer < millis() ) {
          TINYGSM_DEBUG_PRINTLN(F("#update Blynk location"));

          // Update position if it's more then 10m
          if ( get_distance(gps_latitude, gps_longitude, gps_latitude_blynk, gps_longitude_blynk) >= 10 ) {
            //myMap.location(1, 52.4161, 9.66569, BLYNK_DEVICE_NAME);
            myMap.location(1, gps_latitude, gps_longitude, BLYNK_DEVICE_NAME);
            TINYGSM_DEBUG_PRINTLN(F("#location is set"));
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);

            //Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, 52.4161);
            //Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, 9.6656);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, gps_latitude);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, gps_longitude);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_altitude, gps_altitude);

            gps_latitude_blynk = gps_latitude;
            gps_longitude_blynk = gps_longitude;
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

          Blynk.run();

          tinygsm_blynk_timer = millis() + TinyGSM_BLYNK_TIMER;
        }
      }


      gps_fixerrcnt = 0;
    }
    else {
      TINYGSM_DEBUG_PRINT(F("#gps not fix (cnt: "));
      TINYGSM_DEBUG_PRINT(gps_fixerrcnt);
      TINYGSM_DEBUG_PRINTLN(F(" )"));

      if ( gps_view_satellites == 0 ) {
        gps_fixerrcnt++;
      }

      if (gps_fixerrcnt >= 100) {
        TINYGSM_DEBUG_PRINTLN(F("#try to reset GPS"));
        if ( !modem.disableGPS() ) {
          INFO_PRINTLN(F("can't disable GPS"));
        }
        else {
          TINYGSM_DEBUG_PRINTLN(F("GPS disabled"));
        }
        delay(3000);
        if ( !modem.enableGPS() ) {
          INFO_PRINTLN(F("can't enable GPS"));
        }
        else {
          TINYGSM_DEBUG_PRINTLN(F("GPS enabled"));
        }
        gps_fixerrcnt = 0;
      }

      if (online) {
        if (gps_used_satellites != gps_used_satellites_blynk) {
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
          gps_used_satellites_blynk = gps_used_satellites;
        }
        if (gps_view_satellites != gps_view_satellites_blynk) {
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);
          gps_view_satellites_blynk = gps_view_satellites;
        }
        Blynk.run();
      }
    }
  }*/
}


void tinygsm_gps_loop() {
  if ( !tinygsm_gps_ok ) return;

  // Check GPS position
  if ( tinygsm_gps_timer < millis() ) {
    tinygsm_gps_timer = millis() + TinyGSM_GPS_TIMER;
    //TRACE_PRINTLN(F("#get location"));

    gps_view_satellites = 0;
    gps_used_satellites = 0;
    gps_fixstatus = modem.getGPS(&gps_latitude, &gps_longitude, &gps_speed, &gps_altitude, &gps_view_satellites, &gps_used_satellites);
    if ( gps_fixstatus ) {

      update_port_value( 0x10, gps_speed);

      int year, month, day, hour, minute, second;
      if ( modem.getGPSTime(&gps_year, &gps_month, &gps_day, &gps_hour, &gps_minute, &gps_second) ) {
        //Sync time if it's diffrent.
        set_time(gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second);

        // register the values
        //update_port_value(0xCC, (gps_year*10000) + (gps_month*100) + gps_day);
        //update_port_value(0xCB, (gps_hour*10000) + (gps_minute*100) + gps_hour);
        //update_port_value(0xA0, gps_speed);

      }

      if (gps_latitude_old == 0) {
        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }

      float distance = get_distance(gps_latitude, gps_longitude, gps_latitude_old, gps_longitude_old);
#ifdef OFFLINE
      gps_distance += (int)(distance + .5);
      //gps_distance_trip += (int)(distance + .5);
#endif

      if (gps_speed > 10) {
        gps_distance += (int)(distance + .5);

        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }



      gps_fixerrcnt = 0;
    }
    else {
      message(TRACE_MSG, F("#gps not fix (cnt: "));
      message(TRACE_MSG, String(gps_fixerrcnt, DEC));
      message(TRACE_MSG, F(" )\n"));

      if ( gps_view_satellites == 0 ) {
        gps_fixerrcnt++;
      }

      if (gps_fixerrcnt >= 254) {
        message(DEBUG_TINYGSM, F("#try to reset GPS\n"));
        if ( !modem.disableGPS() ) {
          message(DEBUG_TINYGSM, F("can't disable GPS\n"));
        }
        else {
          message(DEBUG_TINYGSM, F("GPS disabled\n"));
        }
        delay(3000);
        if ( !modem.enableGPS() ) {
          message(DEBUG_TINYGSM, F("can't enable GPS\n"));
        }
        else {
          message(DEBUG_TINYGSM, F("GPS enabled\n"));
        }
        gps_fixerrcnt = 0;
      }
    }
  }
}

void tinygsm_sms() {

  // exit if SIM is not OK
  if ( !tinygms_ok ) return;

  int numofsms = modem.getNumSMS();

  char sender[18];

  //message(DEBUG_TINYGSM, F("#Read SMS\n"));
  for (int i = 1; i < numofsms + 1; i++) {
    message(DEBUG_TINYGSM, F("Read SMS# "));
    message(DEBUG_TINYGSM, String(i, DEC));
    String smsmsg = modem.readSMS(i, sender);
    smsmsg.toLowerCase();
    //    sender.replace('"', 0);
    message(DEBUG_TINYGSM, F("#Sender: "));
    message(DEBUG_TINYGSM, sender);
    message(DEBUG_TINYGSM, F("Message: "));
    //TINYGSM_DEBUG_PRINTLN(F("##############################"));
    message(DEBUG_TINYGSM, smsmsg);
    //TINYGSM_DEBUG_PRINTLN(F("##############################"));

    // Check if keyword present
    if ( smsmsg.indexOf(SMS_Keyword) >= 0 ) {
      if ( smsmsg.indexOf(F("on")) >= 0 ) {
        if ( tinygsm_go_online() ) {
          Blynk.notify(F("Hello. Now I'm online!"));
          online = true;
        }
        else  {
          modem.sendSMS(sender, F("Coldn't connect to Blynk Server"));
        }
        modem.deleteSMS(i);
      }
      else if ( smsmsg.indexOf("off") >= 0 ) {
        Blynk.notify(F("disconnect Blynk"));

        if ( tinygsm_go_offline() ) {
          online = false;
          modem.deleteSMS(i);
          message(DEBUG_TINYGSM,F("Disconnected\n"));
        }
      }
    }
    else {
      modem.deleteSMS(i);
    }
  }
}

boolean tinygsm_go_online() {

  message(INFO_MSG, F("#going online\n"));

  blynk_key.trim();
  char tmp_blynk_key[blynk_key.length() + 1];
  blynk_key.toCharArray(tmp_blynk_key, blynk_key.length() + 1);

  sim_apn.trim();
  char tmp_sim_apn[sim_apn.length() + 1];
  sim_apn.toCharArray(tmp_sim_apn, sim_apn.length() + 1);
  //Serial.println(sim_apn.length(), DEC);
  //Serial.print(tmp_sim_apn);

  sim_user.trim();
  char tmp_sim_user[sim_pass.length() + 1];
  sim_user.toCharArray(tmp_sim_user, sim_user.length() + 1);

  sim_pass.trim();
  char tmp_sim_pass[sim_pass.length() + 1];
  sim_pass.toCharArray(tmp_sim_pass, sim_pass.length() + 1);

  message(TRACE_MSG, F("#tmp_blynk_key: "));
  message(TRACE_MSG, tmp_blynk_key);
  message(TRACE_MSG, F("\n"));
  message(TRACE_MSG, F("#tmp_sim_apn: "));
  //message(TRACE_MSG, String(tmp_sim_apn));
  message(TRACE_MSG, sim_apn);
  message(TRACE_MSG, F("\n"));
  message(TRACE_MSG, F("#tmp_sim_user: "));
  message(TRACE_MSG, tmp_sim_user);
  message(TRACE_MSG, F("\n"));
  message(TRACE_MSG, F("#tmp_sim_pass: "));
  message(TRACE_MSG, tmp_sim_pass);
  message(TRACE_MSG, F("\n"));


  /*if ( !blynk_enabled ) {
    message( ERROR, F("ERROR: Blynk not enabled\n"));
    return false;
  }*/

  message(TRACE_MSG, F("#Blynk.begin... \n"));
  message(TRACE_MSG, F("#If blynk_key isn't correct, it never comes back! \n"));
  Blynk.begin(tmp_blynk_key, modem, tmp_sim_apn, tmp_sim_user, tmp_sim_pass);
  //Blynk.begin(BLYNK_KEY, modem, SIM_APN, SIM_USER, SIM_PASS);
  message(TRACE_MSG, F("#Blynk.begin finished \n"));


  //message(INFO_MSG, F("#online?\n"));

  if (Blynk.connect()) {
    blynk_msg(F("Blynk is now online"));
    message(INFO_MSG, F("#now online\n"));

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

    online_intervall_timer = millis() + 10000;
    online = true;
    blynk = true;
    return true;
  }
  else {
    return false;
  }
}

boolean tinygsm_go_offline() {
  message(INFO_MSG, F("#going offline\n"));
  online_LED.off();
  delay(1000);

  Blynk.disconnect();
  if (modem.gprsDisconnect()) {
    online = false;
    online_intervall_timer = millis() + online_interval * 60000;
    return true;
  }
  else {
    if (!modem.gprsDisconnect()) {
      if (!modem.gprsDisconnect()) {
        message(INFO_MSG ,F("TinyGSM couldn't go offline\n"));
        return false;
      }
    }
  }
}


void tinygsm_sleep() {
  message(INFO_MSG ,F("TinyGSM go into sleep mode...\n"));

  modem.disableGPS();

  delay(5000);
}

void tinygsm_wakeup() {
  message(INFO_MSG ,F("#wakeup TinyGSM\n"));


  modem.enableGPS();



  delay(5000);
}


void tinygsm_alarm() {
  message(INFO_MSG ,F("#tinygsm_alarm\n"));
  //modem.sendSMS(MYNUMBER, F("ALARM!!!"));
  Blynk.notify(F("ALARM!!!"));

  blynk_msg(F("ALARM!!!"));

  alarm_led.on();

  if (geo_fence_alarm) {
    geo_fence_led.on();
  }
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


/*
 *   Blynk Functions
 */
BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_APP_DISCONNECTED() {
  blynk_msg(F("app disconected"));
}

BLYNK_APP_CONNECTED() {
  blynk_msg(F("app conected"));
}


BLYNK_WRITE(BLYNK_VIRTUAL_stay_online) {
  int value = param.asInt(); // Get value as integer


  if ( value == 1 ) {
    if ( !stay_online ) {
      stay_online = true;
      blynk_msg(F("stay_online turned on"));
      message(DEBUG_TINYGSM, F("#stay_online turned off"));
    }
  }
  else {
    if ( stay_online ) {
      stay_online = false;
      blynk_msg(F("stay_online turned off"));
      message(DEBUG_TINYGSM, F("#stay_online turned off"));
      online_intervall_timer = millis() + 10000;
    }
  }
}

BLYNK_WRITE(BLYNK_VIRTUAL_geo_fence_distance)
{
  int value = param.asInt(); // Get value as integer
  if ( value != geo_fence_distance ) {
    message(INFO_MSG ,F("#set geo-fance distance to "));
    message(INFO_MSG ,String(value));
    message(INFO_MSG ,F("\n"));
    value += 5;
    value = ( value / 10 ) * 10;
    geo_fence_distance = value;
    Blynk.virtualWrite(BLYNK_VIRTUAL_geo_fence_distance, value);

    if ( geo_fence_distance != 0 ) {
      blynk_msg_float(F("geo fance distance set to: "), value);
    }
    else {
      blynk_msg(F("geo fence disabled"));
    }
  }
}

BLYNK_WRITE(BLYNK_VIRTUAL_alarm) {
  int value = param.asInt(); // Get value as integer
  if ( value == 1 ) {
    //blynk_alarm = true;
    if ( !alarm_system_armed ) {
      alarm_system_armed = true;
      armed_led.on();
      blynk_msg(F("alarm_system turned on"));
    }
  }
  else {
    if ( alarm_system_armed ) {
      alarm_system_armed = false;
      alarm_system_triggered = false;
      armed_led.off();
      alarm_led.off();
      blynk_msg(F("alarm_system turned off"));
    }
  }
}

//#endif // TinyGSM
