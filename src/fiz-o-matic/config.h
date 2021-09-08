/****************************************************
 * FIZ-o-matic
 * https://fiz-o-matic.net/
 *
 * fiz-o-matic configuration
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/


// defination of LCD dimmer
#define DIMMER 2 //Port for the cluster dimmer source
#define DIMMER_MAX_mV 13800
#define DIMMER_MIN_mV 4000
//#define DIMMER_MAX 150
#define DIMMER_MIN 20

// how long ist the display active in the standby after an action
#define DISPLAY_ACTIVE_TIME 30000


// enable SD card
//#define SDCard


//TinyGSM configuration
//#define TinyGSM
#define SIM_PIN ""
#define APN ""
#define APN_USER "t-mobile"
#define APN_PASS "tm"
#define SMS_KEYWORD "fiz"

#ifndef BLYNK_KEY
#define BLYNK_KEY "0123456789abcd"
#endif

#define MY_NUMBER "+491234567890"
#define BLYNK_DEVICE_NAME "MyCar"

// if this is turned on
// we are online if the car is turned off
#define ONLINE_INTERVALL 15 // interval in min



#define USE_GPS_SPEED true // use GPS for speed Signal

/*
    Temperature source
*/
//#define TEMP_OUT_PORT 0xF3
#define TEMP_OUT_PORT 0x30
#define HUM_OUT_PORT 0xF4
#define TEMP_IN_PORT 0xF5
#define HUM_IN_PORT 0xF6

/*
   Speed source
   1 = Speedpulse
   2 = GPS
*/
#define SPEEDSOURCE 2
#define SPEED_OFFSET 0

// Speedpulse Port
#define SPEEDPULSE_PORT 6
#define SPEEDPULSEARRAY 25


// RPM gauge
#define RPM_PORT 0x05
#define RPM_MULTIPL 2

// Water Temp gauge
#define WATER_TEMP 0x03
//#define WATER_TEMP 0x80
#define SERIESRESISTOR 50 // Ohm of the original gauge
#define WATER_TEMP_WARNING 110

// Fuel gauge
#define FUEL_GAUGE_PORT 0x04
//#define FUEL_GAUGE_PORT 0x81
#define FUEL_GAUGERESISTOR 48
#define FUEL_FULL 34
#define FUEL_EMPTY 175
#define FUEL_L 60
#define FUEL_WARNING 15


// Alarm output
#define ALARM_OUT
#define ALARM_PORT 13
//#define ALARM_PORT 0x08


// bord voltage
#define BORD_VOLTAGE_PORT 1

// OIL
#define OIL_TEMP_PORT 0
#define OIL_PRESSURE_PORT 0
#define OIL_PRESSURE_WARNING 2
#define OIL_TEMP_WARNING 130


// Port for Button input
#define BUTTON_PIN_1 9



// I2S Sensors
#define I2C
//#define SI7021


/*
   Watchdog
   This is not the Atmel internal Watchdag.
   The Watchdog is based on an timer interrrupt
*/
#define WATCHDOG_TIMER 5 * 60000 // 120s
//#define WATCHDOG_TIMER 50 // 120s


/*
 * Blynk definations
 */

#ifdef DEBUG
  #define TINY_GSM_DEBUG Serial
  #define BLYNK_PRINT Serial // Defines the object that is used for printing
  #define BLYNK_DEBUG        // Optional, this enables more detailed prints
#endif

//#define TINY_GSM_YIELD() { delay(10); yield(); }

// Select your modem:
//#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_M590

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>



/*
 * CAN Bus definations
 */

 #define CAN_time_MASTER
 #define CAN_gps_MASTER
 #define CAN_voltage_Master
 #define CAN_rpm_Master
 #define CAN_fuel_water_MASTER
 #define CAN_oil_MASTER
 #define CAN_klima_MASTER
 #define CAN_trip_MASTER




/*
   Timer for the seperate Tasks
*/

#define U8G2_DISPLAY_UPDATE_TIMER 40 // 200ms
#define PRINT_STATUS_TIMER 2000 //
#define UPDATE_VARS_TIMER 200 // 200ms
#define BUTTON_TIMER 200 // 200ms
#define I2C_TIMER 200 // 200ms
#define ONEWIRE_TIMER 5000 // 5s
//#define ANALOG_TIMER 1000 // 1000ms
#define IO_TIMER 2 // 200ms
#define ALARM_TIMER 100 // 100ms
#define LOG_TIMER 1000 // 1s
#define STATUS_CHECKER_TIMER 200 // 200ms
#define TAIL_TIMER 2000 // 2sec
#define FUEL_TIMER 5000 // 5sec

#define TinyGSM_TIMER 2000 // 1s

#define TinyGSM_SMS_TIMER 15000 // 15s
#define TinyGSM_BATT_TIMER 30000 // 30s
#define TinyGSM_BLYNK_TIMER 5000 // 5s
#define TinyGSM_ONLINE_TIMER 30000

#define GPS_TIMER 1000 // 0,5s
#define CANBUS_TIMER 1000 // 1s

//#define TinyGSM_GPS_STATUS_TIMER 3000 // 3s
//#define BLYNK_CHECK_TIMER 600000 // 10min




// Number of values for an average
#define IO_ARRAY 100
