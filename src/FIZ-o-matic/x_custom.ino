/***************************************************
 *  Here you can define your own code.
 *  Is your code good, you can publish it and we can commit it in the main tree.
 *
 *  Dont forget to write a short decription
 *
 *  Author: You
 *
 *  Funktion: you define it.
 *
 ****************************************************/

// enable the custom module
#define CUSTOM

#define CUSTOM_TIMER 200 // 200ms
unsigned long custom_timer = 0;

#ifdef CUSTOM


void custom_init() {
  notify(BOOTMSG, F("Init custom functions"));
}

void custom_loop() {

  // to save cpu power run this function only every (above defined) time
  if ( custom_timer < millis() ) {
    // put your code here

    custom_timer = millis() + CUSTOM_TIMER;
  }
}

/*
 * part for the custum menu
 */
#ifdef U8G2_DISPLAY
#ifdef ARDUINO_ARCH_SAMD
/*void custom_menu() {
  //char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("User defined Men\xfc")); // \xfc = ü

  u8g2.setCursor(Xpos, Ypos+8);
  u8g2.print(F("This is a"));
  u8g2.setCursor(Xpos, Ypos+16);
  u8g2.print(F("selbstgemachtes Men\xfc"));


  // this is needed to reackt on the button action
  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;

}*/
#endif // ARDUINO_ARCH_SAMD
#endif // U8G2_DISPLAY
#else // CUSTOM

#endif // CUSTOM
