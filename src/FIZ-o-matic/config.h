/****************************************************
   fiz-o-matic Configuration
   for detailed informations see
   https://github.com/brvdg/busputer/wiki
 ****************************************************/

/*
   Hardware configuration
*/

//#define HW_FEATHER
//#define HW_TRINKET

// Display Configuration
//#define U8G2_DISPLAY
//#define DOGS102
//#define OLED
//#define OLED_FULL
//#define NOKIA




// Port defination
//#ifdef U8G2_DISPLAY
//#define DISPLAY_BG_LED 10 //turn this off if a OLED ist used
//#define U8G2_DISPLAY_CS 11
//#define U8G2_DISPLAY_DC 12
//#define U8G2_DISPLAY_RST 10

// defination of LCD dimmer
#define DIMMER 2 //Port for the cluster dimmer source
#define DIMMER_MAX_mV 13800
#define DIMMER_MIN_mV 4000
//#define DIMMER_MAX 150
#define DIMMER_MIN 20

// how long ist the display active in the standby after an action
#define DISPLAY_ACTIVE_TIME 30000

//#define CLOCK_VIEW 1 // 1 = clock and temperature / 0 = clock and date

#define DISPLAY_TEMP false // display temperature insted of date in standby

// enable SD card
//#define SDCard


//TinyGSM configuration
//#define TinyGSM
#define SIM_PIN ""
#define APN "internet.t-mobile"
#define APN_USER "t-mobile"
#define APN_PASS "tm"
#define SMS_Keyword "keyword"

#define BLYNK_KEY "0123456789abcdefghijkl"

#define MYNUMBER "+4915141284285"
#define BLYNK_DEVICE_NAME "MyCar"

// if this is turned on
// we are online if the car is turned off
#define ONLINE_INTERVALL 15 // interval in min

#define BLYNK_VIRTUAL_terminal V0
#define BLYNK_VIRTUAL_map V1
#define BLYNK_VIRTUAL_gps_used_satellites V2
#define BLYNK_VIRTUAL_gps_view_satellites V3
#define BLYNK_VIRTUAL_gps_latitude V4
#define BLYNK_VIRTUAL_gps_longitude V5
#define BLYNK_VIRTUAL_gps_altitude V6
#define BLYNK_VIRTUAL_bord_voltage V7
#define BLYNK_VIRTUAL_stay_online V8
#define BLYNK_VIRTUAL_online_LED V9
#define BLYNK_VIRTUAL_geoalarm_modus V10
#define BLYNK_VIRTUAL_geo_fence_armed_led V11
#define BLYNK_VIRTUAL_geo_fence_distance V12
#define BLYNK_VIRTUAL_geo_fence_led V13
#define BLYNK_VIRTUAL_alarm V14
#define BLYNK_VIRTUAL_armed_led V15
#define BLYNK_VIRTUAL_alarm_led V16
#define BLYNK_VIRTUAL_aux_heater V17
#define BLYNK_VIRTUAL_temp_out V18
#define BLYNK_VIRTUAL_hum_out V19
#define BLYNK_VIRTUAL_temp_in V20
#define BLYNK_VIRTUAL_hum_in V21

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
#define RPM_PORT 5
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
#define FUEL_EMPTY 168
#define FUEL_L 60
#define FUEL_WARNING 15


// Alarm output
#define ALARM_OUT
#define ALARM_PORT 13


// bord voltage
#define BORD_VOLTAGE_PORT 1

// OIL
#define OIL_TEMP_PORT 0
#define OIL_PRESSURE_PORT 0
#define OIL_PRESSURE_WARNING 2
#define OIL_TEMP_WARNING 130


// Port for Button input
#define BUTTON_PIN_1 9

// for Dalles DS18B20 Temperatursensor
#define ONEWIRE
#define ONE_WIRE_BUS 6
#define ONE_WIRE_BUS_PORT 0x07 // need this declaration for plausibility check

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
#include <BlynkSimpleSIM800.h>






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

#define TinyGSM_TIMER 2000 // 1s
#define TinyGSM_GPS_TIMER 1000 // 0,5s
#define TinyGSM_SMS_TIMER 15000 // 15s
#define TinyGSM_BATT_TIMER 30000 // 30s
#define TinyGSM_BLYNK_TIMER 5000 // 5s
#define TinyGSM_ONLINE_TIMER 30000


//#define TinyGSM_GPS_STATUS_TIMER 3000 // 3s
//#define BLYNK_CHECK_TIMER 600000 // 10min




// Number of values for an average
#define IO_ARRAY 5




// enable Debugging
//#define OFFLINE // disable some functions for programming and testing
/*#define INFO
#define DEBUG
#define TINYGSM_DEBUG
#define SD_DEBUG
#define TRACE*/

//#ifdef TINYGSM_DEBUG
//#define BLYNK_PRINT Serial // Defines the object that is used for printing
//#define BLYNK_DEBUG        // Optional, this enables more detailed prints
//#endif

// print the status on serial port
//#define PRINT_STATUS

/*#ifdef INFO
 #define INFO_PRINT(x)  Serial.print (x)
 #define INFO_PRINTLN(x)  Serial.println (x)
 #define INFO_PRINTHEX(x)  Serial.print (x, HEX)
#else
 #define INFO_PRINT(x)
 #define INFO_PRINTLN(x)
 #define INFO_PRINTHEX(x)
#endif*/
/*#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.print (x)
 #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTLN(x)
#endif*/
/*#ifdef TINYGSM_DEBUG
 #define TINYGSM_DEBUG_PRINT(x)  Serial.print (x)
 #define TINYGSM_DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define TINYGSM_DEBUG_PRINT(x)
 #define TINYGSM_DEBUG_PRINTLN(x)
#endif*/
/*#ifdef SD_DEBUG
 #define SD_DEBUG_PRINT(x)  Serial.print (x)
 #define SD_DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define SD_DEBUG_PRINT(x)
 #define SD_DEBUG_PRINTLN(x)
#endif*/


/*#ifdef TRACE
 #define TRACE_PRINT(x)  Serial.print (x)
 #define TRACE_PRINTLN(x)  Serial.println (x)
#else
 #define TRACE_PRINT(x)
 #define TRACE_PRINTLN(x)
#endif*/
