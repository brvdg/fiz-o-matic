/***************************************************
 *  This sketch is for I2C Bus functions.
 *
 *  Author: Brun
 *
 ****************************************************/

#ifdef I2C
#include <Wire.h>

//#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
char displaybuffer[4] = {' ', ' ', ' ', ' '};

const uint16_t fonttable[] =  {
  0b0000110000111111, // 0
  0b0000000000000110, // 1
  0b0000000011011011, // 2
  0b0000000010001111, // 3
  0b0000000011100110, // 4
  0b0010000001101001, // 5
  0b0000000011111101, // 6
  0b0000000000000111, // 7
  0b0000000011111111, // 8
  0b0000000011101111 // 9
};


unsigned long i2c_timer = 0;

//#ifdef SI7021
double si7021_temperature;
double si7021_humidity;
//#endif // SI7021


//const int ADDR =0x40;
//int X0,X1,Y0,Y1,Y2,Y3;
//double X,Y,X_out,Y_out1,Y_out2;


void i2c_init() {
  //TRACE_PRINTLN(F("#i2c_init"));
  notify(BOOTMSG, F("#Init I2C"));


  byte error, address;
  int nDevices;

  //TRACE_PRINTLN(F("#Scanning..."));

  Wire.begin();
  delay(100);

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    //TRACE_PRINTLN(address);
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      message(INFO_MSG ,F("#I2C device found at address 0x"));
      if (address<16) {
        message(INFO_MSG ,"0");
      }
      message(INFO_MSG ,String(address, HEX));
      message(INFO_MSG ,F("  !\n"));

      switch(address) {
        case 64:
        //case 76:
          si7021_available = true;
          //INFO_PRINTLN(F("#SI7021 found!"));
          notify(BOOTMSG, F("SI7021 found"));
          //delay(2000);
          break;
        case 0x70:
          ht16k33_available = true;
          message(INFO_MSG ,F("#HT16K33 found!\n"));
          notify(BOOTMSG, F("HT16K33 (#1) found"));

          alpha4.begin(0x70);  // pass in the address
          alpha4.writeDigitRaw(3, 0x0);
          alpha4.writeDigitRaw(0, 0xFFFF);
          alpha4.writeDisplay();
          delay(200);
          alpha4.writeDigitRaw(0, 0x0);
          alpha4.writeDigitRaw(1, 0xFFFF);
          alpha4.writeDisplay();
          delay(200);
          alpha4.writeDigitRaw(1, 0x0);
          alpha4.writeDigitRaw(2, 0xFFFF);
          alpha4.writeDisplay();
          delay(200);
          alpha4.writeDigitRaw(2, 0x0);
          alpha4.writeDigitRaw(3, 0xFFFF);
          alpha4.writeDisplay();
          delay(200);

          alpha4.clear();
          alpha4.writeDisplay();


          //delay(2000);
          break;
        case 0x48:
          //si7021_available = true;
          //INFO_PRINTLN(F("#ADS1115 found!"));
          notify(BOOTMSG, F("ADS1115 found"));
          //delay(2000);
          break;
        case 0x76:
          bmp280_available = true;
          //INFO_PRINTLN(F("#BMP280 found!"));
          notify(BOOTMSG, F("BMP280 found"));
          //delay(2000);
          break;
        case 0x49:
          lm75_1_available = true;
          //INFO_PRINTLN(F("#LM75 (#1) found!"));
          notify(BOOTMSG, F("LM75 (#1) found"));
          reg_port(0x60, TYPE_GradCelsius);
          //delay(2000);
          break;
        case 0x4A:
          lm75_2_available = true;
          //INFO_PRINTLN(F("#LM75 (#2) found!"));
          notify(BOOTMSG, F("LM75 (#2) found"));
          reg_port(0x61, TYPE_GradCelsius);
          //delay(2000);
          break;
        case 0x50:
          //case 76:
          //si7021_available = true;
          //INFO_PRINTLN(F("#SI7021 found!"));
          notify(BOOTMSG, F("EEPROM found"));
          //delay(2000);
          break;
      }


      nDevices++;
    }
    else if (error==4)
    {
      message(INFO_MSG ,F("#Unknow error at address 0x"));
      if (address<16) {
        message(INFO_MSG ,"0");
      }
      message(INFO_MSG ,String(address, HEX));
      message(INFO_MSG ,F("  !\n"));
    }
  }

  /*if (nDevices == 0) {
    DEBUG_PRINT(F("#No I2C devices found\n"));
  }
  else {
    DEBUG_PRINT(F("done\n"));
  }*/




  //#ifdef SI7021
  /*if ( si7021_available ) {
    Wire.beginTransmission(0x40);
    delay(200);

    if ( temp_out_port == 0 ) {
      temp_out_port = 2;
    }
  }
  else {
    if ( temp_out_port == 2 ) {
      temp_out_port = 0;
    }
  }*/
  //#endif

  //Wire.begin();
  delay(100);

  Wire.endTransmission();
}

