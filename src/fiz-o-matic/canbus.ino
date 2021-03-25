#ifdef CANBUS

#include <SPI.h>
#include <CAN.h>



void can_init() {

  CAN.setPins(CAN_CS, CAN_IRQ);
  // start the CAN bus at 50 kbps
  //DEBUG_PRINTLN(F("Init CAN"));
  message(DEBUG_MSG, F("Init CAN"));
  if (CAN.begin(50E3)) {
    //DEBUG_PRINTLN(F("Starting CAN finished"));
    message(DEBUG_MSG, F("Starting CAN finished"));
    CAN.onReceive(onReceive);
    can_active = true;
  }
  else {
    while(1) {
      //DEBUG_PRINTLN(F("Starting CAN failed"));
      message(DEBUG_MSG, F("Starting CAN failed"));
      delay(2000);
    }
  }

}


void can_loop () {

  // return if timer is not reached
  if ( !timer_check(&canbus_timer, CANBUS_TIMER) ) return;

  can_send();

}

void can_send () {
  // try to parse packet
  packetSize = CAN.parsePacket();

  CAN.beginPacket(0x13);
  CAN.write(gps_fixerrcnt>>8);
  CAN.write(gps_fixerrcnt);
  CAN.endPacket();

  #ifdef CAN_time_MASTER
  //Time
  CAN.beginPacket(0x10);
  CAN.write(rtc.getHours());
  CAN.write(rtc.getMinutes());
  CAN.write(rtc.getSeconds());
  CAN.write(rtc.getDay());
  CAN.write(rtc.getMonth());
  CAN.write(rtc.getYear());
  CAN.endPacket();

  //UnixTime
  CAN.beginPacket(0x11);
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

  #ifdef CANMASTER_gps
  CAN.beginPacket(CANADDR_gps_lat_long);
  gps_latitude_long = long(gps_latitude*100000);
  can_buff[CANBYTE_gps_latitude] = gps_latitude_long >> 24;
  can_buff[CANBYTE_gps_latitude+1] = gps_latitude_long >> 16;
  can_buff[CANBYTE_gps_latitude+2] = gps_latitude_long >> 8;
  can_buff[CANBYTE_gps_latitude+3] = gps_latitude_long;

  gps_longitude_long = long(gps_longitude*100000);
  can_buff[CANBYTE_gps_longitude] = gps_longitude_long >> 24;
  can_buff[CANBYTE_gps_longitude+1] = gps_longitude_long >> 16;
  can_buff[CANBYTE_gps_longitude+2] = gps_longitude_long >> 8;
  can_buff[CANBYTE_gps_longitude+3] = gps_longitude_long;

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

  message(DEBUG_MSG,F("CAN send done"));
  message(DEBUG_MSG,String(gps_fixerrcnt, DEC));
  message(DEBUG_MSG,String(gps_latitude_long, DEC));
  message(DEBUG_MSG,String(gps_longitude_long, DEC));
  //message(DEBUG_MSG,String(timebuffer[2], DEC));
  //message(DEBUG_MSG,String(timebuffer[3], DEC));
  message(DEBUG_MSG,String(time_tmp, DEC));


}

void onReceive(int packetSize) {

}

#else

void can_init() {}
void can_loop () {}


#endif
