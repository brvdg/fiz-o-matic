#if defined ST7735

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>


// https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>


//Adafruit_ST7735 tft = Adafruit_ST7735(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);
Adafruit_ST7735 tft = Adafruit_ST7735(DISPLAY_CS, DISPLAY_DC, -1);

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0xD6BA


#define small_font &FreeSansBold9pt7b
#define small_font_high 14
#define small_font_width 6
#define medium_font &FreeSansBold12pt7b
#define medium_font_high 18
#define medium_font_width 9
#define big_font &FreeSansBold24pt7b
#define big_font_high 54
#define big_font_width 41

#define FONT9 &FreeSansBold9pt7b
#define FONT12 &FreeSansBold12pt7b
#define FONT18 &FreeSansBold18pt7b
#define FONT24 &FreeSansBold24pt7b


#define BGCOLOR BLACK
#define FONTCOLOR WHITE

// definiation for the poritioning
#define TIME_X 36
#define TIME_Y 2
#define TIME_LINE TIME_Y + 20

#define TEMP_X 36
#define TEMP_Y 140
#define TEMP_LINE 130

#define TRIP_X 32
#define TRIP_Y 116
#define TRIP_LINE TRIP_Y - 4

#define VALUE_X 24
#define VALUE_Y 56
#define VAL_NAME_X 8
#define VAL_NAME_Y 30








// this defines the starting possition.
#define Xpos 0
#define Ypos 0
#define Xsize 128
#define Ysize 160

#define BIG_VAL_POS_X Xsize/2 + Xpos - 36
#define BIG_VAL_POS_Y Ysize/2 + Ypos - big_font_high/2
#define UNIT_POS_X BIG_VAL_POS_X + 36 + 30
#define UNIT_POS_Y Ysize/2 + Ypos - big_font_high/2


// Layout Nr. 1
// Digital Clock
#define LAYOUT1_1_X 12
#define LAYOUT1_1_Y 15
#define LAYOUT1_2_X 10
#define LAYOUT1_2_Y 44

// Layout Nr. 2
// e.g. Speed
//BIG_VAL_POS_X, BIG_VAL_POS_Y-12
#define LAYOUT2_1_X Xsize/2 + Xpos - 36
#define LAYOUT2_1_Y Ysize/2 + Ypos - big_font_high/2 - 12
#define LAYOUT2_2_X LAYOUT2_1_X + 36 + 30
#define LAYOUT2_2_Y LAYOUT2_1_Y
#define LAYOUT2_3_X 4
#define LAYOUT2_3_Y LAYOUT2_2_Y + big_font_high
#define LAYOUT2_4_X 68
#define LAYOUT2_4_Y LAYOUT2_3_Y

// Layout Nr. 3
// e.g. Values
#define LAYOUT3_1_X 0
#define LAYOUT3_1_Y 0
#define LAYOUT3_2_X 12
#define LAYOUT3_2_Y 22
#define LAYOUT3_3_X 96
#define LAYOUT3_3_Y LAYOUT3_2_Y

// Layout Nr. 4
// e.g. Trip, Clima and Options Menue
#define LAYOUT4_1_X Xpos
#define LAYOUT4_1_Y Ypos
#define LAYOUT4_2_X Xpos + 12
#define LAYOUT4_2_Y LAYOUT4_1_Y + small_font_high + 2
#define LAYOUT4_3_X Xpos + 12
#define LAYOUT4_3_Y LAYOUT4_2_Y + medium_font_high + 2

// Layout Nr. 5
// e.g. GPS and Info Page
#define LAYOUT5_1_X Xpos
#define LAYOUT5_1_Y Ypos
#define LAYOUT5_2_X Xpos + 2
#define LAYOUT5_2_Y LAYOUT5_1_Y + small_font_high
#define LAYOUT5_3_X Xpos + 2
#define LAYOUT5_3_Y LAYOUT5_2_Y + small_font_high
#define LAYOUT5_4_X Xpos + 2
#define LAYOUT5_4_Y LAYOUT5_3_Y + small_font_high


