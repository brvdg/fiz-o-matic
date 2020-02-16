/****************************************************
 * FIZ-o-matic
 * https://fiz-o-matic.net/
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/


#define VERSION "0.8"
#define BUILD "140220a"


#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/dtostrf.h>
#include <Scheduler.h>



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

/*
 * Copy the .bin file to the SD card and rename
 * the file to UPDATE.bin.
 */
#if defined SDU
//#error "<SDU.h>"
#include <SDU.h>
#endif


/*
 * Timer Interrupts
 */
#include "Adafruit_ZeroTimer.h"
Adafruit_ZeroTimer zt4 = Adafruit_ZeroTimer(4);
void TC4_Handler(){
  Adafruit_ZeroTimer::timerHandler(4);
}


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
  //#ifdef U8G2_DISPLAY
  #ifdef ENABLE_DISPLAY
  //delay(10000);
  //Serial.println("DISPLAY...");
  display_init();
  //display_active_timer = millis() + 1000 * 90; // 30 sec.
  display_active_timer = millis() + 1000 * 90; // 30 sec.

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

  reg_port(0xF1, TYPE_Volt);
  reg_port(0xF2, TYPE_Volt);
  reg_port(0xF3, TYPE_GradCelsius);
  reg_port(0xF4, TYPE_Humidity);
  reg_port(0xF5, TYPE_GradCelsius);
  reg_port(0xF6, TYPE_Humidity);
  update_port_value( 0xF1, 8.4 );
  update_port_value( 0xF2, 6.8 );
  update_port_value( 0xF3, 12.3 );
  update_port_value( 0xF4, 80 );
  update_port_value( 0xF5, 23.4 );
  update_port_value( 0xF6, 64 );

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

/*
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
*/

  //
  /********************* Timer #3, 8 bit, one callback with adjustable period = 350KHz ~ 2.86us for DAC updates */
  /*zt3.configure(TC_CLOCK_PRESCALER_DIV1, // prescaler
                TC_COUNTER_SIZE_8BIT,   // bit width of timer/counter
                TC_WAVE_GENERATION_MATCH_PWM  // match style
                );

  zt3.setPeriodMatch(150, 1, 0); // ~350khz, 1 match, channel 0
  zt3.setCallback(true, TC_CALLBACK_CC_CHANNEL0, Timer3Callback0);  // set callback
  zt3.enable(true);*/


  /********************* Timer #4, 8 bit, one callback with adjustable period */
  zt4.configure(TC_CLOCK_PRESCALER_DIV1024, // prescaler
                TC_COUNTER_SIZE_32BIT,   // bit width of timer/counter
                TC_WAVE_GENERATION_MATCH_PWM  // match style
                );

  zt4.setPeriodMatch(150, 100, 0); // 1 match, channel 0
  zt4.setCallback(true, TC_CALLBACK_CC_CHANNEL0, Timer4Callback0);  // set DAC in the callback
  zt4.enable(true);



  // at least custom functions
  #ifdef CUSTOM
  custom_init();
  #endif // CUSTOM



  // Watchdog
  watchdog_timer = millis() + WATCHDOG_TIMER*10;


  //#ifdef U8G2_DISPLAY
  #ifdef ENABLE_DISPLAY

  if ( MainMenuPos == 0 ) {
    MainMenuPos = 1;
  }
  //boot = false;
  //display_update_timer_lock = true;
  display_loop();
  display_update_timer_lock = false;
  #endif // LCD


  //Scheduler.startLoop(loop1);
  Scheduler.startLoop(status_checker);

  //Scheduler.startLoop(tinygsm_gps_loop);
  //Scheduler.startLoop(tinygsm_loop);
  Scheduler.startLoop(display_loop);


  message(F("#---> Ready after "));
  message(String(millis()/1000, DEC));
  message(F("s\n"));


}

/*
 * The main loop
 */
void loop() {

  // Check millis rollover
  if ( millisRollover )
  {
    status_checker_timer = 0;
    online_intervalll_timer = 0;
    display_update_timer = 0;
    alarm_timer = 0;
    tail_timer = 0;
    log_timer = 0;
    trip_timer = 0;
    update_vars_timer = 0;
    IO_timer = 0;
    aux_heater_timer = 0;

    //tinygsm_sms_timer = 0;
    //tinygsm_blynk_timer = 0;
  }

  tinygsm_loop();

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

  //#ifdef U8G2_DISPLAY
  #ifdef ENABLE_DISPLAY
  if ( display_update_timer < millis() ) {
    display_update_timer = millis() + U8G2_DISPLAY_UPDATE_TIMER;
    //display_loop();
    //Serial.println("DISPLAY...");
  }
  #endif // U8G2_DISPLAY

  // Button...
  //button();

  // at least custom functions
  #ifdef CUSTOM
  custom_loop();
  #endif // CUSTOM

  delay(10);
}

void loop1 () {
  //if ( !tinygsm_lock ) {
  //  tinygsm_lock = true;

    //if ( timer_check(&tinygsm_gps_timer, TinyGSM_GPS_TIMER) ) {
    //  if ( tinygsm_gps_ok ) {
        //tinygsm_gps_loop();
    //  }
    //}
  //  tinygsm_lock = false;
    //message(INFO_MSG, F("loop1 finished\n"));
  //}
  delay(100);
  yield();
}



bool timer_check(unsigned long *timer, unsigned long delay)
{

  if ( *timer + delay < millis() ) {
    //message(INFO_MSG, F("#it's time\n"));
    *timer = millis();
    return true;
  }
  if ( *timer > millis() ) {
    *timer = millis();
    return true;
  }

  return false;
}

bool millisRollover() {
  // found at:
  // https://www.faludi.com/2007/12/18/arduino-millis-rollover-handling/

  // get the current millis() value for how long the microcontroller has been running
  //
  // To avoid any possiblity of missing the rollover, we use a boolean toggle that gets flipped
  //   off any time during the first half of the total millis period and
  //   then on during the second half of the total millis period.
  // This would work even if the function were only run once every 4.5 hours, though typically,
  //   the function should be called as frequently as possible to capture the actual moment of rollover.
  // The rollover counter is good for over 35 years of runtime. --Rob Faludi http://rob.faludi.com
  //
  //static int numRollovers=0; // variable that permanently holds the number of rollovers since startup
  static boolean readyToRoll = false; // tracks whether we've made it halfway to rollover
  unsigned long now = millis(); // the time right now
  unsigned long halfwayMillis = 2147483647; // this is halfway to the max millis value (17179868 for earlier versions of Arduino)

  if (now > halfwayMillis) { // as long as the value is greater than halfway to the max
    readyToRoll = true; // you are ready to roll over
  }

  if (readyToRoll == true && now < halfwayMillis) {
    // if we've previously made it to halfway
    // and the current millis() value is now _less_ than the halfway mark
    // then we have rolled over
    readyToRoll = false; // we're no longer past halfway

    return true;

  }
  else {
    return false;
  }
}



/*
 *   Internal Timer
 */
 /*
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
    //button();
  }

}*/


void Timer4Callback0()
{
  //Serial.print(F("."));
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
