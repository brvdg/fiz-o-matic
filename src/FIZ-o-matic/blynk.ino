
WidgetTerminal terminal(BLYNK_VIRTUAL_terminal);
WidgetMap myMap(BLYNK_VIRTUAL_map);
WidgetLED geo_fence_armed_led(BLYNK_VIRTUAL_geo_fence_armed_led);
WidgetLED geo_fence_led(BLYNK_VIRTUAL_geo_fence_led);
WidgetLED online_LED(BLYNK_VIRTUAL_online_LED);
WidgetLED armed_led(BLYNK_VIRTUAL_armed_led);
WidgetLED alarm_led(BLYNK_VIRTUAL_alarm_led);


/*
 *   Blynk Functions
 */
BLYNK_CONNECTED() {
  message(TRACE_MSG ,F("BLYNK_CONNECTED"));
  Blynk.syncAll();
}

BLYNK_APP_DISCONNECTED() {
  message(TRACE_MSG ,F("BLYNK_APP_DISCONNECTED"));
  blynk_msg(F("app disconected"));
}

BLYNK_APP_CONNECTED() {
  message(TRACE_MSG ,F("BLYNK_APP_CONNECTED"));
  blynk_msg(F("app conected"));
}


// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
BLYNK_WRITE(BLYNK_VIRTUAL_terminal)
{

  // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
  if (String("Marco") == param.asStr()) {
    terminal.println("You said: 'Marco'") ;
    terminal.println("I said: 'Polo'") ;
  } else {

    // Send it back
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }

  // Ensure everything is sent
  if ( parse_config_string(param.asStr()) ) {
    terminal.print("OK");
  }
  else {
    terminal.print(parse_ui_string(param.asStr(), SRC_BLYNK));
  }
  terminal.flush();
}



BLYNK_WRITE(BLYNK_VIRTUAL_stay_online) {
  message(DEBUG_MSG ,F("BLYNK_WRITE(BLYNK_VIRTUAL_stay_online)"));
  int value = param.asInt(); // Get value as integer


  if ( value == 1 ) {
    if ( !stay_online ) {
      stay_online = true;
      blynk_msg(F("stay_online turned on"));
      message(TINYGSM, F("stay_online turned off"));
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
  message(DEBUG_MSG ,F("BLYNK_WRITE(BLYNK_VIRTUAL_geo_fence_distance)"));
  int value = param.asInt(); // Get value as integer
  if ( value != geo_fence_distance ) {
    message(INFO_MSG ,F("set geo-fance distance to "));
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
  message(DEBUG_MSG ,F("BLYNK_WRITE(BLYNK_VIRTUAL_alarm)"));
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
  message(TRACE_MSG ,F("BLYNK_WRITE(BLYNK_VIRTUAL_aux_heater)"));
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