const unsigned char logo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1c, 0x38, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
  0x0a, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf0, 0x03, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x73, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x8f, 0x3b, 0x3c,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb8,
  0xcf, 0x39, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x38, 0xc7, 0x3d, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xc7, 0x9d, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0xe7, 0x8c, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x1c,
  0xe7, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7f, 0x9c, 0xe3, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x73, 0x9e, 0xf3, 0x30, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x71, 0x8e, 0xf3, 0x1d, 0x60,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe3, 0x38, 0xce,
  0xf3, 0x0f, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
  0xe3, 0x38, 0xce, 0xe9, 0x03, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xfe, 0xe3, 0x38, 0xc7, 0x0f, 0x00, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xdf, 0x73, 0x1c, 0xe7, 0x07, 0x00, 0x1c,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb8, 0xcf, 0x71, 0x9c, 0xdf,
  0x01, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xcf,
  0x79, 0x9c, 0x1f, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x78, 0xce, 0x39, 0x7c, 0x0f, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x38, 0xe7, 0x39, 0x7e, 0x00, 0x00, 0xe0, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xe7, 0x38, 0x3f, 0x00,
  0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x38, 0xe7,
  0xfc, 0x0d, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
  0x00, 0x9c, 0xe3, 0xfe, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xee, 0x80, 0x9c, 0xf3, 0x33, 0x00, 0x00, 0xf8, 0x1f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe6, 0xf0, 0x9c, 0xf3, 0x01, 0x00, 0x00,
  0xff, 0x07, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0xe7, 0xf8, 0x8e, 0xf3,
  0x00, 0x00, 0xf0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x80, 0xe3,
  0x38, 0xce, 0x07, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x30, 0x0e,
  0x30, 0x80, 0xf3, 0x04, 0xce, 0x03, 0x00, 0xf8, 0xff, 0x1f, 0x00, 0x00,
  0x00, 0x7c, 0x0e, 0x1e, 0xc0, 0x71, 0x00, 0x1e, 0x00, 0xc0, 0xff, 0xff,
  0x03, 0x00, 0x00, 0x00, 0x46, 0xc0, 0x1f, 0xc0, 0x71, 0x00, 0x1e, 0x00,
  0xfe, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x77, 0xe0, 0x1e, 0xc8, 0x71,
  0x00, 0x06, 0xe0, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x80, 0x7b, 0x02,
  0x0f, 0xee, 0x39, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00,
  0x80, 0xbb, 0x87, 0x87, 0xe7, 0x18, 0x00, 0xc0, 0xff, 0xff, 0x3f, 0x00,
  0x00, 0x00, 0x00, 0xc0, 0xbd, 0xc7, 0xc3, 0xe3, 0x1c, 0x00, 0xf8, 0xff,
  0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xdd, 0xc3, 0x41, 0xe0, 0x0c,
  0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xcd, 0xe3,
  0x00, 0xe0, 0x07, 0xe0, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xe0, 0xa0, 0x73, 0x00, 0xc0, 0x01, 0xf8, 0xff, 0xff, 0x03, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xe0, 0xb0, 0xf3, 0x07, 0x00, 0x00, 0xfe, 0xff, 0x7f,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xdc, 0xf1, 0x07, 0x00, 0x80,
  0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xc7, 0x99,
  0x07, 0x00, 0xe0, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xf0, 0xe1, 0x89, 0x07, 0x00, 0xf8, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x70, 0xe1, 0x94, 0x07, 0x00, 0xfc, 0xff, 0x3f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xe1, 0xbf, 0x03, 0x00, 0xff,
  0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xe1, 0x9f,
  0x03, 0x80, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x38, 0xf1, 0xdd, 0x01, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xb8, 0xe1, 0xde, 0x01, 0xe0, 0xff, 0x3f, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x01, 0xce, 0x00, 0xf0, 0xff,
  0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x00, 0x62,
  0x00, 0xf8, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x9c, 0x00, 0x32, 0x00, 0xfc, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xce, 0x00, 0x1e, 0x00, 0xfe, 0xff, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4e, 0x00, 0x00, 0x00, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00,
  0x80, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x3e, 0x00, 0x00, 0x80, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0xe0, 0x03, 0x0e,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
  0x60, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#define LOGO_width 120
