/****************************************************
 * fiz-o-matic
 * https://fiz-o-matic.net/
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/


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


WidgetTerminal terminal(BLYNK_VIRTUAL_terminal);
WidgetMap myMap(BLYNK_VIRTUAL_map);
WidgetLED geo_fence_armed_led(BLYNK_VIRTUAL_geo_fence_armed_led);
WidgetLED geo_fence_led(BLYNK_VIRTUAL_geo_fence_led);
WidgetLED online_LED(BLYNK_VIRTUAL_online_LED);
WidgetLED armed_led(BLYNK_VIRTUAL_armed_led);
WidgetLED alarm_led(BLYNK_VIRTUAL_alarm_led);
