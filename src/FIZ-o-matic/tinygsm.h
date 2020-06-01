/*
 * TINYGSM library
 */
unsigned long tinygsm_timer = 0;
unsigned long gps_timer = 0;
unsigned long tinygsm_sms_timer = 0;
unsigned long tinygsm_blynk_timer = 0;

boolean tinygsm_enabled = true;
boolean blynk_enabled = true;
boolean sms_enabled = true;
boolean gps_enabled = true;
boolean blynk_report = false;

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
float gps_altitude;
int gps_view_satellites;
int gps_used_satellites;
float gps_altitude_blynk;
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
float accuracy = 0;

boolean gprs_tracking = false;



#ifdef TinyGSM


#else


#endif
