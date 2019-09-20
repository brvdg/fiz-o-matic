#ifdef U8G2_DISPLAY2

#include <U8g2lib.h>

//Menu defination
#define MENU_clock 1
#define MENU_speed 2
#define MENU_trip 3
#define MENU_values 4
//#define MENU_water_temp 40
//#define MENU_fuel 50
#define MENU_clima 5
//#define MENU_power 7
#define MENU_gpsinfo 6
//#define MENU_rpm 9
#define MENU_optionen 7
#define MENU_info 8

//#define MENU_altitude

#define MENU_custom 13
#define MENU_gpsinfo2 14
//#define MENU_

#define MENU_opt_config 101
#define MENU_opt_features 102
#define MENU_opt_ports 103
#define MENU_save_config 104

//#define MENU_opt_temp_out 1010
//#define MENU_opt_rpm 102
//#define MENU_opt_bord_voltage 103
//#define MENU_opt_ki_dimmer 104
//#define MENU_opt_speed 105
//#define MENU_opt_dimmer_min 106
//#define MENU_opt_dimmer_max 107


#define MENU_debug_ports 200
#define MENU_debug_ports_2 201
#define MENU_sdcard 202




#ifdef DOGS102
#include <display_u8g2_dogs102.h>
#endif

#ifdef OLEDFULL
#include <display_u8g2_OLEDFULL.h>
#endif

#ifdef OLED
#include <display_u8g2_OLED.h>
#endif

#ifdef NOKIA
//U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* dc=*/ 10, /* reset=*/ 11);     // Nokia 5110 Display
#endif




/**
#define sign_info_width 16
#define sign_info_height 16
static unsigned char sign_info[] = {
   0xfc, 0x3f, 0x02, 0x40, 0x01, 0x80, 0x81, 0x81, 0x01, 0x80, 0x01, 0x80,
   0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
   0xc1, 0x83, 0x01, 0x80, 0x02, 0x40, 0xfc, 0x3f };
**/









void display_init(void) {
    u8g2.begin();
}

void display_loop() {
  //TRACE_PRINTLN(F("#->display_loop"));
  //if ( !SPI_lock ) {
    //SPI_lock = true;
    if ( !display_update_timer_lock ) {
      minute = rtc.getMinutes();
      hour = rtc.getHours();
      day = rtc.getDay();
      month = rtc.getMonth();
      year = rtc.getYear() + 2000;

#if defined OLED || defined OLED_FULL || defined DSP_PWRSAVE
      if ( ( display_active_timer > millis() ) || ( engine_running ) ) {
        display_update_timer_lock = true;
        u8g2.setPowerSave(false);
        display_update();
        display_update_timer_lock = false;
      }
      else {
        u8g2.setPowerSave(true);
      }
#else
      display_update_timer_lock = true;
      display_update();
      display_update_timer_lock = false;
#endif

      //display_set_led();
    }
    else {
      message(TRACE_MSG, F("#display update locked...\n"));
    }
    //SPI_lock = false;
  //}
  //else {
    //DEBUG_PRINTLN(F("#SPI Bus locked for Display update"));
  //}
}

void display_draw(void) {
  //TRACE_PRINTLN(F("#->draw"));
   //u8g2.setFont(small_font);

  u8g2.setFontPosTop();
  //u8g2.setCursor(Xpos, Ypos);

  //DEBUG_PRINTLN(MainMenuPos);

  if ( millis() <= MsgTimer ) {
    //message("message");
    //message(String(millis(),DEC));
    //message("<=");
    //message(String(MsgTimer,DEC));
    //message("\n");
    notifyscreen();

  }
  else {
    switch (MainMenuPos) {
      case 0: bootscreen(); break;
      case MENU_clock: menu_clock(); break;
      case MENU_speed: menu_speed(); break;
      case MENU_trip: menu_trip(); break;
      case MENU_values: menu_values(); break;
      case MENU_gpsinfo: menu_gpsinfo(); break;
      //case MENU_gpsinfo2: menu_gpsinfo2(); break;
      //case MENU_power: menu_power(); break;
      //case MENU_sdcard: menu_sdcard(); break;
      //case MENU_water_temp: menu_water_temp(); break;
      case MENU_clima: menu_clima(); break;
      case MENU_info: menu_info(); break;
      //case MENU_fuel: menu_fuel(); break;
      //case MENU_rpm: menu_rpm(); break;

      case MENU_optionen: menu_optionen(); break;

      case MENU_opt_config: menu_opt_config(); break;
      case MENU_opt_features: menu_opt_features(); break;
      case MENU_opt_ports: menu_opt_ports(); break;
      //case MENU_opt_temp_out: menu_opt_temp_out(); break;
      //case MENU_opt_rpm: menu_opt_rpm(); break;
      //case MENU_opt_bord_voltage: menu_opt_bord_voltage(); break;
      //case MENU_opt_ki_dimmer: menu_opt_ki_dimmer(); break;
      //case MENU_opt_speed: menu_opt_speed(); break;
      //case MENU_opt_dimmer_min: menu_opt_dimmer_min(); break;
      //case MENU_opt_dimmer_max: menu_opt_dimmer_max(); break;
      case MENU_save_config: menu_save_config(); break;

      case MENU_debug_ports: menu_debug_ports(); break;
      case MENU_debug_ports_2: menu_debug_ports_2(); break;



      #ifdef CUSTOM
      case MENU_custom: custom_menu(); break; // display the custom menu
      #endif // CUSTOM

      default: {
        menu_clock();
        MainMenuPos = 1;
        break;
      }
    }
  }
}

void display_update(void) {
  if ( !SPI_lock ) {
    SPI_lock = true;
    //TRACE_PRINTLN(F("#->display_update"));
    u8g2.firstPage();
    do {
      display_draw();
    } while ( u8g2.nextPage() );
    SPI_lock = false;
  }
}


void display_bootmsg(String msg) {


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
}




#endif // U8G2_DISPLAY
