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
int NotifyLast = 0;
bool NotifyActive = false;


void display_init(void) {

  #if defined ST7735
  st7735_init();
  #endif
  #if defined U8G2_DISPLAY
  u8g2_init();
  #endif

}




void display_loop() {
  if ( !display_update_timer_lock ) {
    display_update_timer_lock = true;
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

      /*if ( millis() <= MsgTimer ) {
        //notifyscreen();
        MainMenuPos = MENU_notify;
      }*/

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
          /**
          case MENU_optionen: menu_optionen(); break;

          case MENU_opt_config: menu_opt_config(); break;
          case MENU_opt_features: menu_opt_features(); break;
          case MENU_opt_ports: menu_opt_ports(); break;
          case MENU_save_config: menu_save_config(); break;

          case MENU_debug_ports: menu_debug_ports(); break;
          case MENU_debug_ports_2: menu_debug_ports_2(); break;**/


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

      /*switch (button_1) {
        case 1: MainMenuPos++; break;
        case 2: MainMenuPos = MENU_debug_ports; break;
      }*/
      if ( next() ) {
        MainMenuPos++;
      }
      button_1 = 0;

      #if defined U8G2_DISPLAY
      u8g2_update();
      #endif
      display_draw();

      SPI_lock = false;
    }



    display_update_timer_lock = false;

    //display_set_led();
  }
  else {
    message(TRACE_MSG, F("#display update locked...\n"));
  }
}

void display_update(void) {

  //#if defined U8G2_DISPLAY
  //u8g2_update();
  //#else
  display_loop();
  //#endif
  //  SPI_lock = false;
  //}
}

void display_notify() {
  //Serial.println("NOTIFY DISPLAY");

  if ( !(NotifyActive) && !(ignore_notify) ){
    Serial.println("NOTIFY DISPLAY");
    if ( MainMenuPos == 0 ) {
      MainMenuPos = 1;
    }
    NotifyLast = MainMenuPos;
    MainMenuPos = MENU_notify;
    NotifyActive = true;
    MsgTimer = millis() + 10000;
    //clear_screen();
    display_update();
  }
  else {
    MsgTimer = millis() + 10000;
  }

  display_update();
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


#endif