void i2c_loop() {
  if ( i2c_timer < millis() ) {
    // put your code here

    if ( !I2C_lock ) {
      I2C_lock = true;

      //#ifdef SI7021
      if (si7021_available) i2c_get_si7021();
      //#endif // SI7021

      if (lm75_1_available) i2c_get_lm75(1);
      if (lm75_2_available) i2c_get_lm75(2);

      if (ht16k33_available) i2c_ht16k33();

      I2C_lock = false;
    }

    i2c_timer = millis() + I2C_TIMER;
  }
}


//#ifdef SI7021
void i2c_get_si7021() {
  //TRACE_PRINT(F("#i2c_get_si7021"));

  Wire.beginTransmission(0x40);
  Wire.write(0xE3);
  Wire.endTransmission();


  //read temperature
  Wire.requestFrom(0x40,2);
  if(Wire.available()<=2);
  {
    int t0 = Wire.read();
    int t1 = Wire.read();
    t0 = t0<<8;
    si7021_temperature = ((175.72*(t0+t1))/65536)-46.85;
  }

  //TRACE_PRINT(F("#si7021_temperature: "));
  //TRACE_PRINT(si7021_temperature);

  si7021_temp = si7021_temperature*10;

  // read humidity
  Wire.beginTransmission(0x40);
  Wire.write(0xE5);
  Wire.endTransmission();

  Wire.requestFrom(0x40,2);
  if(Wire.available()<=2);
  {
    int h0 = Wire.read();
    int h2=h0/100;
    h0=h0%100;
    int h1 = Wire.read();
    double hd1 = h2*25600;
    double hd2 = h0*256+h1;
    hd1 = (125*hd1)/65536;
    hd2 = (125*hd2)/65536;
    //Y = hd1+hd2;
    si7021_humidity = hd1+hd2-6;
  }

  //TRACE_PRINT(F(", si7021_humidity: "));
  //TRACE_PRINTLN(si7021_humidity);

  si7021_hum = si7021_humidity;


}
//#endif // SI7021

// LM75 Temperatur auslesen. Device = 0-7, regx = TEMP, OBEN, UNTEN (Registerauswahl)
// http://www.horter.de/i2c/i2c-beispiele/arduino_1.html

void i2c_get_lm75(int device)
{
  byte msb;
  byte lsb;
  byte msb1;
  byte regx = 0;
  double value = 0;
  Wire.beginTransmission(0x48 + device);
  Wire.write(regx);
  Wire.endTransmission();
  Wire.beginTransmission(0x48 + device);
  Wire.requestFrom(0x48 + device, 2);
  if (Wire.available()) {
     msb1 = Wire.read();
     msb = msb1 << 1; // Vorzeichenbit entfernen, verbliebener Wert ist nun doppelt so groß
     lsb = Wire.read();
  }
  // höchstes bit von lsb sagt aus, ob 0,5 Grad dazu addiert werden sollen
  lsb = (lsb & 0x80 ) >> 7; // nun ist lsb = 0 oder 1
  Wire.endTransmission();
  if (msb1 < 0x80) { // Positiver Wert?
    value = float (msb + lsb)/2; // positiver Wert
  }
  else {
    value = float (msb + lsb)/2 - 128; // negativer Wert
  }

  switch(device){
    case 1: lm75_1_temp = value; break;
    case 2: lm75_2_temp = value; break;
    default: lm75_1_temp = value; break;
  }

  switch(device){
    case 1: update_port_value(0x60, value); break;
    case 2: update_port_value(0x61, value); break;
    default: update_port_value(0x60, value); break;
  }
  //Serial.println(value);
}

void i2c_ht16k33() {
  //Serial.print(".");

  hour = rtc.getHours();
  minute = rtc.getMinutes();
  int tmp_h = (hour / 10) * 10;
  int tmp_m = (minute / 10) * 10;

  alpha4.writeDigitRaw(0, fonttable[tmp_h/10]);
  alpha4.writeDigitRaw(1, fonttable[hour - tmp_h]);
  alpha4.writeDigitRaw(2, fonttable[tmp_m/10]);
  alpha4.writeDigitRaw(3, fonttable[minute - tmp_m]);
  alpha4.writeDisplay();

  /*alpha4.writeDigitRaw(0, 0b0000000000000110);
  alpha4.writeDigitRaw(1, 0b0000100010001011);
  alpha4.writeDigitRaw(2, 0b0000000010001111);
  alpha4.writeDigitRaw(3, 0b0000000000000110);
  alpha4.writeDisplay();*/
}


#endif // I2C
