#ifdef CANBUS
#include <CAN.h>
#include "can_addr.h"

void can_send () {
  // try to parse packet
  packetSize = CAN.parsePacket();

  /*CAN.beginPacket(0x13);
  CAN.write(gps_fixerrcnt>>8);
  CAN.write(gps_fixerrcnt);
  CAN.endPacket();*/
  //CANDBG_PRINTLN(F("--- can_send "));

  #ifdef CAN_time_MASTER
  //Time
  CAN.beginPacket(CANADDR_time);
  CAN.write(rtc.getHours());
  CAN.write(rtc.getMinutes());
  CAN.write(rtc.getSeconds());
  CAN.write(rtc.getDay());
  CAN.write(rtc.getMonth());
  CAN.write(rtc.getYear());
  CAN.endPacket();

  //UnixTime
  CAN.beginPacket(CANADDR_unixtime);
  unsigned long time_tmp = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear() + 2000);
  byte timebuffer[4];
  timebuffer[0] = time_tmp >> 24;
  timebuffer[1] = time_tmp >> 16;
  timebuffer[2] = time_tmp >> 8;
  timebuffer[3] = time_tmp;
  /*CAN.write(time_tmp >> 24);
  CAN.write(time_tmp >> 16);
  CAN.write(time_tmp >> 8);
  CAN.write(time_tmp);*/
  CAN.write(timebuffer, 4);
  CAN.endPacket();
  #endif

  #ifdef CAN_gps_MASTER
  CAN.beginPacket(CANADDR_gps_lat_long);
  gps_latitude_long = long(gps_latitude*100000);
  can_buff[0] = gps_latitude_long >> 24;
  can_buff[1] = gps_latitude_long >> 16;
  can_buff[2] = gps_latitude_long >> 8;
  can_buff[3] = gps_latitude_long;

  gps_longitude_long = long(gps_longitude*100000);
  can_buff[4] = gps_longitude_long >> 24;
  can_buff[5] = gps_longitude_long >> 16;
  can_buff[6] = gps_longitude_long >> 8;
  can_buff[7] = gps_longitude_long;

  CAN.write(can_buff, 8);
  CAN.endPacket();
  //delay(25);
  CAN.beginPacket(CANADDR_gps_2);
  can_buff[0] = gps_speed >> 8;
  can_buff[1] = gps_speed;
  can_buff[2] = gps_altitude >> 8;
  can_buff[3] = gps_altitude;
  can_buff[4] = gps_view_satellites;
  can_buff[5] = gps_used_satellites;
  CAN.write(can_buff, 6);
  CAN.endPacket();
  #endif

  #ifdef CAN_voltage_Master
  CAN.beginPacket(CANADDR_voltage);
  can_buff[0] = bord_voltage*10;
  can_buff[1] = batt1_voltage*10;
  can_buff[2] = batt2_voltage*10;

  CAN.write(can_buff, 3);
  CAN.endPacket();
  #endif

  #ifdef CAN_fuel_water_MASTER
  CAN.beginPacket(CANADDR_fuel_water);
  can_buff[0] = fuel_l;
  can_buff[1] = water_temp;

  CAN.write(can_buff, 2);
  CAN.endPacket();
  #endif

  #ifdef CAN_oil_MASTER
  CAN.beginPacket(CANADDR_oil);
  can_buff[0] = oil_temp;
  can_buff[1] = oil_pressure;

  CAN.write(can_buff, 2);
  CAN.endPacket();
  #endif

  #ifdef CAN_trip_MASTER
  CAN.beginPacket(CANADDR_trip);
  can_buff[0] = trip1 >> 8;
  can_buff[1] = trip1;

  can_buff[2] = trip2 >> 8;
  can_buff[3] = trip2;

  CAN.write(can_buff, 4);
  CAN.endPacket();
  #endif



  #ifdef CAN_klima_MASTER
  CAN.beginPacket(CANADDR_klima);
  can_buff[0] = int(temp_out) >> 8;
  can_buff[1] = int(temp_out);
  can_buff[2] = int(temp_in) >> 8;
  can_buff[3] = int(temp_in);
  can_buff[4] = hum_out;
  can_buff[5] = hum_in;

  CAN.write(can_buff, 6);
  CAN.endPacket();
  #endif





  #ifdef CAN_display_MASTER
  CAN.beginPacket(CANADDR_display);
  can_buff[0] = trip1 >> 8;
  can_buff[1] = trip1;
  can_buff[2] = trip2 >> 8;
  can_buff[3] = trip2;

  CAN.write(can_buff, 2);
  CAN.endPacket();
  #endif

  /*message(DEBUG_MSG,F("CAN send done"));
  message(DEBUG_MSG,String(gps_fixerrcnt, DEC));
  message(DEBUG_MSG,String(gps_latitude_long, DEC));
  message(DEBUG_MSG,String(gps_longitude_long, DEC));
  //message(DEBUG_MSG,String(timebuffer[2], DEC));
  //message(DEBUG_MSG,String(timebuffer[3], DEC));
  message(DEBUG_MSG,String(time_tmp, DEC));*/


}

#endif //CANBUS
