/***************************************************
 *  This code is based on the U8G2 library from olikraus.
 *  GIT: https://github.com/olikraus/u8g2
 *       https://github.com/olikraus/U8g2_Arduino
 *  Wiki: https://github.com/olikraus/u8g2/wiki
 *
 *  Author: Brun von der Gönne
 *
 *  This module will only used by 32Bit processors.
 *
 ****************************************************/


#ifdef U8G2_DISPLAY

#include <U8g2lib.h>

void u8g2_init(void) {
    u8g2.begin();
    display_logo();
    display_draw();
}

void display_logo() {
  u8g2.drawXBM(LOGO_Xpos, LOGO_Ypos, LOGO_width, LOGO_height, logo);
  //u8g2_update();
}

void display_draw() {
    u8g2.sendBuffer();					// transfer internal memory to the display
}

void u8g2_update(void) {
  u8g2.sendBuffer();					// transfer internal memory to the display
}

/*bool display_pwrsave() {
  #if defined OLED || defined OLED_FULL || defined DSP_PWRSAVE
    if ( ( display_active_timer > millis() ) || ( engine_running ) ) {
      u8g2.setPowerSave(false);
      u8g2.sendBuffer();					// transfer internal memory to the display
      return false;
    }
    else {
      u8g2.setPowerSave(true);
      return true;
    }
  #endif
  return false;
}*/

void display_pwrsave(bool pwrsave) {
  u8g2.setPowerSave(pwrsave);
  u8g2.sendBuffer();					// transfer internal memory to the display
}

void clear_screen() {
  u8g2.clearDisplay();
}

void display_blink(bool blink) {

}
/*
 * Display MENUs
 */

void menu_notify() {

  /*switch ( info_type ) {
    case 1:
      //bootmsg1 = F("INFO");
      u8g2.drawXBM(Xpos + 5, Ypos, sign_info_width, sign_info_height, sign_info);
      break;
    case 2:
      //bootmsg1 = F("ERROR");
      u8g2.drawXBM(Xpos + 5, Ypos+10, sign_info_width, sign_info_height, sign_info);
      break;
  }*/


  u8g2.drawXBM(Xpos+5, Ypos+5, sign_info_width, sign_info_height, sign_info);

  u8g2.setFontPosTop();
  u8g2.setFont(medium_font);
  u8g2.setCursor(Xpos + 10, Ypos);
  //u8g2.print(bootmsg1);
  u8g2.setCursor(Xpos + 24, Ypos + 10);
  u8g2.setFont(medium_font);
  u8g2.print(display_message);
  //u8g2.print(info_text);
  //u8g2.setCursor(Xpos, Ypos + 16);
  //u8g2.print(bootmsg3);

}


void bootscreen() {

  switch ( info_type ) {
    case 1:
      bootmsg1 = F("INFO");
      u8g2.drawXBM(Xpos + 5, Ypos, sign_info_width, sign_info_height, sign_info);
      break;
    case 2:
      bootmsg1 = F("ERROR");
      u8g2.drawXBM(Xpos + 5, Ypos+10, sign_info_width, sign_info_height, sign_info);
      break;
  }

  if ( info_type == 0 ) {
    u8g2.drawXBM(LOGO_Xpos, LOGO_Ypos, LOGO_width, LOGO_height, logo);
  }
  else {
    u8g2.setFontPosTop();
    u8g2.setFont(medium_font);
    u8g2.setCursor(Xpos + 10, Ypos);
    //u8g2.print(bootmsg1);
    u8g2.setCursor(Xpos + 24, Ypos + 10);
    u8g2.setFont(medium_font);
    u8g2.print(info_text);
    //u8g2.setCursor(Xpos, Ypos + 16);
    //u8g2.print(bootmsg3);
  }

  switch (button_1) {
    case 1:
      MainMenuPos++;
      info_type = 0;
      break;
    //case 2: MainMenuPos = 200; break;
  }
  button_1 = 0;
}

/*
 * simple clock with date
 */