#define LOGO_height 62
#define LOGO_Xpos 5
#define LOGO_Ypos 40

//local Variables
bool build_screen = false;
int old_minute = 0;
float old_temp = 0;
int old_trip_distance = 0;
unsigned long blink_on_timer = 0;
unsigned long blink_off_timer = 0;


void st7735_init(void) {

  pinMode(DISPLAY_ON, OUTPUT);

  digitalWrite(DISPLAY_ON, LOW);
  delay(100);
  digitalWrite(DISPLAY_ON, HIGH);

  tft.initR();

  tft.setRotation(0);
  tft.fillScreen(BGCOLOR);
  tft.drawXBitmap(LOGO_Xpos, LOGO_Ypos, logo, LOGO_width, LOGO_height, FONTCOLOR);
  tft.setTextColor(FONTCOLOR, BGCOLOR);
  tft.setCursor(LOGO_Xpos, LOGO_Ypos + LOGO_height + 10);
  tft.print(F("Booting..."));
}

bool display_pwrsave() {
  #if defined DSP_PWRSAVE
    if ( ( display_active_timer > millis() ) || ( engine_running ) ) {
      //message(F(" Cannot initialize modem!\n"));
      digitalWrite(DISPLAY_ON, HIGH);
      return false;
    }
    else {
      digitalWrite(DISPLAY_ON, LOW);
      return true;
    }
  #endif
  return false;
}

void display_draw() {
  /*
  #if defined DSP_PWRSAVE
    if ( ( display_active_timer > millis() ) || ( engine_running ) ) {
      //message(F(" Cannot initialize modem!\n"));
      digitalWrite(DISPLAY_ON, HIGH);
    }
    else {
      digitalWrite(DISPLAY_ON, LOW);
    }
  #endif
  yield();
  */
}

void notifyscreen() {

  switch ( info_type ) {
    case 1:
      //bootmsg1 = F("INFO");
      //u8g2.drawXBM(Xpos + 5, Ypos, sign_info_width, sign_info_height, sign_info);

      //tft.fillScreen(BGCOLOR);
      tft.setTextColor(YELLOW, BGCOLOR);
      break;
    case 2:
      //bootmsg1 = F("ERROR");
      //u8g2.drawXBM(Xpos + 5, Ypos+10, sign_info_width, sign_info_height, sign_info);
      //tft.fillScreen(BGCOLOR);
      tft.setTextColor(RED, BGCOLOR);
      tft.setCursor(Xpos + 10, Ypos + 32);
      tft.setTextSize(2);
      tft.print(F("WARNING"));
      break;
  }

  //u8g2.setFontPosTop();
  //u8g2.setFont(medium_font);
  //tft.fillScreen(BGCOLOR);
  //tft.setTextColor(FONTCOLOR, BGCOLOR);
  tft.setCursor(Xpos + 10, Ypos + 64);
  tft.setTextSize(2);
  //u8g2.setCursor(Xpos + 24, Ypos + 10);
  //u8g2.setFont(medium_font);
  tft.print(info_text);
  //u8g2.setCursor(Xpos, Ypos + 16);

}

