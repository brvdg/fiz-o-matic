#ifdef CANBUS
#include <CAN.h>
#include "can_addr.h"


void can_receive(int packetSize) {
  String msg;
  can_id = CAN.packetId();

  // received a packet
  //CANDBG_PRINT(F("Received "));

  if (CAN.packetExtended()) {
    //CANDBG_PRINT(F("extended "));
  }

  if (CAN.packetRtr()) {
    // Remote transmission request, packet contains no data
    //CANDBG_PRINT(F("RTR "));
  }

  //CANDBG_PRINT(F("0x"));
  msg = F("0x");
  msg += String(can_id, HEX);
  msg += F(": ");
  //CANDBG_PRINT(String(can_id, HEX));
  //CANDBG_PRINT(F(": "));

  if (CAN.packetRtr()) {
    //Serial.print(" and requested length ");
    //CANDBG_PRINT(CAN.packetDlc());
  } else {
    // only print packet data for non-RTR packets
    /*CANDBG_PRINT(F(" and length "));
    CANDBG_PRINT(String(packetSize));
    CANDBG_PRINTLN(F("; "));*/


    /*
    if ( can_id == 0x13 ) {
      while (CAN.available()) {
        field = CAN.read();
        if ( field_cnt == 0 ) {
          value = field;
        }
        else if ( field == 1 ) {
          value = value + (field<<8);
        }
        Serial.println(field, DEC);

        Serial.println(value, DEC);

        field_cnt++;
      }

    }
    else {
      field_cnt = 0;
      while (CAN.available()) {
        //Serial.print((char)CAN.read());
        can_buff[field_cnt] = CAN.read();
        field_cnt++;
      }
      Serial.println();
    }*/

    field_cnt = 0;
    for ( byte i=0; i>=7; i++ ) {
      can_buff[i] = 0;
    }
    while (CAN.available()) {
      //Serial.print((char)CAN.read());
      can_buff[field_cnt] = CAN.read();
      //CANDBG_PRINT(String(can_buff[field_cnt], DEC));
      //CANDBG_PRINT(F(","));
      msg += String(can_buff[field_cnt], DEC);
      msg += F(",");
      field_cnt++;
    }

    //CANDBG_PRINTLN();

    switch (can_id) {
      #ifdef CAN_time_SLAVE
      case CANADDR_time:
        hour = can_buff[0];
        minute = can_buff[1];
        second = can_buff[2];
        day = can_buff[3];
        month = can_buff[4];
        year = can_buff[5];
        /*CANDBG_PRINTLN(F("---"));
        CANDBG_PRINTLN(String(hour, DEC));
        CANDBG_PRINTLN(String(minute, DEC));
        CANDBG_PRINTLN(String(second, DEC));*/
        break;
      case CANADDR_unix_time:
        unix_time = can_buff[0];
        unix_time = (unix_time<<8) + can_buff[1];
        unix_time = (unix_time<<8) + can_buff[2];
        unix_time = (unix_time<<8) + can_buff[3];
        //CANDBG_PRINT(F("--- Unix Time: "));
        //CANDBG_PRINT(String(unix_time, DEC));
        break;
      #endif

      #ifdef CAN_voltage_SLAVE
      case CANADDR_voltage:
        bord_voltage = float(can_buff[0]) / 10;
        //CANDBG_PRINT(String(bord_voltage, DEC));
        if ( bord_voltage != 0 ); bord_voltage_active = true;
        batt1_voltage = float(can_buff[1]) / 10;
        if ( batt1_voltage != 0 ); batt1_voltage_active = true;
        batt2_voltage = float(can_buff[2]) / 10;
        if ( batt2_voltage != 0 ); batt2_voltage_active = true;
        break;
      #endif

      #ifdef CAN_gps_SLAVE
      case CANADDR_gps_lat_long:
        gps_latitude_long = can_buff[0];
        gps_latitude_long = (gps_latitude_long<<8) + can_buff[1];
        gps_latitude_long = (gps_latitude_long<<8) + can_buff[2];
        gps_latitude_long = (gps_latitude_long<<8) + can_buff[3];
        gps_latitude = float(gps_latitude_long)/100000;

        gps_longitude_long = can_buff[4];
        gps_longitude_long = (gps_longitude_long<<8) + can_buff[5];
        gps_longitude_long = (gps_longitude_long<<8) + can_buff[6];
        gps_longitude_long = (gps_longitude_long<<8) + can_buff[7];
        gps_longitude = float(gps_longitude_long)/100000;

        /*CANDBG_PRINTLN(F("--- gps_latitude: "));
        CANDBG_PRINTLN(String(gps_latitude, DEC));
        CANDBG_PRINTLN(F(" --- gps_longitude: "));
        CANDBG_PRINTLN(String(gps_longitude, DEC));*/
        break;

      case CANADDR_gps_2:
        gps_speed = can_buff[0];
        gps_speed = (gps_speed<<8) + can_buff[1];
        gps_altitude = can_buff[2];
        gps_altitude = (gps_altitude<<8) + can_buff[3];
        gps_view_satellites = can_buff[4];
        gps_used_satellites = can_buff[5];
        //CANDBG_PRINT(F("--- gps_speed: "));
        //CANDBG_PRINT(String(gps_speed, DEC));
        //CANDBG_PRINT(F(" --- gps_view_satellites: "));
        //CANDBG_PRINT(String(gps_view_satellites, DEC));
        speed = gps_speed;
        //altitude = gps_altitude;
        break;
      #endif

      #ifdef CAN_fuel_water_SLAVE
      case CANADDR_fuel_water:
        fuel_l = can_buff[0];
        if ( fuel_l != 0 ); fuel_l_active = true;
        water_temp = can_buff[1];
        if ( water_temp != 0 ); water_temp_active = true;
        break;
      #endif

      #ifdef CAN_oil_SLAVE
      case CANADDR_oil:
        oil_temp = can_buff[0];
        if ( oil_temp != 0 ); oil_temp_active = true;
        oil_pressure = can_buff[1];
        if ( oil_pressure != 0 ); oil_pressure_active = true;
        break;
      #endif

      #ifdef CAN_klima_SLAVE
      case CANADDR_klima:
        temp_out = can_buff[0];
        temp_out = (temp_out<<8) + can_buff[1];
        if ( temp_out != 0 ); temp_out_active = true;
        temp_in = can_buff[2];
        temp_in = (temp_in<<8) + can_buff[3];
        if ( temp_in != 0 ); temp_in_active = true;
        hum_out = can_buff[4];
        if ( hum_out != 0 ); hum_out_active = true;
        hum_in = can_buff[5];
        if ( hum_in != 0 ); hum_in_active = true;
        break;
      #endif

      #ifdef CAN_rpm_SLAVE
      case CANADDR_rpm:
        rpm = can_buff[0];
        rpm = (rpm<<8) + can_buff[1];
        if ( rpm != 0 ) {
          rpm_active = true;
        }
        else {
          rpm_active = false;
        }
        //CANDBG_PRINT(F("--- rpm: "));
        //CANDBG_PRINT(String(rpm, DEC));
        break;
      #endif

      #ifdef CAN_trip_SLAVE
      case CANADDR_trip:
        trip1 = can_buff[0];
        trip1 = (rpm<<8) + can_buff[1];
        trip2 = can_buff[2];
        trip2 = (rpm<<8) + can_buff[3];
        if ( trip1 != 0 ) {
          trip1_active = true;
        }
        else {
          trip1_active = false;
        }
        break;
      #endif


    }

    //CANDBG_PRINTLN(F("---"));
    message(CAN_MSG, msg);
  }
}
#endif //CANBUS
