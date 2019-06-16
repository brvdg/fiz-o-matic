/****************************************************
 * fiz-o-matic Variables
 *
 * for detailed informations see
 * https://github.com/brvdg/busputer/wiki
 ****************************************************/



/*
 * global vars
 */
//String bootmsg;
String bootmsg1;
String bootmsg2;
String bootmsg3;
//boolean boot=true;



// this is a large buffer for replies
char replybuffer[255];
// a smal buffer
char buf[36]; //

bool serial_export = true;
bool SPI_lock = false;
bool I2C_lock = false;
byte saved_config = 0;



/*
 * Debugging Variable
 */
#ifdef DEBUG
byte debug = 143;
#else
byte debug = 0;
#endif

#define ERROR 0
#define INFO_MSG 1
#define DEBUG_MSG 2
#define DEBUG_TINYGSM 4
#define DEBUG_SD 8
#define DEBUG_IO 16
//#define DEBUG_ 32
//#define TRACE_TINYGSM 64
#define TRACE_MSG 128

/*
 * definations for different kinds of notifycations
 */
#define DISPLAY_INFO 1
#define DISPLAY_WARNING 2
#define SOUND_INFO 4
#define SOUND_WARNING 8
#define SOUND_ERROR 12
#define BLYNK_MSG 16
#define BLYNK_PUSH 32
#define SMS 64
#define BOOTMSG 128


/*
 * to print different messages on the display
 *
 *  0 = nothing
 *  1 = info
 *  2 = error
 *  3 = internal error
 *  4 = engine Warning
 */
byte info_type = 0;
String info_text;


/*
 * Variables ...
 */
// Temperature / Humidity
float temp_out = 0;
byte temp_out_port = TEMP_OUT_PORT;
float hum_out = 0;
byte hum_out_port = 0;
float temp_in = 0;
byte temp_in_port = 0;
float hum_in = 0;
byte hum_in_port = 0;


int ds18b20_temp = -127;




// Voltage
float bord_voltage = 0;
//float bord_voltage_blynk = 0;
byte bord_voltage_port = BORD_VOLTAGE_PORT;

// Dimmer
byte dimmer_port = DIMMER;
float dimmer_V = 0;
//byte mood_port = 2;
byte dimmer_min = 0;
byte dimmer_max = 10;
byte dimmer_active = 5;
unsigned long dimmer_active_timer = 0;



// Fuel
float fuel_V = 0;
float fuel_l = 43.8;
float fuel_ohm = 0;
byte fuel_port = FUEL_GAUGE_PORT;
int fuel_gaugeresistor = FUEL_GAUGERESISTOR;
byte fuel_warning = FUEL_WARNING;

// Water Temperatur
float water_temp_V = 0; // Voltage of port
float water_temp_ohm = 0;
float water_temp = 0;
byte water_temp_port = WATER_TEMP;
byte water_temp_warning = WATER_TEMP_WARNING;

// RPM
float rpm = 0;
byte rpm_port = RPM_PORT;
float rpm_multipl = RPM_MULTIPL;

// Speed
uint8_t speed = 0;
int speedpulse = 0;
byte speed_offset = SPEED_OFFSET;
int speedpulse_tmp[SPEEDPULSEARRAY];
byte speedpulse_port = SPEEDPULSE_PORT;
byte speed_source = SPEEDSOURCE;

// Door
byte door_port = 0;

// OIL
float oil_temp = 0;
byte oil_temp_port = 0;
float oil_pressure = 0;
byte oil_pressure_port = 0;
byte oil_press_warning = OIL_PRESSURE_WARNING;
byte oil_temp_warning = OIL_TEMP_WARNING;

// Alarm
byte alarm = 0;
byte alarm_port = ALARM_PORT;
unsigned long alarm_timer = 0;

unsigned long alarm_on_timer = 0;
unsigned long alarm_off_timer = 0;

unsigned int alarm_on = 0;
unsigned int alarm_pause = 0;
unsigned int alarm_counter = 0;
boolean blink_bg_led = false;



// Watchdog
unsigned long watchdog_timer = 0;
//const boolean wdreset = false;
PROGMEM boolean wdreset = false;

// Aauxiliary Heating
byte aux_heating_port = 0;

// OneWire
bool onewire_enabled = false;
bool onewire_available = false;

//I2C
boolean si7021_available = false;
boolean bmp280_available = false;
boolean lm75_1_available = false;
boolean lm75_2_available = false;
boolean ht16k33_available = false;
boolean ads1115_0_available = false;
boolean ads1115_1_available = false;
boolean ads1115_2_available = false;
boolean ads1115_3_available = false;