void menu_notify() {

  switch ( info_type ) {
    case 1:
      //bootmsg1 = F("INFO");
      //u8g2.drawXBM(Xpos + 5, Ypos, sign_info_width, sign_info_height, sign_info);

      //tft.fillScreen(BGCOLOR);
      tft.setTextColor(YELLOW, BGCOLOR);
      tft.setCursor(Xpos + 10, Ypos + 32);
      tft.setTextSize(2);
      tft.print(F("Info"));
      break;
    case 2:
      //bootmsg1 = F("ERROR");
      //u8g2.drawXBM(Xpos + 5, Ypos+10, sign_info_width, sign_info_height, sign_info);
      //tft.fillScreen(BGCOLOR);
      tft.setTextColor(RED, BGCOLOR);
      tft.setCursor(Xpos + 10, Ypos + 32);
      tft.setTextSize(2);
      tft.print(F("WARNING"));

      if ( blink_on_timer > millis() ) {
        display_blink(true);
      }
      else if ( blink_off_timer > millis() ) {
        display_blink(false);
      }
      else {
        blink_on_timer = millis() + 1000;
        blink_off_timer = blink_on_timer + 1000;
      }

      break;
  }
  if ( millis() >= MsgTimer ) {
    //notifyscreen();
    Serial.println("ABGELAUFEN");
    MainMenuPos++;
  }


  //u8g2.setFontPosTop();
  //u8g2.setFont(medium_font);
  //tft.fillScreen(BGCOLOR);
  //tft.setTextColor(FONTCOLOR, BGCOLOR);
  tft.setCursor(Xpos + 10, Ypos + 64);
  tft.setTextSize(1);
  //u8g2.setCursor(Xpos + 24, Ypos + 10);
  //u8g2.setFont(medium_font);
  tft.print(info_text);
  if ( info_val != 0 ) {
    tft.setTextSize(2);
    tft.setCursor(Xpos + 24, Ypos + 80);
    //Serial.println(info_val);
    tft.print(info_val, DEC);
    if ( info_unit == "dgC" ) {
      tft.print((char)247);
      tft.print(F("C"));
    }
    else {
      tft.print(info_unit);
    }
  }
  //u8g2.setCursor(Xpos, Ypos + 16);

}

void display_blink(bool blink) {
  if ( blink ) {
    tft.invertDisplay(true);
  }
  else {
    tft.invertDisplay(false);
  }
}

