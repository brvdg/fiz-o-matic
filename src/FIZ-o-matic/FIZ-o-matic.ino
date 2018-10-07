/****************************************************
 * FIZ-o-matic
 * https://github.com/brvdg/fiz-o-matic
 *
 * Author: Brun
 * License: MIT
 ****************************************************/


#define VERSION "0.8"
#define BUILD "181006a"


#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/dtostrf.h>
#include <Scheduler.h>

/*
 * Copy the .bin file to the SD card and rename
 * the file to UPDATE.bin.
 */
#if defined SDCARD
#include <SDU.h>
#endif

// Include EEPROM-like API for FlashStorage
#include <FlashAsEEPROM.h>
#include <FlashStorage.h>


// include configuration file
#include "config.h"
#include "hardware.h"
#include "vars.h"

/*
 * RTC
 */
#include <RTCZero.h>
RTCZero rtc;
//RTC_Millis rtc;

#ifdef U8G2_DISPLAY
#include <U8g2lib.h>
#endif




void setup() {

  #ifdef DEBUG
  delay(5000);
  #endif

  //delay(3000);

  Serial.begin(9600);
  //delay(2000);
  Serial.print( F("#Compiled: "));
  Serial.print( F(__DATE__));
  Serial.print( F(", "));
  Serial.print( F(__TIME__));
  Serial.print( F(", "));
  Serial.println( F(__VERSION__));


  /*
   * Display
   */
  #ifdef U8G2_DISPLAY
  display_init();
  dimmer_active_timer = millis() + 1000 * 30; // 30 sec.
  #endif

  notify(BOOTMSG, F("#Booting..."));
  notify(BOOTMSG, F(BUILD));
  notify(BOOTMSG, F("\n"));
  //delay(5000);

  /*
   * IO
   */
  IO_init();

  #ifdef FeatherLED8
  pinMode(FeatherLED8, OUTPUT);
  digitalWrite(FeatherLED8, LOW);
  #endif //FeatherLED8

  // Button
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);

  /*
   * read config from FlashStorage
   */
  notify(BOOTMSG, F("#READ FLASH"));
  read_virtual_eeprom();

  // plausibility check for variables
  notify(BOOTMSG, F("#check plausibility"));
  check_plausibility();

  /*
   * read config from  SD Card
   */
  if ( sd_enabled ) {
    sdcard_init();
    if ( sd_ok ) {
      //open the configuration from SD
      sdcard_open_config();
      //Check the logs
      get_last_log();
    }
  }
  else {
    notify(BOOTMSG, F("#SD is disabled"));
  }
  notify(BOOTMSG, F("#check plausibility"));
  check_plausibility();


  /*
   * initialize the IO Ports
   */

  //reg_port(0x01, TYPE_Volt);
  //reg_port(0x02, TYPE_Volt);
  //reg_port(0x03, TYPE_Volt);
  //reg_port(0x04, TYPE_Volt);
  //reg_port(0x05, TYPE_Hz);
  //reg_port(0x06, TYPE_Hz);

  //IO_init();

  /*reg_port(0xC1);
  reg_port(0xC2);
  reg_port(0xC3);
  reg_port(0xC4);
  reg_port(0xC5);*/


  /*
   * TinyGSM
   */
  tinygsm_init();
  tinygsm_gps_init();

  /*
   * OneWire Bus
   */
  if ( onewire_enabled ) {
    onewire_init();
  }
  /*
   * I2C Sensors
   */
  i2c_init();

  /*
   * RTC
   */
  rtc.begin();


  /*
   * enable Timer
   */
  // set time for Watchdog
  watchdog_timer = millis() + WATCHDOG_TIMER*10;

  // Enable clock for TC
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID ( GCM_TCC2_TC3 ) ) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync
  // The type cast must fit with the selected timer mode
  TcCount16* TC = (TcCount16*) TC3; // get timer struct
  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TCx
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;  // Set Timer counter Mode to 16 bits
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_NFRQ; // Set TC as normal Normal Frq
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;   // Set perscaler
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  // TC->PER.reg = 0xFF;   // Set counter Top using the PER register but the 16/32 bit timer counts allway to max
  // while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  TC->CC[0].reg = 0xFFF;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  // Interrupts
  TC->INTENSET.reg = 0;              // disable all interrupts
  TC->INTENSET.bit.OVF = 1;          // enable overfollow
  //TC->INTENSET.bit.MC0 = 1;          // enable compare match to CC0
  // Enable InterruptVector
  NVIC_EnableIRQ(TC3_IRQn);
  // Enable TC
  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync


  //
  /********************* Timer #3, 8 bit, one callback with adjustable period = 350KHz ~ 2.86us for DAC updates */
  /*zt3.configure(TC_CLOCK_PRESCALER_DIV1, // prescaler
                TC_COUNTER_SIZE_8BIT,   // bit width of timer/counter
                TC_WAVE_GENERATION_MATCH_PWM  // match style
                );

  zt3.setPeriodMatch(150, 1, 0); // ~350khz, 1 match, channel 0
  zt3.setCallback(true, TC_CALLBACK_CC_CHANNEL0, Timer3Callback0);  // set callback
  zt3.enable(true);*/

  // at least custom functions
  #ifdef CUSTOM
  custom_init();
  #endif // CUSTOM



  // Watchdog
  watchdog_timer = millis() + WATCHDOG_TIMER*10;


  #ifdef U8G2_DISPLAY
  MainMenuPos = 1;
  //boot = false;
  display_update_timer_lock = true;
  display_update();
  display_update_timer_lock = false;
  #endif // LCD


  Scheduler.startLoop(loop1);


  message(F("#---> Ready after "));
  message(String(millis()/1000, DEC));
  message(F("s\n"));


}

