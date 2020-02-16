/***************************************************
    This sketch is for TinyGSM and BLYNK communication.
    https://github.com/vshymanskyy/TinyGSM

    Author: Brun

 ****************************************************/


#ifdef TinyGSM

//#define TINY_GSM_YIELD() { delay(10); yield(); }

#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 2048
#endif


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


//unsigned long tinygsm_gps_timer = 0;
unsigned long tinygsm_sms_timer = 0;
unsigned long tinygsm_blynk_timer = 0;





void tinygsm_init()
{
  notify(BOOTMSG, F("#Initializing TinyGSM"));
  if ( !tinygsm_enabled ) {
    notify(BOOTMSG, F("TinyGSM disabled"));
    return;
  }


  Serial1.begin(115200);
  delay(3000);

  if (!modem.init()) {
    message(F("***********************************************************\n"));
    message(F(" Cannot initialize modem!\n"));
    message(F("***********************************************************\n"));


    //digitalWrite(TinyGSM_RESET, HIGH);
    //delay(1500);
    digitalWrite(TinyGSM_PWRKEY, LOW);
    delay(1500);
    digitalWrite(TinyGSM_PWRKEY, HIGH);
    delay(1000);

    TinyGsmAutoBaud(Serial1);
  }


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
    notify(BOOTMSG, F("#GPS is disabled"));
    return;
  }
  notify(BOOTMSG, F("#enable GPS"));
  if ( !modem.enableGPS() ) {
    //INFO_PRINTLN(F("can't enable GPS"));
    message(TINYGSM, F("#can't enable GPS\n"));
    //notify(2, F("GPS failed"));
    notify(DISPLAY_INFO, F("GPS failed"));
    tinygsm_gps_ok = false;
  }
  else {
    //TINYGSM_DEBUG_PRINTLN(F("GPS enabled"));
    message(TINYGSM, F("#GPS is enabled\n"));
    tinygsm_gps_ok = true;
  }
  reg_port(0x10, TYPE_kmh);

}