void bootscreen() {

  switch ( info_type ) {
    case 1:
      bootmsg1 = F("INFO");
      //u8g2.drawXBM(Xpos + 5, Ypos, sign_info_width, sign_info_height, sign_info);
      break;
    case 2:
      bootmsg1 = F("ERROR");
      //u8g2.drawXBM(Xpos + 5, Ypos+10, sign_info_width, sign_info_height, sign_info);
      break;
  }

  if ( info_type == 0 ) {
    tft.fillScreen(BGCOLOR);
    tft.drawXBitmap(LOGO_Xpos, LOGO_Ypos, logo, LOGO_width, LOGO_height, FONTCOLOR);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(LOGO_Xpos, LOGO_Ypos + LOGO_height + 10);
    tft.print(F("Booting..."));
  }
  else {
    tft.fillScreen(BGCOLOR);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    //u8g2.setFontPosTop();
    //u8g2.setFont(medium_font);
    tft.setCursor(Xpos + 10, Ypos);
    tft.setCursor(Xpos + 24, Ypos + 10);
    //u8g2.setFont(medium_font);
    tft.print(info_text);
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

void clear_screen() {
  tft.fillScreen(BGCOLOR);

  old_minute = 0;
  old_temp = 0;
  old_trip_distance = 0;

}

/*
 * standby menue
 */
//positions for the Home Screen
#define HOME_DATE_X 4
#define HOME_DATE_Y 4

#define HOME_TIME_X 8
#define HOME_TIME_Y 66

void menu_clock() {
  //temp_out = 28.6;

  tft.setFont();
  //clear_screen();

  if ( old_minute != minute ) {
    //print time insted of time
    //Rect(0, 0, Xsize, 100, BGCOLOR);
    tft.drawLine(0, TIME_LINE, Xsize, TIME_LINE, FONTCOLOR);

    //tft.setTextSize(0);
    //tft.setFont(FONT9);
    tft.setTextSize(2);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(HOME_DATE_X, HOME_DATE_Y);
    sprintf(buf, "%02d.%02d.%04d", day, month, year);
    tft.print(buf);


    tft.setFont();
    tft.setTextSize(4);
    //tft.setTextSize(1);
    //tft.setFont(FONT24);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(HOME_TIME_X, HOME_TIME_Y);
    sprintf(buf, "%02d:%02d", hour, minute);
    tft.print(buf);

    old_minute = minute;
  }


  draw_temp();

  /*tft.setFont();
  if ( old_temp != temp_out ) {
    tft.fillRect(0, TEMP_LINE+2, Xsize, 40, BGCOLOR);
    tft.drawLine(0, TEMP_LINE, Xsize, TEMP_LINE, FONTCOLOR);
    //tft.setTextSize(2);
    tft.setTextSize(0);
    tft.setFont(FONT9);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(TEMP_X, TEMP_Y);
    int m = (temp_out - ( (int) temp_out ))*10;
    int t = (int)temp_out;
    sprintf (buf, "%02d.%1d", t, m);
    tft.print(buf);
    //u8g2.print(temp_out, 1);
    tft.setTextSize(1);
    //tft.print(" \0xF7");
    tft.print((char)247);
    tft.print(F("C"));
    old_temp = temp_out;
  }*/


  /*if ( old_trip_distance != trip_distance ) {
    tft.setTextSize(1);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(TRIP_X, TRIP_Y);

    uint16_t m = trip_distance - uint32_t(( trip_distance / 1000 )) * 1000;
    sprintf(buf, "%04d.%1d", trip_distance / 1000, m / 100);
    tft.print(buf);
    tft.setTextSize(1);
    tft.print(F(" km"));

    tft.drawLine(6, TRIP_LINE, Xsize - 6 , TRIP_LINE, FONTCOLOR);

    old_trip_distance = trip_distance;

  }*/

  trip_distance+=10;






  /*switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos = MENU_debug_ports; break;
  }
  button_1 = 0;*/

}
void draw_time() {

  tft.drawLine(0, TIME_LINE, Xsize, TIME_LINE, FONTCOLOR);

  if ( (millis() <= MsgTimer) && (info_type == DISPLAY_WARNING) ) {
    tft.setCursor(TIME_X-12, TIME_Y);
    tft.setTextSize(2);
    tft.setTextColor(RED, BGCOLOR);
    tft.print(F("WARNING"));
  }
  else {
    if ( old_minute != minute ) {
      tft.setTextSize(2);
      tft.setTextColor(FONTCOLOR, BGCOLOR);
      tft.setCursor(TIME_X, TIME_Y);
      sprintf(buf, "%02d:%02d", hour, minute);
      tft.print(buf);
      old_minute = minute;
    }
  }
}

void draw_temp() {
  //temp_out = 28.6;
  if ( old_temp != temp_out ) {
    //tft.fillRect(0, TEMP_LINE+2, Xsize, 40, BGCOLOR);
    tft.drawLine(0, TEMP_LINE, Xsize, TEMP_LINE, FONTCOLOR);
    //tft.setTextSize(2);
    //tft.setTextSize(0);
    //tft.setFont(FONT9);
    tft.setTextSize(2);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(TEMP_X, TEMP_Y);
    int m = (temp_out - ( (int) temp_out ))*10;
    int t = (int)temp_out;
    sprintf (buf, "%02d.%1d", t, m);
    tft.print(buf);
    //u8g2.print(temp_out, 1);
    tft.setTextSize(1);
    //tft.print(" \0xF7");
    tft.print((char)247);
    tft.print(F("C"));
    old_temp = temp_out;
  }
}

void draw_trip() {
  if ( old_trip_distance != trip_distance ) {
    tft.setTextSize(1);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(TRIP_X, TRIP_Y);

    uint16_t m = trip_distance - uint32_t(( trip_distance / 1000 )) * 1000;
    sprintf(buf, "%04d.%1d", trip_distance / 1000, m / 100);
    tft.print(buf);
    tft.setTextSize(1);
    tft.print(F(" km"));

    tft.drawLine(6, TRIP_LINE, Xsize - 6 , TRIP_LINE, FONTCOLOR);

    old_trip_distance = trip_distance;

  }

  trip_distance+=10;
}



void menu_speed() {
  //temp_out = 28.6;

  tft.setFont();



  draw_time();
  draw_temp();
  draw_trip();


  tft.setTextSize(1);
  tft.setTextColor(FONTCOLOR, BGCOLOR);
  tft.setCursor(VAL_NAME_X, VAL_NAME_Y);
  tft.print(F("Speed"));

  tft.setTextSize(4);
  tft.setTextColor(FONTCOLOR, BGCOLOR);
  tft.setCursor(VALUE_X, VALUE_Y);
  sprintf (buf, "%03d", speed);
  tft.print(buf);
  tft.setTextSize(1);
  tft.print(F(" km/h"));


  /*switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos = MENU_debug_ports; break;
  }
  button_1 = 0;*/

}

/*
 * Trip Infos
 * distance, speed (avg, max), triptime
 */
void menu_trip() {

  draw_time();
  draw_temp();

  tft.setTextSize(1);
  tft.setTextColor(FONTCOLOR, BGCOLOR);
  tft.setCursor(VAL_NAME_X, VAL_NAME_Y);
  tft.print(F("Trip Infos: "));
  tft.setTextSize(2);
  tft.setTextColor(FONTCOLOR, BGCOLOR);
  tft.setCursor(VALUE_X, VALUE_Y);

  uint16_t m = gps_distance - ( gps_distance / 1000 ) * 1000;
  sprintf (buf, "%04d.%1dkm", gps_distance / 1000, m / 100);
  tft.print(buf);

  tft.setTextSize(2);
  tft.setCursor(VALUE_X-18, VALUE_Y+32);
  long tmp = engine_running_total + engine_running_sec;
  //u8g2.print(F("travel time: "));
  tft.print(F("in "));
  tft.print(tmp/3600, DEC);
  tft.print(F("h "));
//  if ( engine_running_sec > 3600 ) {
  tft.print((tmp - ((tmp/3600) * 3600)) / 60 , DEC);
  tft.print(F("min"));


}

/*
 * Dynamic Menu for all define values
 */
void menu_values() {
  while(true) {
    if (( *values[MenuValuesPos].port == 0 ) || ( !values[MenuValuesPos].show )) {
      if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
        MainMenuPos++;
        MenuValuesPos = 0;
        break;
      } else {
        MenuValuesPos++;
      }
    }
    /**else if ( !values[MenuValuesPos].show_off && !engine_running) {
      if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
        MainMenuPos++;
        MenuValuesPos = 0;
        break;
      } else {
        MenuValuesPos++;
      }
    }**/
    else {
      break;
    }
  }


  if ( MainMenuPos == MENU_values ) {
    draw_time();
    draw_temp();

    tft.setTextSize(1);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(VAL_NAME_X, VAL_NAME_Y);
    tft.print(values[MenuValuesPos].desc);

    tft.setTextSize(3);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(VALUE_X, VALUE_Y);


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
      tft.print(F(" "));
    }

    tft.print(*values[MenuValuesPos].value, values[MenuValuesPos].digits);
    tft.setTextSize(1);
    tft.print(values[MenuValuesPos].suffix);



    if ( next() ) {
      if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
        MainMenuPos++;
        MenuValuesPos = 0;
      } else {
        MenuValuesPos++;
        clear_screen();
      }
    }

    /*switch (button_1) {
      case 1:
        if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
          MainMenuPos++;
          MenuValuesPos = 0;
        } else {
          MenuValuesPos++;
          clear_screen();
        }
        break;
      case 2: MainMenuPos--; break;
    }
    button_1 = 0;
    **/
  }
}
/*
 * Clima
 */
