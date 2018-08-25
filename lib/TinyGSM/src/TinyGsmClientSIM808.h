/**
 * @file     TinyGsmClientSIM808.h
 * @author   Volodymyr Shymanskyy
 * @license  LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date     Nov 2016
 */

#ifndef TinyGsmClientSIM808_h
#define TinyGsmClientSIM808_h

#include <TinyGsmClientSIM800.h>


//============================================================================//
//============================================================================//
//              Declaration and Definitio of the TinyGsmSim808 Class
//============================================================================//
//============================================================================//

class TinyGsmSim808: public TinyGsmSim800
{

public:

  TinyGsmSim808(Stream& stream)
    : TinyGsmSim800(stream)
  {}

  /*
   * GPS location functions
   */

  // enable GPS
  bool enableGPS() {
    uint16_t state;

    sendAT(GF("+CGNSPWR=1"));
    if (waitResponse() != 1) {
      return false;
    }

    return true;
  }

  bool disableGPS() {
    uint16_t state;

    sendAT(GF("+CGNSPWR=0"));
    if (waitResponse() != 1) {
      return false;
    }

    return true;
  }

  // get the RAW GPS output
  // works only with ans SIM808 V2
  String getGPSraw() {
    sendAT(GF("+CGNSINF"));
    if (waitResponse(GF(GSM_NL "+CGNSINF:")) != 1) {
      return "";
    }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  // get GPS informations
  // works only with ans SIM808 V2
  bool getGPS(float *lat, float *lon, float *speed=0, int *alt=0, int *vsat=0, int *usat=0) {
    //String buffer = "";
    char chr_buffer[12];
    bool fix = false;

    sendAT(GF("+CGNSINF"));
    if (waitResponse(GF(GSM_NL "+CGNSINF:")) != 1) {
      fix = false;
    }

    stream.readStringUntil(','); // mode
    if ( stream.readStringUntil(',').toInt() == 1 ) fix = true;
    stream.readStringUntil(','); //utctime
    *lat =  stream.readStringUntil(',').toFloat(); //lat
    *lon =  stream.readStringUntil(',').toFloat(); //lon
    if (alt != NULL) *alt =  stream.readStringUntil(',').toFloat(); //lon
    if (speed != NULL) *speed = stream.readStringUntil(',').toFloat(); //speed
    stream.readStringUntil(',');
    stream.readStringUntil(',');
    stream.readStringUntil(',');
    stream.readStringUntil(',');
    stream.readStringUntil(',');
    stream.readStringUntil(',');
    stream.readStringUntil(',');
    if (vsat != NULL) *vsat = stream.readStringUntil(',').toInt(); //viewed satelites
    if (usat != NULL) *usat = stream.readStringUntil(',').toInt(); //used satelites
    stream.readStringUntil('\n');

    waitResponse();

    return fix;
  }

  // get GPS time
  // works only with SIM808 V2
  bool getGPSTime(int *year, int *month, int *day, int *hour, int *minute, int *second) {
    bool fix = false;
    char chr_buffer[12];
    sendAT(GF("+CGNSINF"));
    if (waitResponse(GF(GSM_NL "+CGNSINF:")) != 1) {
      return false;
    }

    for (int i = 0; i < 3; i++) {
      String buffer = stream.readStringUntil(',');
      buffer.toCharArray(chr_buffer, sizeof(chr_buffer));
      switch (i) {
        case 0:
          //mode
          break;
        case 1:
          //fixstatus
          if ( buffer.toInt() == 1 ) {
            fix = buffer.toInt();
          }
          break;
        case 2:
          *year = buffer.substring(0,4).toInt();
          *month = buffer.substring(4,6).toInt();
          *day = buffer.substring(6,8).toInt();
          *hour = buffer.substring(8,10).toInt();
          *minute = buffer.substring(10,12).toInt();
          *second = buffer.substring(12,14).toInt();
          break;

        default:
          // if nothing else matches, do the default
          // default is optional
          break;
      }
    }
    String res = stream.readStringUntil('\n');
    waitResponse();

    if (fix) {
      return true;
    } else {
      return false;
    }
  }

  int getNumSMS() {

      sendAT(GF("+CMGF=1"));
      if (waitResponse() != 1) {
          return 0;
      }

      sendAT(GF("+CPMS?"));
      if (waitResponse(GF(GSM_NL "+CPMS:")) != 1) {
          return 0;
      }
      String res = stream.readStringUntil('\n');
      int index1 = res.indexOf(",");
      int index2 = res.indexOf(",", index1+1);
      String tmp = res.substring(index1+1,index2);
      waitResponse();
      //res.trim();
      tmp.trim();
      //return res;
      //int num = tmp.toInt();
      return tmp.toInt();
  }

  String readSMS(int num, char *sender) {
      String buffer;

      sendAT(GF("+CMGF=1"));
      if (waitResponse() != 1) {
          return "";
      }
      sendAT(GF("+CSDH=1"));
      if (waitResponse() != 1) {
          return "";
      }


      sendAT(GF("+CMGR="), num, GF(""));
      if (waitResponse(GF(GSM_NL "+CMGR:")) != 1) {
          return "";
      }

      stream.readStringUntil(',');
      buffer = stream.readStringUntil(',');
      buffer.replace('"', ' ');
      buffer.trim();
      buffer.toCharArray(sender, 18);

      stream.readStringUntil('\r');

      buffer = "";

      delay(20); // Wait a moment to get data into the buffer
      while (stream.available()) {
          char c = stream.read();
          buffer = buffer + c;
      }

      String res = buffer;
      waitResponse();
      res.trim();
      return res;
  }

  boolean deleteSMS(int num) {
      sendAT(GF("+CMGD="), num, GF(""));
      if (waitResponse(GF(GSM_NL "+CMGD:")) != 1) {
          return false;
      }
      return true;
  }

};

#endif