int si7021_temp = -127;
int si7021_hum = 0;
double lm75_1_temp = 0;
double lm75_2_temp = 0;

byte i2c_led_disp_clock = 1;

// TinyGSM
byte enable_tinygsm = 1;
byte enable_blynk = 1;
boolean sim_ok = true;
boolean tinygsminit = false;
String sim_pin = SIM_PIN;
String apn = APN;
String apn_user = APN_USER;
String apn_pass = APN_PASS;
String blynk_key = BLYNK_KEY;
String sms_keyword = SMS_Keyword;
boolean online = false;
boolean startmsg = true;
byte online_interval = ONLINE_INTERVALL;

byte blynk_offline_counter = 0;
boolean geo_fence_enabled = false;
boolean geo_fence_armed = false;
boolean geo_fence_alarm = false;
boolean geo_fence_alarm_system = false;
boolean stay_online = false;
int geo_fence_distance = 100;
boolean blynk_geo_fence_alarmed = false;
unsigned long online_intervall_timer = 0;
boolean blynk_alarm = false;

boolean running = false;


// TIME
boolean summer_time = true;
int timezone = 1;
int16_t year;
uint8_t day;
int8_t month;
int8_t hour;
int8_t minute;
int8_t second;


//Statistics
long engine_start = 0;
long engine_running_sec = 0;
long engine_running_total = 0;
long engine_running_total_last = 0;
long engine_running_trip_last = 0;
boolean engine_running = false;




// Port vars
float a0_V = 0;
float a1_V = 0;
float a2_V = 0;
float a3_V = 0;
int a4_int = 0;
int a5_int = 0;
float a4_hz = 0;
float a5_hz = 0;
unsigned long a4_time = 0;
unsigned long a5_time = 0;

float a0_tmp[IO_ARRAY];
float a1_tmp[IO_ARRAY];
float a2_tmp[IO_ARRAY];
float a3_tmp[IO_ARRAY];



/*
 * Serial UI
 */
String inputString = "";         // a String to hold incoming data
boolean tail = false;
unsigned long tail_timer = 0;



 /*
 * Display
 */
int MainMenuPos = 0;
unsigned long MsgTimer = 0;

//byte clock_view = CLOCK_VIEW;
unsigned long display_update_timer = 0;
boolean display_update_timer_lock = false;

unsigned long display_active_timer = 0;

int MenuValuesPos = 0;
int MenuConfigPos = 0;
boolean MenuEdit = false;

boolean display_temp = DISPLAY_TEMP;


/*
 * SD Card configuration
 */
boolean sd_enabled = true;
boolean sd_logging = true;
boolean sd_ok = false;
unsigned long log_timer = 0;
char filename[16];
boolean SDmount = false;
boolean SDerror = false;
int lastfile = 0;
byte lastfile_config = 0;


/*
 * TINYGSM library
 */

boolean tinygsm_enabled = true;
boolean blynk_enabled = true;
boolean sms_enabled = true;
boolean gps_enabled = true;
boolean blynk_report = true;

boolean tinygms_ok = false;
boolean tinygsm_gps_ok = false;

boolean go_online = false;
boolean go_offline = false;
boolean tinygsm_lock = false;

int gsm_signal = 0;

boolean use_gps_speed = USE_GPS_SPEED;

boolean gps_fixstatus;
int gps_fixerrcnt = 0;
int gps_year;
int gps_day;
int gps_month;
int gps_hour;
int gps_minute;
int gps_second;
float gps_speed;
uint8_t gps_speed_max = 0;
uint8_t gps_speed_max_trip = 0;
uint8_t gps_speed_avg = 0;
uint8_t gps_speed_avg_trip = 0;
uint16_t gps_course;
int gps_altitude;
int gps_view_satellites;
int gps_used_satellites;
int gps_altitude_blynk;
int gps_view_satellites_blynk = 255;
int gps_used_satellites_blynk = 255;
float gps_latitude, gps_longitude;
float gps_latitude_old = 0;
float gps_longitude_old = 0;
float gps_latitude_lastlog = 0;
float gps_longitude_lastlog = 0;
float gps_latitude_blynk = 0;
float gps_longitude_blynk = 0;
float gps_latitude_geo_fence = 0;
float gps_longitude_geo_fence = 0;
boolean gps_fix = false;
uint32_t gps_distance = 0;

boolean gprs_tracking = false;


/*
 * Trip counter
 */
unsigned long trip_distance = 0;
unsigned long trip_dist_last = 0;
unsigned long trip_dist_all_last = 0;
unsigned long trip_dist_all = 0;