void menu_clima() {
  int m = 0;

  draw_time();

  //if ( temp_out_port == 0 ) {
  //  MainMenuPos++;

  //} else {
    // use a different layout if humidity exists

    tft.setTextSize(1);
    tft.setTextColor(FONTCOLOR, BGCOLOR);
    tft.setCursor(VAL_NAME_X, VAL_NAME_Y);
    tft.print(F("Klima: "));

    tft.setCursor(VALUE_X-16, VALUE_Y);
    tft.print(F("Out : "));
    m = (temp_out - ( (int) temp_out ))*10;
    sprintf (buf, "%02d.%1d", (int)temp_out, m);
    tft.setTextSize(2);
    tft.print(buf);
    tft.setTextSize(1);
    tft.print((char)247);
    tft.print(F("C"));

    tft.setCursor(VALUE_X-8, VALUE_Y+20);
    tft.print(F("Hum: "));
    sprintf (buf, "%02d", int(hum_out));
    tft.setTextSize(2);
    tft.print(buf);
    tft.setTextSize(1);
    tft.print(F("%"));

    tft.setCursor(VALUE_X-18, VALUE_Y+40);
    tft.print(F("In  : "));
    m = (temp_in - ( (int) temp_in ))*10;
    sprintf (buf, "%02d.%1d", (int)temp_in, m);
    tft.setTextSize(2);
    tft.print(buf);
    tft.setTextSize(1);
    tft.print((char)247);
    tft.print(F("C"));

    tft.setCursor(VALUE_X-8, VALUE_Y+60);
    tft.print(F("Hum: "));
    sprintf (buf, "%02d", int(hum_in));
    tft.setTextSize(2);
    tft.print(buf);
    tft.setTextSize(1);
    tft.print(F("%"));


    /**if ( hum_out > 0 ) {
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
    }**/

  //}
  //MainMenuPos++;
}