void menu_clock() {
  //clear_screen();
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT1_1_X, LAYOUT1_1_Y);
  sprintf(buf, "%02d:%02d", hour, minute);
  u8g2.print(buf);

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT1_2_X + 25, LAYOUT1_2_Y);
  int m = (temp_out - ( (int) temp_out ))*10;
  sprintf (buf, "%02d.%1d", (int)temp_out, m);
  u8g2.print(buf);
  //u8g2.print(temp_out, 1);
  u8g2.print(F("\xb0"));
  u8g2.print(F("C"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
//    case 2: MainMenuPos--; break;
    case 2: MainMenuPos = 200; break;
//    case 2: MainMenuPos = 101; break;
  }
  button_1 = 0;

}



/*
 * SPEED
 */
void menu_speed() {
  //clear_screen();
  if ( !engine_running) {
    MainMenuPos++;
  }
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT2_1_X, LAYOUT2_1_Y);
  //sprintf (buf, "%3d", speed + SPEED_OFFSET);
  sprintf (buf, "%3d", speed);
  u8g2.print(buf);

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT2_2_X, LAYOUT2_2_Y);
  u8g2.print(F("km/h"));

  //Temperature
  if ( temp_out_port != 0 ) {
    u8g2.setFont(medium_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT2_3_X, LAYOUT2_3_Y);
    int m = (temp_out - ( (int) temp_out ))*10;
    sprintf (buf, "%02d.%1d", (int)temp_out, m);
    u8g2.print(buf);
    //u8g2.print(temp_out, 1);
    u8g2.print(F("\xb0"));
    u8g2.print(F("C"));
  }

  //Time
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT2_4_X, LAYOUT2_4_Y);
  sprintf(buf, "%02d:%02d", hour, minute);
  u8g2.print(buf);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}


/*
 * GPS informations
 */
void menu_gpsinfo() {
  //clear_screen();
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT5_1_X, LAYOUT5_1_Y);
  u8g2.print(F("GPS Infos: "));
  u8g2.setCursor(LAYOUT5_2_X, LAYOUT5_2_Y);
  u8g2.print(F("Pos : "));
  u8g2.print(gps_latitude, 3);
  u8g2.print(F(", "));
  u8g2.print(gps_longitude, 3);
  u8g2.setCursor(LAYOUT5_3_X, LAYOUT5_3_Y);
  u8g2.print(F("Alt : "));
  u8g2.print(gps_altitude, DEC);
  u8g2.print(F("m"));
  u8g2.setCursor(LAYOUT5_4_X, LAYOUT5_4_Y);
  u8g2.print(F("Empfang: "));
  u8g2.print(gps_used_satellites, DEC);
  u8g2.print(F("/"));
  u8g2.print(gps_view_satellites, DEC);
  //u8g2.print(F(" )"));


  switch (button_1) {
    case 1: MainMenuPos++; break;
    //case 2: MainMenuPos = MENU_gpsinfo2; break;
  }
  button_1 = 0;

}

void menu_gpsinfo2() {
  //clear_screen();
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("GPS Infos (Trip): "));
  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(F("V max: "));
  u8g2.print(gps_speed_max, 5);
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("Dist.: "));
  u8g2.print(gps_distance/1000, DEC);
  u8g2.setCursor(Xpos, Ypos + 24);

  switch (button_1) {
    case 1: MainMenuPos++; break;
  }
  button_1 = 0;

}

/*
 * Trip Infos
 * distance, speed (avg, max), triptime
 */
void menu_trip() {
  //clear_screen();
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT4_1_X, LAYOUT4_1_Y);
  u8g2.print(F("Trip Infos: "));
  u8g2.setFont(medium_font);
  u8g2.setCursor(LAYOUT4_2_X, LAYOUT4_2_Y);
  //u8g2.print(F("Dist: "));
  uint16_t m = gps_distance - ( gps_distance / 1000 ) * 1000;
  sprintf (buf, "%04d.%1d km", gps_distance / 1000, m / 100);

  u8g2.print(buf);
  u8g2.setFont(small_font);
  u8g2.setCursor(LAYOUT4_3_X, LAYOUT4_3_Y);

  long tmp = engine_running_total + engine_running_sec;
  //u8g2.print(F("travel time: "));
  u8g2.print(F("in "));
  u8g2.print(tmp/3600, DEC);
  u8g2.print(F("h "));