unsigned long trip_time = 0;
unsigned long trip_time_last = 0;

unsigned long trip_timer = 0;


/*
 *  Input/Output
 */

unsigned long button_timer = 0;
bool button_timer_lock = false;

uint8_t button_1 = 0;
uint8_t button_1_low = 0;
uint8_t button_1_high = 0;
uint8_t button_1_double = 0;


unsigned long update_vars_timer = 0;

/*
 * Status Checker
 */
unsigned long status_checker_timer = 0;
boolean alarm_system_armed = false;
boolean alarm_system_triggered = false;




/*
 * Features
 */
boolean aux_heater_enabled = true;
byte aux_heater_status = false;
unsigned long aux_heater_timer = 0;
byte aux_heating_mode = 0;
byte aux_heating_time = 15;

#define STATE_off 0
#define STATE_on 1
#define STATE_request_on 2
#define STATE_wait_blynk_on 3
#define STATE_request_off 4
#define STATE_wait_blynk_off 5
#define STATE_error 6


struct struckt_value_maps {
  unsigned int port_value;
  int value;
  // size is 32 Bits / 4 Bytes
};

// one map has 32 fields a 4 Bytes = 128 Bytes
struckt_value_maps map_water_temp[32] = {
  {1000, 20},
  {800, 26},
  {700, 30},
  {500, 40},
  {425, 44},
  {350, 50},
  {300, 55},
  {250, 60},
  {212, 65},
  {180, 69},
  {150, 75},
  {125, 80},
  {107, 84},
  {89, 90},
  {75, 95},
  {60, 101},
  {53, 105},
  {45, 110},
  {38, 114},
  {32, 119},
  {26, 125}
};

/*
 * Ports and there values
 */

/*
 * Types:
 * 0 = undefined
 * 1 = Volt
 * 2 = Hz
 * 3 = °C
 * 4 = %
 * 5 = km/h
 * 6 = OneWire
 */
#define TYPE_undef 0
#define TYPE_Volt 1
#define TYPE_Hz 2
#define TYPE_GradCelsius 3
#define TYPE_Prozent 4
#define TYPE_kmh 5
#define TYPE_System 6
#define TYPE_Humidity 7


struct struct_port_values {
  byte port;
  float value;
  byte type;
  boolean output;
  // size is 49 Bits / 6 Bytes + 1 bit
};

struct_port_values port_values[32]; // 32x 6 Bytes = 160 Bytes

/*struct_port_values port_values[32] = {
  {0x00, 0},
  {0x01, 1},
  {0x02, 2},
  {0x03, 3},
  {0x04, 4},
  {0x05, 5},
  {0x06, 6},
  {0x07, 7},
  {0x08, 8},
  {0x09, 9},
  {0x0a, 10},
  {0x0b, 11},
  {0x0c, 12},
  {0x0d, 13},
  {0x0e, 14},
  {0x0f, 15},
  {0x10, 16},
  {0x11, 17},
  {0x12, 18},
  {0x13, 19},
  {0x14, 20},
  {0x15, 21},
  {0x16, 22},
  {0x17, 23},
  {0x18, 24},
  {0x19, 25},
  {0x1a, 26},
  {0x1b, 27},
  {0x1c, 28},
  {0x1d, 29},
  {0x1e, 30},
  {0x1f, 31}
};*/


/*
 * Pointer to the programm variables and to the config variables
 */

#define MAX_PORTS 10
#define MIN_CONFIG 1
#define DEFAULT_STEPS 1 // if "0" -> menue is off
struct struct_ports {
  char name[20];
  char desc[25];
  byte *port;
  byte steps;
  byte max;
  byte min;
  boolean output;
};

const struct_ports ports[] = {
  {"bord_voltage_port", "Bord Voltage Port", &bord_voltage_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"dimmer_port", "Dimmer Port", &dimmer_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"fuel_port", "Fuel Gauge Port", &fuel_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"water_temp_port", "Water Gauge Port", &water_temp_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"rpm_port", "RPM Port", &rpm_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"speedpulse_port", "GALA Port", &speedpulse_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"speed_source", "Speed Source", &speed_source, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"door_port", "Door Port", &door_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"oil_temp_port", "Oil Temp. Port", &oil_temp_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"oil_pressure_port", "Oil Press. Port", &oil_pressure_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"temp_out_port", "Temp. outsise", &temp_out_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"temp_in_port", "Temp. inside", &temp_in_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, false},
  {"alarm_port", "Alarm", &alarm_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, true},
  {"aux_heating_port", "Aux. Heating", &aux_heating_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG, true}
 };

