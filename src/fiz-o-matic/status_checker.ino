/****************************************************
 * FIZ-o-matic
 * https://fiz-o-matic.net/
 *
 * This sketch is to check serveral status information and react with events.
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/




bool fuel_notified = false;

void status_checker() {
  //if ( status_checker_timer < millis() ) {

    check_engine();

    /*
     * functions based on tinygsm
     */
    if ( tinygsminit ) {
      check_alarm_system();
      check_geo_fence();
      check_online();
    }
    else{
      message(TRACE_MSG, F("#tinygsminit=false\n"));
    }

    if ( engine_running ) {
      engine_running_sec = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear()) - engine_start;
    }

    check_water_temp();
    check_fuel();

    check_auxiliary_heating();

    status_checker_timer = millis() + STATUS_CHECKER_TIMER;
    delay(STATUS_CHECKER_TIMER);
  //}
}


void check_engine() {
  if (bord_voltage > 4) {
    display_active_timer = millis() + 30000;
    if (!engine_running) {

      go_online = false;

      message(DEBUG_MSG, F("#--->Start Engine\n"));
      engine_running = true;
      engine_start = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear());

      if ( alarm_system_armed ) {
        //message(DEBUG_MSG, F("#alarm is armed\n"));
        check_alarm_system();

      }
      else {
        #ifdef ENABLE_DISPLAY
        MainMenuPos = 1;
        //display_update();
        #endif //U8G2_DISPLAY

        if ( online ) {
          message(DEBUG_MSG, F("#go offline\n"));
          /*if ( tinygsm_go_offline() ) {
            online = false;
          }*/
          go_offline = true;
        }
      }

      // SD Card functions
      open_file();
      log_to_sdcard();

    }
  }
  else {
    if (engine_running) {
      engine_running = false;

      message(DEBUG_MSG, F("#--->Stop Engine\n"));

      NotifyActive = false;

      #ifdef ENABLE_DISPLAY
      MainMenuPos = 1;
      //display_update();
      #endif //ENABLE_DISPLAY

      engine_running = false;
      engine_running_total += engine_running_sec;
      engine_running_sec = 0;

      //trip_time_last = trip_time;
      //trip_time_all += trip_time;

      close_file();

      // check if light is still on
      if ( dimmer_V > 2 ) {
        //notify(DISPLAY_WARNING + SOUND_INFO, F("Licht noch an"));
        display_message = F("Licht");
        set_alarm(150, 150, 3, true);
      }

      //online_intervalll_timer = millis() + 10000;
      //tinygsm_go_online();
      display_active_timer = millis() + 30000;
      //go_online = true;

      // disable geo fenece
      if ( geo_fence_distance != 0 ) {
        geo_fence_enabled = true;
      }
      else {
        blynk_msg(F("geo fence is disabled"));
      }

      // reset notification states
      ignore_notify = false;
      fuel_notified = false;

    }
  }
}

/*
 * Alarm System
 */
void check_alarm_system() {
  if (alarm_system_armed && !alarm_system_triggered) {
    // at started engine
    if (engine_running) {
      message(INFO_MSG ,F("#--->ALARM!!! -Engine-\n"));
      alarm_system_triggered = true;

      /*if ( tinygsm_go_online() ) {
        online = true;
      }*/
      //go_online=true;

      //set_alarm(600, 200, 5, true);
      //tinygsm_sms_alarm();
      notify(SMS + BLYNK_PUSH, "Engine started!!!");

      #ifdef U8G2_DISPLAY
      MainMenuPos = 1;
      //display_update();
      #endif
    }
  }

  // block display
  if ( alarm_system_armed ) {
    #ifdef U8G2_DISPLAY
    MainMenuPos = 1;
    //display_update();
    //message(INFO_MSG, F("Alarm is amed\n"));
    #endif
  }

}

/*
 * Geo Fence
 */
void check_geo_fence() {
  if (geo_fence_enabled && (gps_fixstatus = 1) ) {
    if ( !geo_fence_armed ) {
      geo_fence_armed = true;
      gps_latitude_geo_fence = gps_latitude;
      gps_longitude_geo_fence = gps_longitude;
    }
    else {
      int tmp = get_distance(gps_latitude, gps_longitude, gps_latitude_geo_fence, gps_longitude_geo_fence);
      if ( !geo_fence_alarm ) {
        if ( ( tmp >= geo_fence_distance ) && ( tmp < 100000 ) ) {
          geo_fence_alarm = true;
          message(INFO_MSG, F("GEO FENCE ALARM "));
          message(INFO_MSG, String(tmp, DEC));
          message(INFO_MSG, F(" m\n"));
          blynk_msg(String(tmp, DEC));
          tinygsm_sms_alarm();
        }
      }
      else {
        if ( tmp <= geo_fence_distance ) {
          geo_fence_alarm = false;
          message(INFO_MSG, F("back in geo fence\n"));
          blynk_msg(F("back in geo fence"));
        }
      }
    }
  }
}

/*
 * check if it's time to go online or to stay onlie or to go offline
 */
void check_online() {

  // exit if BLYNK is disabled
  //if ( enable_blynk != 1 ) return;
  if ( !blynk_enabled ) return;
  if ( !tinygms_ok ) return;

  //message(DEBUG_MSG, F("#check_online\n"));

  // online intervall


  if ( !stay_online && !engine_running && !alarm_system_triggered && !geo_fence_alarm && ( display_active_timer < millis() ) ) {
    if ( online_intervalll_timer < millis() ) {
      message(DEBUG_MSG, F("#check_online state\n"));
      if (!online) {
        go_online = true;
      }
      else {
        go_offline = true;
      }
    }
  }
  else {
    if ( stay_online ) {
      //message(TRACE_MSG, F("#stay online (stay_online=true)\n"));
    }
    else if ( engine_running ) {
      //message(TRACE_MSG, F("#stay offline (engine_running=true)\n"));
    }
    else if ( alarm_system_triggered ) {
      message(TRACE_MSG, F("#stay online (alarm_system_triggered=true)\n"));
    }
    else if ( geo_fence_alarm ) {
      message(TRACE_MSG, F("#stay online (geo_fence_alarm=true)\n"));
    }
  }

  //check the online state
  /*if (online) {
    Blynk.run();
    if (!Blynk.connected()) {
      message(INFO_MSG, F("BLYNK connection was lost\n"));
      blynk_offline_counter++;
      if ( blynk_offline_counter >= 100 ) {
        message(INFO_MSG, F("try to reset BLYNK connection\n"));
        //tinygsm_go_offline();
        go_offline = true;
        //delay(2000);
        //tinygsm_go_online();
        go_online = true;
        blynk_offline_counter = 0;
        blynk_msg(F("BLYNK connection was lost"));
      }
    }
  }*/
}


void check_water_temp() {
  if ( water_temp != 255 ) {
    if ( water_temp > water_temp_warning ) {
      //sprintf (buf, "%3d \xf7 C", int(water_temp));
      //buf += (char)247;
      //buf += "C";
      //info_val = buf;

      /*for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
        if (port_values[i].name = F("water_temp")){
          info_val = i;
          break;
        }
      }*/
      info_val = int(water_temp);
      info_unit = "dgC";

      notify(DISPLAY_WARNING + SOUND_INFO, F("Wassertemperatur"));
    }
    else if ( millis() <= MsgTimer ) {
      info_val = int(water_temp);
    }
  }
}

void check_fuel() {
  if ( ( fuel_l < 10 ) && (!fuel_notified) ){
    notify(DISPLAY_INFO + SOUND_INFO, F("TANKEN!"));
    fuel_notified = true;
  }
}