void tinygsm_loop()
{

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

  tinygsm_gps_loop();

  if ( !timer_check(&tinygsm_timer, TinyGSM_TIMER) ) return;

  // disable functions until the dislay is not active
  if ( display_active_timer > millis() &&  alarm_system_triggered == false ) {
    yield();
    return;
  }

  tinygsm_lock = true;

  // should we go online or offline?
  if ( go_offline ) {
    message(TINYGSM, F("#go_offline is set\n"));
    tinygsm_go_offline();
  }

  if ( go_online ) {
    message(TINYGSM, F("#go_online is set\n"));
    tinygsm_go_online();
    message(TINYGSM, F("#go_online is finished\n"));
  }



  // Check SMS
  if ( timer_check(&tinygsm_sms_timer, TinyGSM_SMS_TIMER) ) {
    message(DEBUG_MSG, F("#checking sms\n"));
    tinygsm_sms();
  }


  if ( tinygsm_blynk_timer < millis() ) {
    message(DEBUG_MSG, F("#tinygsm_blynk_timer\n"));
    if (online) {
      if ( gps_fixstatus ) {
        //TINYGSM_DEBUG_PRINTLN(F("#update Blynk location"));
        message(TINYGSM, F("#update Blynk location\n"));
        // Update position if it's more then 10m
        if ( get_distance(gps_latitude, gps_longitude, gps_latitude_blynk, gps_longitude_blynk) >= 10 ) {
          //myMap.location(1, 52.4161, 9.66569, BLYNK_DEVICE_NAME);
          myMap.location(1, gps_latitude, gps_longitude, BLYNK_DEVICE_NAME);
          message(TINYGSM, F("#location is set\n"));
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);

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
    message(TRACE_MSG, F("#tinygsm_lock = locked\n"));
    delay(100);
    return;
  }


  if ( !timer_check(&tinygsm_gps_timer, TinyGSM_GPS_TIMER) ) return;

  tinygsm_lock = true;

  gps_view_satellites = 0;
  gps_used_satellites = 0;
  gps_fixstatus = modem.getGPS(&gps_latitude, &gps_longitude, &gps_speed, &gps_altitude, &gps_view_satellites, &gps_used_satellites);
  if ( gps_fixstatus ) {

    update_port_value( 0x10, gps_speed);

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

    if (gps_speed > 10) {
      gps_distance += (int)(distance + .5);

      gps_latitude_old = gps_latitude;
      gps_longitude_old = gps_longitude;
    }



    gps_fixerrcnt = 0;
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

void tinygsm_sms() {

  // exit if SIM is not OK
  if ( !tinygms_ok ) return;

  int numofsms = modem.getNumSMS();


  char sender[18];

  message(TINYGSM, F("#Read SMS\n"));
  for (int i = 1; i < numofsms + 1; i++) {
    message(TINYGSM, F("Read SMS# "));
    message(TINYGSM, String(i, DEC));
    String smsmsg = modem.readSMS(i, sender);
    smsmsg.toLowerCase();
    //    sender.replace('"', 0);
    message(TINYGSM, F("#Sender: "));
    message(TINYGSM, sender);
    message(TINYGSM, F("Message: "));
    //TINYGSM_DEBUG_PRINTLN(F("##############################"));
    message(TINYGSM, smsmsg);
    //TINYGSM_DEBUG_PRINTLN(F("##############################"));

    // Check if keyword present
    if ( smsmsg.indexOf(SMS_Keyword) >= 0 ) {
      if ( smsmsg.indexOf(F("on")) >= 0 ) {
        go_online = true;
        modem.deleteSMS(i);
      }
      else if ( smsmsg.indexOf("off") >= 0 ) {
        Blynk.notify(F("disconnect Blynk"));

        if ( tinygsm_go_offline() ) {
          online = false;
          modem.deleteSMS(i);
          message(TINYGSM,F("Disconnected\n"));
        }
      }
    }
    else {
      modem.deleteSMS(i);
    }
  }
}

boolean tinygsm_go_online() {

  // reset the request
  go_online = false;

  message(INFO_MSG, F("#going online\n"));

  blynk_key.trim();
  char tmp_blynk_key[blynk_key.length() + 1];
  blynk_key.toCharArray(tmp_blynk_key, blynk_key.length() + 1);

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

  message(TRACE_MSG, F("#tmp_blynk_key: "));
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
  message(TRACE_MSG, F("\n"));


  /*if ( !blynk_enabled ) {
    message( ERROR, F("ERROR: Blynk not enabled\n"));
    return false;
  }*/

  message(TRACE_MSG, F("#Blynk.begin... \n"));
  message(TRACE_MSG, F("#If blynk_key isn't correct, it never comes back! \n"));
  Blynk.begin(tmp_blynk_key, modem, tmp_apn, tmp_apn_user, tmp_apn_pass);
  //Blynk.begin(BLYNK_KEY, modem, SIM_APN, SIM_USER, apn_pass);
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

    online_intervalll_timer = millis() + 10000;
    online = true;
    blynk = true;
    return true;
  }
  else {
    return false;
  }
  message(TRACE_MSG, F("#go_online finished\n"));
}

boolean tinygsm_go_offline() {

  // reset the request
  go_offline = false;

  message(INFO_MSG, F("#going offline\n"));
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
        message(INFO_MSG ,F("TinyGSM couldn't go offline\n"));
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

void tinygsm_set_baud() {
  //while ( !tinygsm_lock ) yield();
  tinygsm_lock = true;


  message(F("#check baudrate\n"));
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
  message(TRACE_MSG ,F("#BLYNK_CONNECTED\n"));
  Blynk.syncAll();
}

BLYNK_APP_DISCONNECTED() {
  message(TRACE_MSG ,F("#BLYNK_APP_DISCONNECTED\n"));
  blynk_msg(F("app disconected"));
}

BLYNK_APP_CONNECTED() {
  message(TRACE_MSG ,F("#BLYNK_APP_CONNECTED\n"));
  blynk_msg(F("app conected"));
}


BLYNK_WRITE(BLYNK_VIRTUAL_stay_online) {
  message(TRACE_MSG ,F("#BLYNK_WRITE(BLYNK_VIRTUAL_stay_online)\n"));
  int value = param.asInt(); // Get value as integer


  if ( value == 1 ) {
    if ( !stay_online ) {
      stay_online = true;
      blynk_msg(F("stay_online turned on"));
      message(TINYGSM, F("#stay_online turned off"));
    }
  }
  else {
    if ( stay_online ) {
      stay_online = false;
      blynk_msg(F("stay_online turned off"));
      message(TINYGSM, F("#stay_online turned off"));
      online_intervalll_timer = millis() + 10000;
    }
  }
}

BLYNK_WRITE(BLYNK_VIRTUAL_geo_fence_distance) {
  message(TRACE_MSG ,F("#BLYNK_WRITE(BLYNK_VIRTUAL_geo_fence_distance)\n"));
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
  message(TRACE_MSG ,F("#BLYNK_WRITE(BLYNK_VIRTUAL_alarm)\n"));
  /*while ( tinygsm_loop_running ) {
    message(TRACE_MSG ,F("#waiting for a free slot\n"));
    delay(500);
    yield();
  }
  tinygsm_loop_running = true;*/
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
  //tinygsm_loop_running = false;
}

BLYNK_WRITE(BLYNK_VIRTUAL_aux_heater) {
  message(TRACE_MSG ,F("#BLYNK_WRITE(BLYNK_VIRTUAL_aux_heater)\n"));
  int value = param.asInt(); // Get value as integer


  if ( value == 1 ) {
    if ( aux_heater_status == STATE_off ) {
      set_auxiliary_heating(true);
    }
  }
  else {
    if ( aux_heater_status == STATE_on ) {
      set_auxiliary_heating(false);
    }
  }
}
#endif // TinyGSM