//  if ( engine_running_sec > 3600 ) {
  u8g2.print((tmp - ((tmp/3600) * 3600)) / 60 , DEC);
  u8g2.print(F("min"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2:
      //gps_distance_trip = 0;
      gps_speed_max_trip = 0;
      engine_running_total = 0;
      trip_distance = 0;
      break;
  }
  button_1 = 0;
}

/*
 * SD Card
 */
/*void menu_sdcard() {
  #ifdef SDCARD

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("SD Card Infos: "));

  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(F("Card: "));
  if ( SDmount && !SDerror) {
    u8g2.print(F("ready"));
  }
  else {
    u8g2.print(F("not ready"));
  }

  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("File: "));
  u8g2.print(filename);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2:
      button_1 = 0;
      if (SDmount) {
        close_file();
        SDmount = false;
      } else {
        MainMenuPos = 0;
        Serial.println(F("#remount_sdcard"));
        SDmount = false;
        delay(1000);
        sdcard_init();
        MainMenuPos = 7;
      }
      break;
  }
  button_1 = 0;
  #else //SDCARD
  MainMenuPos++;
  #endif //SDCARD
}


/*
 * Clima
 */
void menu_clima() {
  int m = 0;
  //clear_screen();
  if ( temp_out_port == 0 ) {
    MainMenuPos++;

  } else {
    // use a different layout if humidity exists
    if ( hum_out > 0 ) {
      u8g2.setFont(small_font);
      u8g2.setFontPosTop();
      u8g2.setCursor(LAYOUT4_1_X, LAYOUT4_1_Y);
      u8g2.print(F("Klima Aussen:"));

      u8g2.setFont(medium_font);
      u8g2.setCursor(LAYOUT4_2_X + 22, LAYOUT4_2_Y);

      m = (temp_out - ( (int) temp_out ))*10;
      sprintf (buf, "%02d.%1d", (int)temp_out, m);
      u8g2.print(buf);

      //u8g2.print(temp_out, 1);
      u8g2.print(F("\xb0"));
      u8g2.print(F("C"));

      u8g2.setFont(small_font);
      u8g2.setCursor(LAYOUT4_3_X + 28, LAYOUT4_3_Y);

      sprintf (buf, "%02d", hum_out);
      u8g2.print(buf);
      u8g2.print(F("%"));
    }
    else {
      u8g2.setFont(small_font);
      u8g2.setFontPosTop();
      u8g2.setCursor(LAYOUT3_1_X, LAYOUT3_1_Y);
      u8g2.print(F("Klima Aussen:"));

      u8g2.setFont(big_font);
      u8g2.setCursor(LAYOUT3_2_X, LAYOUT3_2_Y);

      m = (temp_out - ( (int) temp_out ))*10;
      sprintf (buf, "%02d.%1d", (int)temp_out, m);
      u8g2.print(buf);

      u8g2.setFont(small_font);
      u8g2.setCursor(LAYOUT3_3_X, LAYOUT3_3_Y);
      u8g2.print(F("\xb0"));
      u8g2.print(F("C"));
    }
    switch (button_1) {
      case 1: MainMenuPos++; break;
      //case 2: MainMenuPos = SubMenu3Nr; break;
    }
    button_1 = 0;
  }
}



/*
 * Dynamic Menu for all define values
 */
