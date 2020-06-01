/****************************************************
 * FIZ-o-matic
 * https://fiz-o-matic.net/
 *
 *
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/


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
        
      }
      else {
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
      }
      // long press repeat
      else if ((button_1_low >= 15) && (button_1_high == 0)) {
        button_1 = 4;
        button_1_high = 0;
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


        if ( ( !running ) && display_active_timer < millis() ) {
          //button_1 = 0;
          //display_active_timer = millis() + DISPLAY_ACTIVE_TIME;
          display_active_timer = millis() + DISPLAY_ACTIVE_TIME;

        }
        else {
          button_1 = 1;
          //display_active_timer = millis() + DISPLAY_ACTIVE_TIME;
          display_active_timer = millis() + DISPLAY_ACTIVE_TIME;

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
