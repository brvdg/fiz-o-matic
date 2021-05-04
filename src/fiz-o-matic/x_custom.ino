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
  notify(BOOTMSG, F("#Init custom functions"));
}

void custom_loop() {

  // to save cpu power run this function only every (above defined) time
  if ( custom_timer < millis() ) {
    // put your code here

    custom_timer = millis() + CUSTOM_TIMER;
  }
}


#else // CUSTOM
void custom_init() {}
void custom_loop() {}
#endif // CUSTOM