void menu_values() {
  //clear_screen();
  if (( *values[MenuValuesPos].port == 0 ) || ( !values[MenuValuesPos].show )) {
    if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
      MainMenuPos++;
      MenuValuesPos = 0;
    } else {
      MenuValuesPos++;
    }
  }
  else if ( !values[MenuValuesPos].show_off && !engine_running) {
    if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
      MainMenuPos++;
      MenuValuesPos = 0;
    } else {
      MenuValuesPos++;
    }
  }
  else {
    u8g2.setFont(small_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT3_1_X, LAYOUT3_1_Y);
    u8g2.print(values[MenuValuesPos].desc);
    u8g2.setFont(big_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT3_2_X, LAYOUT3_2_Y);

    int digits = 1;
    if ( *values[MenuValuesPos].value > 999 ) {
      digits = 4;
    }
    else if ( *values[MenuValuesPos].value > 99 ) {
      digits = 3;
    }
    else if ( *values[MenuValuesPos].value > 9 ) {
      digits = 2;
    }

    if ( values[MenuValuesPos].digits != 0 ) {
      digits += 1 + values[MenuValuesPos].digits;
    }

    digits = 4 - digits;
    for (digits; digits>0; digits--) {
      u8g2.print(F(" "));
    }

    u8g2.print(*values[MenuValuesPos].value, values[MenuValuesPos].digits);

    u8g2.setFont(medium_font);
    u8g2.setCursor(LAYOUT3_3_X, LAYOUT3_3_Y);
    u8g2.print(values[MenuValuesPos].suffix);

    if ( next() ) {
      clear_screen();
      if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
        MainMenuPos++;
        MenuValuesPos = 0;
      } else {
        MenuValuesPos++;
        //clear_screen();
      }
    }

    /*switch (button_1) {
      case 1:
        if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
          MainMenuPos++;
          MenuValuesPos = 0;
        } else {
          MenuValuesPos++;
        }
        break;
      case 2: MainMenuPos--; break;
    }
    button_1 = 0;*/
  }
}

/*
 * Info Menu
 */
void menu_info() {
  //clear_screen();
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT5_1_X, LAYOUT5_1_Y);
  u8g2.print(F("fiz-o-matic"));
  u8g2.setCursor(LAYOUT5_2_X, LAYOUT5_2_Y);
  u8g2.print(F("Designed by Brun"));
  u8g2.setCursor(LAYOUT5_3_X, LAYOUT5_3_Y);
  u8g2.print(F("Version: "));
  u8g2.print(F(VERSION));
  u8g2.setCursor(LAYOUT5_4_X, LAYOUT5_4_Y);
  u8g2.print(F("Build: "));
  u8g2.print(F(BUILD));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    //case 2: MainMenuPos--; break;
    case 2: MainMenuPos = 200; break;

  }
  button_1 = 0;
}

/*
 * Options Menu
 */
void menu_optionen() {

  if ( engine_running) {
    MainMenuPos++;
  }
  if ( no_long_press ) {
    MainMenuPos++;
  }

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos+10, Ypos+10);

  switch (MenuConfigPos) {
    case 0:
      //u8g2.setFont(medium_font);
      //u8g2.setFontPosTop();
      //u8g2.setCursor(Xpos, Ypos + 5);
      u8g2.print(F("Optionen"));
      break;
    case 1:
      //u8g2.setFont(medium_font);
      //u8g2.setFontPosTop();
      //u8g2.setCursor(Xpos + 10, Ypos + 10);
      u8g2.print(F("Config"));
      break;
    case 2:
      //u8g2.setFont(medium_font);
      //u8g2.setFontPosTop();
      //u8g2.setCursor(Xpos + 10, Ypos + 10);
      u8g2.print(F("Features"));
      break;
    case 3:
      //u8g2.setFont(medium_font);
      //u8g2.setFontPosTop();
      //u8g2.setCursor(Xpos + 10, Ypos + 10);
      u8g2.print(F("Ports"));
      break;
    default:
      //u8g2.setFont(medium_font);
      //u8g2.setFontPosTop();
      //u8g2.setCursor(Xpos + 10, Ypos + 10);
      u8g2.print(F("go back"));
      //MenuConfigPos = 1;
      break;
  }

  switch (button_1) {
    case 1:
      if ( MenuConfigPos == 0 ) {
        MainMenuPos++;
      }
      else if ( MenuConfigPos >= 3 ) {
        MenuConfigPos = 0;
        MainMenuPos = 104;
      }
      else {
        MenuConfigPos++;
      }
      break;
    //case 2: MainMenuPos = 101; break;
    case 2:
      switch (MenuConfigPos) {
        case 0: MenuConfigPos = 1; break;
        case 1: MainMenuPos = 101; MenuConfigPos = 0; break;
        case 2: MainMenuPos = 102; MenuConfigPos = 0; break;
        case 3: MainMenuPos = 103; MenuConfigPos = 0; break;
        default: MenuConfigPos = 0; MainMenuPos++; break;
      }
  }
  button_1 = 0;
}