struct struct_config {
  char name[20];
  char desc[25];
  byte *config;
  byte steps;
  byte max;
  byte min;
};

const struct_config config[] = {
  //{"i2c_led_disp_clock", "LED Clock", &i2c_led_disp_clock, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"online_interval", "Online Intervall (min)", &online_interval, 5, 240, 5},
  {"lastfile_config", "Last Log File (x10)", &lastfile_config, 1, 99, 1},
  {"dimmer_max", "Dimmer Max.", &dimmer_max, 5, 255, 0},
  {"dimmer_min", "Dimmer Min.", &dimmer_min, 5, 255, 0},
  {"dimmer_active", "Dimmer Active", &dimmer_active, 10, 250, 0},
//  {"clock_view", "Clock and Temp.", &clock_view, 1, 1, 0},
  {"speed_offset", "Speed Offset", &speed_offset, 1, 25, 0},
  {"water_temp_warning", "Water Temp Warning", &water_temp_warning, 5, 130, 80},
  {"oil_temp_warning", "Oil Temp Warning", &oil_temp_warning, 5, 150, 80},
  {"oil_press_warning", "Oil Press. Warning (/10)", &oil_press_warning, 1, 10, 1},
//  {"i2c_led_disp_clock", "LED Clock", &i2c_led_disp_clock, 1, 10, 1},
//  {"aux_heating_mode", "Aux. Heating Mode", &aux_heating_mode, 1, 2, 0},
  {"aux_heating_time", "Aux. Heating Time", &aux_heating_time, 5, 200, 0}
};

struct struct_values {
  char name[20];
  char desc[20];
  float *value;
  byte digits;
  char suffix[6];
  boolean show; // show this in the display
  boolean show_off; // show this if engine is stopped
  byte *port;
};

const struct_values values[] {
  {"bord_voltage", "Bordspannung", &bord_voltage, 1, "V", true, false, &bord_voltage_port},
  {"fuel_l", "Tank", &fuel_l, 0, "l", true, false, &fuel_port},
  {"water_temp", "Kuehlwasser", &water_temp, 0, "\xb0 C", true, false, &water_temp_port},
  {"rpm", "RPM", &rpm, 0, "U/min", true, false, &rpm_port},
  {"oil_temp", "Oel Temp.", &oil_temp, 0, "\xb0 C", true, false, &oil_temp_port},
  {"oil_pressure", "Oeldruck", &oil_pressure, 1, "bar", true, false, &oil_pressure_port},
  {"temp_out", "Temp. out", &temp_out, 1, "\xb0 C", false, false, &temp_out_port},
  {"hum_out", "Humidity out", &hum_out, 1, "\xb0 C", false, false, &hum_out_port},
  {"temp_in", "Temp. in", &temp_in, 1, "\xb0 C", false, false, &temp_in_port},
  {"hum_in", "Humidity in", &hum_in, 1, "\xb0 C", false, false, &hum_in_port}
};

struct struct_features {
  char name[20];
  char desc[20];
  boolean *feature;
};

const struct_features features[] {
  //{"sd_enabled", "SD Card", &sd_enabled},
  //{"sd_logging", "SD Logging", &sd_logging},
  {"tinygsm_enabled", "TinyGSM enabled", &tinygsm_enabled},
  //{"internet_enabled", "Internet enabled", &internet_enabled},
  {"blynk_enabled", "BLYNK enabled", &blynk_enabled},
  {"sms_enabled", "SMS enabled", &sms_enabled},
  {"gps_enabled", "GPS enabled", &gps_enabled},
  {"blynk_report", "Push reset/boot", &blynk_report},
  {"display_temp", "Display Temperature", &display_temp},
  {"use_gps_speed", "use GPS Speed", &use_gps_speed,},
  {"onewire_enabled", "enable 1Wire Bus", &onewire_enabled}
};


/*
 * saving the configuration at the flash
 */
typedef struct {
  char sim_pin[6];
  char apn_pass[16];
  char apn[36];
  char apn_user[12];
  char blynk_key[36];
  char sms_keyword[36];
} struckt_char_config;

struckt_char_config char_config;
FlashStorage(flash_char_config, struckt_char_config);

FlashStorage(flash_watchdog_reset, boolean);



/*
 * converting a normal time into unix time
 * based on http://christianscode.blogspot.de/2015/07/arduino-timestamp-function.html
 *
 * Function extracted from the library http://playground.arduino.cc/Code/Time
 * if you just need the function without compiling few kbs and you need to save
 * space, this is the way to go
 *
 */

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)

// leap year calulator expects year argument as years offset from 1970
#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
static  const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0
