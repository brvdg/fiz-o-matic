/***************************************************
 *  This sketch is for featrues functions.
 *  possible functions right now:
 *  - auxiliary heating
 *
 *  Author: Brun
 *
 ****************************************************/





void set_auxiliary_heating(boolean status) {
  if ( aux_heating_port != 0x00 ) {
    if ( status ) {
      update_port_value(aux_heating_port, 1);
      notify(DISPLAY_INFO + BLYNK_MSG, F("Heizung an"));
      aux_heater_status = STATE_on;
      //aux_heater_timer = millis() * 60000 * 10; // 10 min.
      aux_heater_timer = millis() + 1000 * aux_heating_time;
      //blynk_msg(F("auxiliary heating turned on"));
      notify(BLYNK_MSG, F("auxiliary heating turned on"));
    }
    else {
      update_port_value(aux_heating_port, 0);
      notify(DISPLAY_INFO + BLYNK_MSG, F("Heizung aus"));
      aux_heater_status = STATE_wait_blynk_off;
    }
  }
  else {
    notify(BLYNK_MSG, F("no auxiliary heater"));
    aux_heater_status = STATE_wait_blynk_off;
  }
}

void check_auxiliary_heating() {
  if ( aux_heating_port != 0x00 ) {
    if ( aux_heater_status == STATE_on) {
      if ( aux_heater_timer < millis() ) {
        set_auxiliary_heating(false);
      }
    }
  }
}