/*
 * Dynamic Menu for config parameters
 */
void menu_opt_config() {
  //clear_screen();
  if ( config[MenuConfigPos].steps == 0 ) {
    if (MenuConfigPos+1 >= (sizeof(config) / sizeof(config[0])) ) {
      MainMenuPos++;
      MenuConfigPos = 0;
    } else {
      MenuConfigPos++;
    }
  }
  else {
    u8g2.setFont(small_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT4_1_X, LAYOUT4_1_Y);
    u8g2.print(config[MenuConfigPos].desc);
    u8g2.setFont(medium_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT4_2_X, LAYOUT4_2_Y);

    u8g2.print(*config[MenuConfigPos].config);

    u8g2.setFont(small_font);
    u8g2.setCursor(LAYOUT4_3_X, LAYOUT4_3_Y);

    if ( MenuEdit ) {
      u8g2.print(F("long -> accept"));
    }
    else {
      u8g2.print(F("long -> edit"));
    }


    switch (button_1) {
      case 1:
        if ( MenuEdit ) {
          *config[MenuConfigPos].config = *config[MenuConfigPos].config + config[MenuConfigPos].steps;
          if ( *config[MenuConfigPos].config > config[MenuConfigPos].max ) {
            *config[MenuConfigPos].config = 0;
          }
        }
        else {
          if (MenuConfigPos+1 >= (sizeof(config) / sizeof(config[0])) ) {
            MainMenuPos = MENU_optionen;
            MenuConfigPos = 1;
          } else {
            MenuConfigPos++;
          }
        }
        break;
      case 2:
        if ( MenuEdit ) {
          MenuEdit = false;
        }
        else {
          MenuEdit = true;
        }
        break;
    }
    button_1 = 0;
  }
}

void menu_opt_features() {
  //clear_screen();
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT4_1_X, LAYOUT4_1_Y);
  u8g2.print(features[MenuConfigPos].desc);
  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT4_2_X, LAYOUT4_2_Y);

  if (*features[MenuConfigPos].feature) {
    u8g2.print(F("on"));
  }
  else {
    u8g2.print(F("off"));
  }
  u8g2.setFont(small_font);
  u8g2.setCursor(LAYOUT4_3_X, LAYOUT4_3_Y);

  if ( MenuEdit ) {
    u8g2.print(F("long -> accept"));
  }
  else {
    u8g2.print(F("long -> edit"));
  }


  switch (button_1) {
    case 1:
      if ( MenuEdit ) {
        //*config[MenuConfigPos].config = *config[MenuConfigPos].config + config[MenuConfigPos].steps;
        if ( *features[MenuConfigPos].feature ) {
          *features[MenuConfigPos].feature = false;
        }
        else {
          *features[MenuConfigPos].feature = true;
        }
      }
      else {
        if (MenuConfigPos+1 >= (sizeof(features) / sizeof(features[0])) ) {
          MainMenuPos = MENU_optionen;
          MenuConfigPos = 1;
        } else {
          MenuConfigPos++;
        }
      }
      break;
    case 2:
      if ( MenuEdit ) {
        MenuEdit = false;
      }
      else {
        MenuEdit = true;
      }
      break;
  }
  button_1 = 0;

}

