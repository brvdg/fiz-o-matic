/***************************************************
 *  This sketch is to check serveral status information and react with events.
 *
 *  Author: Brun
 *
 ****************************************************/

void status_checker() {
  if ( status_checker_timer < millis() ) {

    check_engine();

    /*
     * functions based on tinygsm
     */
    if ( tinygsminit ) {
      check_alarm_system();
      check_geo_fence();
      check_online();
    }

    if ( engine_running ) {
      engine_running_sec = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear()) - engine_start;
    }

    status_checker_timer = millis() + STATUS_CHECKER_TIMER;
  }
}


void check_engine() {
  if (bord_voltage > 4) {
    if (!engine_running) {


      message(DEBUG_MSG, F("#--->Start Engine\n"));
      engine_running = true;
      engine_start = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear());

      if ( alarm_system_armed ) {
        //message(DEBUG_MSG, F("#alarm is armed\n"));
        check_alarm_system();

      }
      else {
        #ifdef U8G2_DISPLAY


        MainMenuPos = 2;
        display_update();


        #endif //U8G2_DISPLAY

        if ( online ) {
          message(DEBUG_MSG, F("#go offline\n"));
          if ( tinygsm_go_offline() ) {
            online = false;
          }
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

      #ifdef U8G2_DISPLAY
      MainMenuPos = 1;
      display_update();
      #endif //U8G2_DISPLAY

      engine_running = false;
      engine_running_total += engine_running_sec;

      trip_time_last = trip_time;

      close_file();

      // check if light is still on
      if ( dimmer_V > 2 ) {
        notify(DISPLAY_WARNING + SOUND_INFO, F("Licht noch an"));
        set_alarm(150, 150, 3, true);
      }

      //online_intervall_timer = millis() + 10000;
      tinygsm_go_online();

      // disable geo fenece
      if ( geo_fence_distance != 0 ) {
        geo_fence_enabled = true;
      }
      else {
        blynk_msg(F("geo fence is disabled"));
      }
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

      if ( tinygsm_go_online() ) {
        online = true;
      }

      set_alarm(600, 200, 5, true);
      tinygsm_alarm();

      #ifdef U8G2_DISPLAY
      MainMenuPos = 1;
      display_update();
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
          tinygsm_alarm();
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
  if ( enable_blynk != 1 ) return;
  if ( !blynk_enabled ) return;


  // online intervall
  if ( !stay_online && !engine_running && !alarm_system_triggered && !geo_fence_alarm) {
    if ( online_intervall_timer < millis() ) {
      message(DEBUG_MSG, F("#check_online state\n"));
      //Serial.println("#");
      if (!online) {

        tinygsm_go_online();
        /*message(INFO_MSG, F("#going online\n"));
        if (tinygsm_go_online()) {
          online = true;
          online_intervall_timer = millis() + 10000;
        }*/
      }
      else {

        tinygsm_go_offline();
        /*message(INFO_MSG, F("#going offline\n"));
        if (tinygsm_go_offline()) {
          online = false;
          online_intervall_timer = millis() + online_interval * 60000;
        }*/
      }
    }
  }

  //check the online state
  if (online) {
    Blynk.run();
    if (!Blynk.connected()) {
      message(INFO_MSG, F("BLYNK connection was lost\n"));
      blynk_offline_counter++;
      if ( blynk_offline_counter >= 100 ) {
        tinygsm_go_offline();
        delay(2000);
        tinygsm_go_online();
        blynk_offline_counter = 0;
        blynk_msg(F("BLYNK connection was lost"));
      }
    }
  }
}