/*
 * The main loop
 */
void loop() {

  // Reset the watchdog with every loop to make sure the sketch keeps running.
  // If you comment out this call watch what happens after about 4 iterations!
  watchdog_timer = millis() + WATCHDOG_TIMER;

  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      serial_parse();
      inputString = "";
    }
  }

  // Tail mode to print periodicly the variables
  if (tail) {
    if ( tail_timer < millis() ) {
      tail_timer = millis() + TAIL_TIMER;
      serial_print_status();
    }
  }


  #ifdef FeatherLED8
  digitalWrite(FeatherLED8, HIGH);
  #endif FeatherLED8

  // Status Checker
  status_checker();

  /*
   * TinyGSM Part
   */
  if ( !tinygsm_lock ) {
    tinygsm_lock = true;

    if ( tinygms_ok ) {
      tinygsm_loop();
    }

    if ( tinygsm_gps_ok ) {
      //tinygsm_gps_loop();
    }

    tinygsm_lock = false;
  }


  #ifdef FeatherLED8
  digitalWrite(FeatherLED8, LOW);
  #endif FeatherLED8


  onewire_loop();
  i2c_loop();
  //update_vars();

  // calculating trip informations
  trip();

  if ( engine_running ) {
    log_to_sdcard();
  }

  // get the Port values
  IO_loop();

  #ifdef U8G2_DISPLAY
  if ( display_update_timer < millis() ) {
    display_update_timer = millis() + U8G2_DISPLAY_UPDATE_TIMER;
    display_loop();
  }
  #endif // U8G2_DISPLAY

  // Button...
  button();

  // Status Checker
  //status_checker();

  //alarm = 1;
  //make_alarm();
  //alarm_loop();

  // at least custom functions
  #ifdef CUSTOM
  custom_loop();
  #endif // CUSTOM

  delay(10);
}

void loop1 () {
  if ( !tinygsm_lock ) {
    tinygsm_lock = true;

    if ( tinygsm_gps_ok ) {
      tinygsm_gps_loop();
    }
    tinygsm_lock = false;
    //message(INFO_MSG, F("loop1 finished\n"));
  }
  delay(1000);
  yield();
}





/*
 *   Internal Timer
 */
void TC3_Handler()
{

  TcCount16* TC = (TcCount16*) TC3; // get timer struct
  if (TC->INTFLAG.bit.OVF == 1) {  // A overflow caused the interrupt
    TC->INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag

    if ( alarm_timer < millis() ) {
      alarm_timer = millis() + ALARM_TIMER;
      alarm_loop();

    }

    // Wagchdog
    if ( watchdog_timer < millis() ) {
        // reset the system
        Serial.println(F("#Watchdog reset"));
        flash_watchdog_reset.write(true);
        NVIC_SystemReset();
    }

    // Button ...
    button();
  }

}