void menu_opt_ports() {
  //clear_screen();
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT4_1_X, LAYOUT4_1_Y);
  u8g2.print(ports[MenuConfigPos].desc);
  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT4_2_X, LAYOUT4_2_Y);

  u8g2.print(F("0x"));
  if (*ports[MenuConfigPos].port<16) {
    u8g2.print(F("0"));
  }
  u8g2.print(*ports[MenuConfigPos].port, HEX);

  u8g2.setFont(small_font);
  u8g2.setCursor(LAYOUT4_3_X, LAYOUT4_3_Y);

  if ( MenuEdit ) {
    u8g2.print(F("long -> accept"));
  }
  else {
    u8g2.print(F("long -> edit"));
  }


  switch (button_1) {
    case 1:
      if ( MenuEdit ) {
        if ( *ports[MenuConfigPos].port == 0x00 ) {
          *ports[MenuConfigPos].port = port_values[0].port;
        }
        else {
          for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
            if (*ports[MenuConfigPos].port == port_values[i].port) {
              i++;
              *ports[MenuConfigPos].port = port_values[i].port;
              break;
            }
          }
        }
      }
      else {
        if (MenuConfigPos+1 >= (sizeof(ports) / sizeof(ports[0])) ) {
          MainMenuPos++;
          MenuConfigPos = 0;
        } else {
          MenuConfigPos++;
        }
      }
      break;
    case 2:
      if ( MenuEdit ) {
        MenuEdit = false;
      }
      else {
        MenuEdit = true;
      }
      break;
  }
  button_1 = 0;
}


void menu_save_config() {
  //clear_screen();
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Save config:"));

  u8g2.setCursor(Xpos + 10, Ypos + 10);
  //u8g2.print(F("press and hold to save"));

  switch (saved_config) {
    case 0: u8g2.print(F("hold to save")); break;
    case 1: u8g2.print(F("OK config saved")); break;
    case 2: u8g2.print(F("ERR: stop Engine")); break;

  }


  switch (button_1) {
    case 1:
      MainMenuPos++;
      saved_config = 0;
      break;
    case 2:
      if (!engine_running) {
        if (saved_config != 1 ) {
          saved_config = 1;
          SPI_lock = false;
          save_config();
          SPI_lock = true;
        }
      }
      else {
        saved_config = 2;
      }

      break;
  }
  button_1 = 0;

  //MainMenuPos++;
}

void menu_debug_ports() {
  //clear_screen();
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Port1: "));
  u8g2.print(a0_V, 1);

  u8g2.setCursor(Xpos, Ypos+small_font_high);
  u8g2.print(F("Port2: "));
  u8g2.print(a1_V, 1);

  u8g2.setCursor(Xpos, Ypos+2*small_font_high);
  u8g2.print(F("Port3: "));
  u8g2.print(a2_V, 1);

  u8g2.setCursor(Xpos, Ypos+3*small_font_high);
  u8g2.print(F("Port4: "));
  u8g2.print(a3_V, 1);


  switch (button_1) {
    case 1: MainMenuPos++; break;
    //case 2: speed_source++; break;
  }
  button_1 = 0;
}

void menu_debug_ports_2() {
  //clear_screen();
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Port4: "));
  u8g2.print(a4_hz);

  u8g2.setCursor(Xpos, Ypos+small_font_high);
  u8g2.print(F("Port5: "));
  u8g2.print(a5_hz);


  u8g2.setCursor(Xpos, Ypos+2*small_font_high);
  u8g2.print(F("Fuel Ohm: "));
  u8g2.print(fuel_ohm);

  u8g2.setCursor(Xpos, Ypos+3*small_font_high);
  u8g2.print(F("Temp Ohm: "));
  u8g2.print(water_temp_ohm);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    //case 2: speed_source++; break;
  }
  button_1 = 0;
}



#endif // U8G2_DISPLAY
