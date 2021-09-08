#ifdef CANBUS


//CAN
boolean can_active = false;
int packetSize = 0;
unsigned long canbus_timer = 0;
byte can_buff[8];


unsigned long value = 0;
uint8_t field = 0;
uint8_t field_cnt = 0;
int can_id = 0;


// TIME
//#define CAN_time_SLAVE
#if defined CAN_time_SLAVE || defined CAN_time_MASTER
#define CANADDR_time 0x10
/*byte hour;
byte minute;
byte second;
byte day;
byte month;
byte year;*/

#define CANADDR_unix_time 0x11
//unsigned long unix_time = 0;
#endif


//#define CAN_gps_SLAVE
#if defined CAN_gps_SLAVE || defined CAN_gps_MASTER
#define CANADDR_gps_lat_long 0xA2
float gps_latitude = 0;
long gps_latitude_long = 0;
float gps_longitude = 0;
long gps_longitude_long = 0;

#define CANADDR_gps_2 0xA3
int gps_speed;
short gps_altitude;
int gps_view_satellites;
int gps_used_satellites;
boolean gps_fix = false;
#endif


//#define CAN_voltage_SLAVE
#if defined CAN_voltage_SLAVE || defined CAN_voltage_Master
#define CANADDR_voltage 0xB1
float bord_voltage = 14.2;
boolean bord_voltage_active = false;

float batt1_voltage = 13.8;
boolean batt1_voltage_active = false;

float batt2_voltage = 12.2;
boolean batt2_voltage_active = false;
#endif

//#define CAN_rpm_SLAVE
#if defined CAN_rpm_SLAVE || defined CAN_rpm_Master
#define CANADDR_rpm 0x13
//int rpm = 3480;
boolean rpm_active = true;
#endif


//#define CAN_fuel_water_SLAVE
#if defined CAN_fuel_water_SLAVE || defined CAN_fuel_water_MASTER
#define CANADDR_fuel_water 0xB2

//int fuel_l = 48;
boolean fuel_l_active = false;

//float water_temp = 86.2;
boolean water_temp_active = false;
#endif

//#define CAN_oil_SLAVE
#if defined CAN_oil_SLAVE || defined CAN_oil_MASTER
#define CANADDR_oil 0xB5
//int oil_temp = 112;
boolean oil_temp_active = false;

//float oil_pressure = 2.6;
boolean oil_pressure_active = false;
#endif


// Temperature / Humidity
//#define CAN_klima_SLAVE
#if defined CAN_klima_SLAVE || defined CAN_klima_MASTER
//#define CANADDR_klima 0xB6
//int temp_out = 0;
boolean temp_out_active = false;
//int temp_in = 0;
boolean temp_in_active = false;
//byte hum_out = 0;
boolean hum_out_active = false;
//byte hum_in = 0;
boolean hum_in_active = false;
#endif

#if defined CAN_trip_SLAVE || defined CAN_trip_MASTER
unsigned int trip1 = 112;
boolean trip1_active = false;

unsigned int trip2 = 26;
//boolean trip_day_active = false;
#endif


#endif //CANBUS
