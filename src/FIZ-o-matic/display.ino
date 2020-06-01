/***************************************************
 *  This sketch handle the common display functions
 *
 *  Author: Brun
 *
 ****************************************************/

#if defined ENABLE_DISPLAY


//Menu defination
#define MENU_clock 1
#define MENU_speed 2
#define MENU_trip 3
#define MENU_values 4
#define MENU_clima 5
#define MENU_gpsinfo 6
#define MENU_optionen 7
#define MENU_info 8

#define MENU_opt_config 101
#define MENU_opt_features 102
#define MENU_opt_ports 103
#define MENU_save_config 104

#define MENU_debug_ports 200
#define MENU_debug_ports_2 201
#define MENU_sdcard 202

#define MENU_notify 999
#define MENU_notify_ack 1000

int standby_menu[] = {0, MENU_clock, MENU_trip, MENU_clima, MENU_gpsinfo, MENU_info};
int running_menu[] = {0, MENU_speed, MENU_trip, MENU_values, MENU_clima};


#ifdef DOGS102
#include <display_u8g2_dogs102.h>
#endif

#ifdef OLED
#include <display_u8g2_OLED.h>
#endif

#ifdef OLEDFULL
#include <display_u8g2_OLEDFULL.h>
#endif

// local variables
int OldPos = 0;



void display_init(void) {

  #if defined ST7735
  st7735_init();
  #endif
  #if defined U8G2_DISPLAY
  u8g2_init();
  #endif

}


void display_loop() {
  int menu_pos = 0;
  second = rtc.getSeconds();
  minute = rtc.getMinutes();
  hour = rtc.getHours();
  day = rtc.getDay();
  month = rtc.getMonth();
  year = rtc.getYear() + 2000;
  yield();

  if ( display_update_timer_lock ) return;

  display_update_timer_lock = true;

  if ( online || go_online ) {
    //Serial.println("clear_screen");
    clear_screen();
    //Serial.println("display_logo");
    display_logo();
    display_update_timer_lock = false;
    yield();
    return;
  }


  if ( MainMenuPos >= 100 ) {
    menu_pos = MainMenuPos;
    //Serial.println(menu_pos, DEC);
  }
  else {
    if (engine_running) {
      if ( MainMenuPos > sizeof(running_menu)-1 ) MainMenuPos = 1;
      menu_pos = running_menu[MainMenuPos];

    }
    else {
      if ( MainMenuPos > sizeof(standby_menu)-1 ) MainMenuPos = 1;
      menu_pos = standby_menu[MainMenuPos];

    }
  }

  //Serial.println("MENÜ");
  //Serial.println(menu_pos, DEC);
  //Serial.println(MainMenuPos, DEC);

  if ( OldPos != menu_pos ) {
    clear_screen();
    OldPos = menu_pos;
  }

  switch (menu_pos) {
    case 0: bootscreen(); break;
    case MENU_clock: menu_clock(); if ( enter() ) MainMenuPos++; break;
    case MENU_speed: menu_speed(); break;
    case MENU_trip: menu_trip(); break;
    case MENU_values: menu_values(); break;
    case MENU_clima: menu_clima(); break;
    case MENU_gpsinfo: menu_gpsinfo(); break;

    case MENU_optionen: menu_optionen(); break;
    case MENU_info: menu_info(); break;

    case MENU_opt_config: menu_opt_config(); break;
    case MENU_opt_features: menu_opt_features(); break;
    case MENU_opt_ports: menu_opt_ports(); break;
    case MENU_save_config: menu_save_config(); break;

    case MENU_debug_ports: menu_debug_ports(); break;
    case MENU_debug_ports_2: menu_debug_ports_2(); break;


    case MENU_notify: {
      menu_notify();
      if ( enter() ) {
        ignore_notify = true;
        MainMenuPos++;
      }
      break;
    }
    case MENU_notify_ack: {
      MainMenuPos = NotifyLast;
      display_blink(false);
      NotifyActive = false;
      info_val=0;
      info_text="";
      break;
    }
    default: {
      menu_clock();
      MainMenuPos = 1;
      OldPos = MainMenuPos;
      break;
    }
  }

  // update the Display!
  display_draw();

  dimmer();

  yield();


  display_update_timer_lock = false;
}

