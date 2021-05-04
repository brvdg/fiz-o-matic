/****************************************************
 * fiz-o-matic
 * https://fiz-o-matic.net/
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/


#define VERSION "0.8"
#define BUILD "210506a"


#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/dtostrf.h>
#include <Scheduler.h>
#include <SPI.h>



// Include EEPROM-like API for FlashStorage
#include <FlashAsEEPROM.h>
#include <FlashStorage.h>



// include configuration file
#include "config.h"
#include "hardware.h"
#include "vars_can.h"
#include "tinygsm.h"
#include "blynk.h"
#include "vars.h"

#include "display.h"

/*
 * RTC
 */
#include <RTCZero.h>
RTCZero rtc;
//RTC_Millis rtc;



/*
 * Copy the .bin file to the SD card and rename
 * the file to UPDATE.bin.
 */
#if defined SDU
//#error "<SDU.h>"
#include <SDU.h>
#endif

#ifdef TinyGPS
#include <TinyGPS++.h>
#endif

/*
 * Timer Interrupts
 */
#include "Adafruit_ZeroTimer.h"
Adafruit_ZeroTimer zt4 = Adafruit_ZeroTimer(4);
void TC4_Handler(){
  Adafruit_ZeroTimer::timerHandler(4);
}

// internal Watchdog
#include <Adafruit_SleepyDog.h>


void setup() {

  #ifdef DEBUG
  delay(10000);
  #endif

  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));

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

  message(BOOTMSG, F("#Booting..."));
  message(BOOTMSG, F(BUILD));
  message(BOOTMSG, F("\n"));
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
  message(BOOTMSG, F("#READ Virtual EEPROM"));
  read_virtual_eeprom();

  // plausibility check for variables
  message(BOOTMSG, F("#check plausibility"));
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
    message(BOOTMSG, F("#SD is disabled"));
  }

  #ifdef HW_FEATHER_EXPRESS
  init_spiflash();
  spiflash_open_config();
  #endif


  message(BOOTMSG, F("#check plausibility"));
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
  can_init();



  /*
   * TinyGSM
   */
  tinygsm_init();
  tinygsm_gps_init();


  /*
   *
   */
  tinygps_init();

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


  // internal Watchdog
  message(BOOTMSG, F("#Init Watchdog"));
  //Watchdog.enable(4000);

  /*
   * enable Timer
   */
  message(BOOTMSG, F("#Init Timer"));
  // set time for Software Watchdog
  watchdog_timer = millis() + WATCHDOG_TIMER*10;



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



  // Software Watchdog
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
  //Scheduler.startLoop(status_checker);

  //Scheduler.startLoop(tinygsm_gps_loop);
  //Scheduler.startLoop(tinygsm_loop);
  Scheduler.startLoop(display_loop);

  //Scheduler.startLoop(watchdog_loop);


  message(F("#---> Ready after "));
  message(String(millis()/1000, DEC));
  message(F("s\n"));




}

/*
 * The main loop
 */
void loop() {
  // internal Wtchdog
  Watchdog.reset();

  // Check millis rollover
  if ( millisRollover() ) {
    message ( INFO_MSG, "millisRollover");
    status_checker_timer = 0;
    tinygsm_timer = 0;
    tinygsm_sms_timer = 0;
    tinygsm_blynk_timer = 0;
    gps_timer = 0;
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

  tinygps_loop();

  can_loop();

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


  // calculating trip informations
  trip();

  if ( engine_running ) {
    log_to_sdcard();
  }

  // get the Port values
  IO_loop();

  // at least custom functions
  #ifdef CUSTOM
  custom_loop();
  #endif // CUSTOM

  //delay(10);
}



void watchdog_loop () {
  Watchdog.reset();
  delay(500);
  yield();
}



/*
  returns TRUE if timer + delay is greather then millis()
*/
bool timer_check(unsigned long *timer, unsigned long delay)
{

  if ( *timer + delay < millis() ) {
    //message(INFO_MSG, String(millis(), DEC));
    //message(INFO_MSG, String(*timer, DEC));
    *timer = millis();
    //message(INFO_MSG, String(*timer, DEC));
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

  // Software Watchdog
  if ( watchdog_timer < millis() ) {
      // reset the system
      Serial.println(F("#Watchdog reset"));
      flash_watchdog_reset.write(true);
      NVIC_SystemReset();
  }

  // internal Wtchdog
  //Watchdog.reset();

  // Button ...
  button();


}