/*
 * GPS informations
 */
void menu_gpsinfo() {

  draw_time();
  draw_temp();

  tft.setTextSize(1);
  tft.setTextColor(FONTCOLOR, BGCOLOR);
  tft.setCursor(VAL_NAME_X, VAL_NAME_Y);
  tft.print(F("GPS Infos: "));
  tft.setCursor(VALUE_X-12, VALUE_Y);
  tft.print(F("Pos : "));
  tft.print(gps_latitude, 3);
  tft.print(F(", "));
  tft.print(gps_longitude, 3);
  tft.setCursor(VALUE_X-12, VALUE_Y+16);
  tft.print(F("Alt : "));
  tft.print(gps_altitude, DEC);
  tft.print(F("m"));
  tft.setCursor(VALUE_X-12, VALUE_Y+32);
  tft.print(F("Empfang: "));
  tft.print(gps_used_satellites, DEC);
  tft.print(F("/"));
  tft.print(gps_view_satellites, DEC);
  //u8g2.print(F(" )"));

}


/*
 * Info Menu
 */
void menu_info() {

  tft.drawXBitmap(LOGO_Xpos, LOGO_Ypos-40, logo, LOGO_width, LOGO_height, FONTCOLOR);

  tft.setTextSize(1);
  tft.setTextColor(FONTCOLOR, BGCOLOR);
  tft.setCursor(VALUE_X-12, VALUE_Y+32);
  tft.print(F("Designed by Brun"));
  tft.setCursor(VALUE_X-12, VALUE_Y+48);
  tft.print(F("Version: "));
  tft.print(F(VERSION));
  tft.setCursor(VALUE_X-12, VALUE_Y+64);
  tft.print(F("Build: "));
  tft.print(F(BUILD));
  tft.setCursor(VALUE_X-12, VALUE_Y+80);
  tft.print(F("fiz-o-matic.net"));

}

/*
 * Options Menu
 */
void menu_optionen() {
  MainMenuPos++;
}


/*
 * Dynamic Menu for config parameters
 */
void menu_opt_config() {
  MainMenuPos++;
}

void menu_opt_features() {
  MainMenuPos++;
}

void menu_opt_ports() {
  MainMenuPos++;
}

void menu_save_config() {
  MainMenuPos++;
}

void menu_debug_ports() {
  MainMenuPos++;
}

void menu_debug_ports_2() {
  MainMenuPos++;
}

void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      if (i & 7) byte <<= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if (byte & 0x80) tft.drawPixel(x + i, y + j, color);
    }
  }
}

#endif