void display_loop2() {

  //if ( !timer_check(&display_update_timer, U8G2_DISPLAY_UPDATE_TIMER) ) return;


  #if defined DSP_PWRSAVE
    if ( display_pwrsave() ) {
      //delay(U8G2_DISPLAY_UPDATE_TIMER);
      return;
    }
    yield();
  #endif



  if ( !display_update_timer_lock ) {
    display_update_timer_lock = true;

    if ( online || go_online ) {
      display_logo();
      display_update_timer_lock = false;
      return;
    }

    second = rtc.getSeconds();
    minute = rtc.getMinutes();
    hour = rtc.getHours();
    day = rtc.getDay();
    month = rtc.getMonth();
    year = rtc.getYear() + 2000;


    if ( !SPI_lock ) {
      SPI_lock = true;
      if ( OldPos != MainMenuPos ) {
        clear_screen();
        OldPos = MainMenuPos;
      }


      else {
        switch (MainMenuPos) {
          case 0: bootscreen(); break;
          case MENU_clock: menu_clock(); if ( enter() ) MainMenuPos++; break;
          case MENU_speed: menu_speed(); break;
          case MENU_trip: menu_trip(); break;
          case MENU_values: menu_values(); break;
          case MENU_clima: menu_clima(); break;
          case MENU_gpsinfo: menu_gpsinfo(); break;

          case MENU_optionen: menu_optionen(); break;
          case MENU_info: menu_info(); break;

          case MENU_opt_config: menu_opt_config(); break;
          case MENU_opt_features: menu_opt_features(); break;
          case MENU_opt_ports: menu_opt_ports(); break;
          case MENU_save_config: menu_save_config(); break;

          case MENU_debug_ports: menu_debug_ports(); break;
          case MENU_debug_ports_2: menu_debug_ports_2(); break;


          case MENU_notify: {
            menu_notify();
            if ( enter() ) {
              ignore_notify = true;
              MainMenuPos++;
            }
            break;
          }
          case MENU_notify_ack: {
            MainMenuPos = NotifyLast;
            display_blink(false);
            NotifyActive = false;
            info_val=0;
            info_text="";
            break;
          }
          default: {
            menu_clock();
            MainMenuPos = 1;
            OldPos = MainMenuPos;
            break;
          }
        }
      }


      if ( next() ) {
        MainMenuPos++;
      }
      button_1 = 0;


      display_draw();

      SPI_lock = false;
    }
    else {
      message(DEBUG_MSG, F("#spi bus locked...\n"));
    }



    display_update_timer_lock = false;

    //display_set_led();
  }
  else {
    message(DEBUG_MSG, F("#display update locked...\n"));
  }
  //delay(U8G2_DISPLAY_UPDATE_TIMER);
  yield();
}

/*void display_update(void) {

  //#if defined U8G2_DISPLAY
  //u8g2_update();
  //#else
  //display_loop();
  yield();
  //#endif
  //  SPI_lock = false;
  //}
}*/


void dimmer() {
#ifdef DISPLAY_BG_LED
  static int dimmer_pct_old = 0;
  static int dimmer_val = 0;
  int dimmer_val_soll = 0;
  dimmer_V = get_port_value(dimmer_port);



  int dimmer_pct = ((100000 / (DIMMER_MAX_mV - DIMMER_MIN_mV)) * (dimmer_V * 1000 - DIMMER_MIN_mV)) / 1000;

  if ( dimmer_pct < 5 ) {
    if (running) {
      dimmer_pct = 5;
    } else {
      if ( display_active_timer > millis() ) {
        dimmer_pct = (display_active * 100 ) / dimmer_max;
      }
      else {
        dimmer_pct = 0;
      }
    }
  }

  //Serial.println(dimmer_pct, DEC);

  /**
  if ( dimmer_pct_old << dimmer_pct ) {
    dimmer_pct = dimmer_pct_old+1;
  }
  else if ( dimmer_pct_old >> dimmer_pct ) {
    dimmer_pct = dimmer_pct_old-1;
  }

  dimmer_pct_old = dimmer_pct;
  **/
  //Serial.println(dimmer_pct_old, DEC);


  //dimmer_val = ( dimmer_pct * DIMMER_MAX ) / 100;

  dimmer_val_soll = ((dimmer_max - dimmer_min) * dimmer_pct) / 100;

  /*Serial.print(dimmer_pct, DEC);
  Serial.print(", ");

  Serial.print(dimmer_val, DEC);
  Serial.print(", ");
  Serial.print(dimmer_val_soll, DEC);
  Serial.print(", ");*/

  if ( dimmer_val_soll > dimmer_val ) {
    dimmer_val++;
  }
  else if ( dimmer_val_soll < dimmer_val ) {
    dimmer_val--;
  }

  /*Serial.print(dimmer_val, DEC);
  Serial.print(", ");
  Serial.println(dimmer_val_soll, DEC);*/
  analogWrite(DISPLAY_BG_LED, dimmer_val);

  /**dimmer_val = ((dimmer_max - dimmer_min) * dimmer_pct) / 100;
  if (alarm_counter == 0) {
    analogWrite(DISPLAY_BG_LED, dimmer_val);
  }**/

#endif
}




void display_notify() {
  //Serial.println("NOTIFY DISPLAY");

  if ( !(NotifyActive) && !(ignore_notify) ){
    //Serial.println("NOTIFY DISPLAY");
    if ( MainMenuPos == 0 ) {
      MainMenuPos = 1;
    }
    NotifyLast = MainMenuPos;
    MainMenuPos = MENU_notify;
    NotifyActive = true;
    MsgTimer = millis() + 10000;
    //clear_screen();
    //display_update();
  }
  else {
    MsgTimer = millis() + 10000;
  }

  //display_update();
}


/**void display_bootmsg(String msg) {


  //TRACE_PRINTLN(F("#->display_bootmsg"));
  if ( info_type == 0 ) {
    bootmsg1 = bootmsg2;
    bootmsg2 = bootmsg3;
    bootmsg3 = msg;
  }

  //bootmsg = "Waiting for Serial";
  display_update();

  message(INFO_MSG, F("-> "));
  message(INFO_MSG, msg);
  message(INFO_MSG, F("\n"));

  //delay(2000);
}**/


/**#else // DISPLAY

void display_bootmsg(String msg) {
  //TRACE_PRINTLN(F("#->display_bootmsg"));

  message(INFO_MSG, F("#"));
  message(INFO_MSG, msg);
  message(INFO_MSG, F("\n"));
}**/


#else

void display_init() {
  yield();
}

void display_notify() {
  yield();
}

void display_loop() {
  yield();
}


#endif
