/****************************************************
 * FIZ-o-matic
 * https://fiz-o-matic.net/
 *
 *  https://learn.adafruit.com/adafruit-feather-m0-express-designed-for-circuit-python-circuitpython/using-spi-flash
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/


#ifdef SPIFLASH
//#include "spiflash.h"
 /*
 * Print size, modify date/time, and name for all files in root.
 */
#include <SPI.h>
#include "SdFat.h"
#include "Adafruit_SPIFlash.h"

#if defined(__SAMD51__) || defined(NRF52840_XXAA)
  Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
#else
  #if (SPI_INTERFACES_COUNT == 1 || defined(ADAFRUIT_CIRCUITPLAYGROUND_M0))
    Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
  #else
    Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
  #endif
#endif

Adafruit_SPIFlash flash(&flashTransport);

// file system object from SdFat
FatFileSystem fatfs;

//File myFile;
File spiflash_config;


//------------------------------------------------------------------------------
void init_spiflash() {
  message(STORAGE, F("Initializing Filesystem on external flash..."));

  // Init external flash
  //flash.begin();
  // Initialize flash library and check its chip ID.
  if (!flash.begin()) {
    message(STORAGE, F("Error, failed to initialize flash chip!"));
    while(1);
  }
  message(STORAGE, "Flash chip JEDEC ID: 0x" + String(flash.getJEDECID(), HEX));
  //message(STORAGE, String(flash.getJEDECID(), HEX));
  //message(STORAGE, F("\n"));
  // Init file system on the flash
  //fatfs.begin(&flash);

  // First call begin to mount the filesystem.  Check that it returns true
  // to make sure the filesystem was mounted.
  if (!fatfs.begin(&flash)) {
    message(STORAGE, F("Failed to mount filesystem!"));
    message(STORAGE, F("Was CircuitPython loaded on the board first to create the filesystem?"));
    while(1);
  }
  message(STORAGE, F("Filesystem mounted!"));



  //message(STORAGE, F("Initialization done."));
}

void spiflash_open_config() {
  String tmp;

  if (fatfs.exists("/config.txt")) {
    File config_read = fatfs.open("/config.txt", FILE_READ);

    message(STORAGE, F("config.txt:"));

    // read from the file until there's nothing else in it:
    while (config_read.available()) {
      //Serial.write(config_read.read());
      tmp = config_read.readStringUntil('\r');
      tmp.trim();
      if ( !tmp.startsWith("#") ) {
        message(STORAGE, tmp);
        parse_config_string(tmp);
      }
    }
    // close the file:
    config_read.close();
  } else {
    // if the file didn't open, print an error:
    message(STORAGE, F("/config.txt doesn't exist"));
  }
}

void spiflash_save_config() {
  message(STORAGE, F("#Writing config to SPI FLASH"));

  if ( !SPI_lock ) {
    SPI_lock = true;

    if (fatfs.exists("/config.txt")) {
      if ( !fatfs.remove("/config.txt")) {
        message(STORAGE, F("can't remove /config.txt"));
      }
      else {
        message(STORAGE, F("deleted /config.txt"));
      }
    }

    File config_write = fatfs.open("/config.txt", FILE_WRITE);

    if (config_write) {
      config_write.println(F("#This is the configuration file"));


      // TinyGSM
      config_write.println(F("#SIM PIN"));
      config_write.print(F("sim_pin="));
      config_write.println(sim_pin);
      config_write.println(F("#Provider APN"));
      config_write.print(F("apn="));
      config_write.println(apn);
      config_write.println(F("#username for the APN"));
      config_write.print(F("apn_user="));
      config_write.println(apn_user);
      config_write.println(F("#password for the APN"));
      config_write.print(F("apn_pass="));
      config_write.println(apn_pass);
      config_write.println(F("#BLYNK authentication key"));
      config_write.print(F("blynk_key="));
      config_write.println(blynk_key);
      config_write.println(F("#BLYNK Server (optional)"));
      config_write.print(F("blynk_server="));
      config_write.println(blynk_server);
      config_write.println(F("#SMS Keyword for authentication"));
      config_write.print(F("sms_keyword="));
      config_write.println(sms_keyword);
      config_write.println(F("#My Mobile Number for Notifications"));
      config_write.print(F("my_number="));
      config_write.println(my_number);




      for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
        config_write.print(F("# "));
        config_write.println(config[i].desc);
        config_write.print(config[i].name);
        config_write.print(F("="));
        config_write.println(*config[i].config, DEC);
      }

      for (int i = 0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++){
        config_write.print(F("# "));
        config_write.println(ports[i].desc);
        config_write.print(ports[i].name);
        config_write.print(F("=0x"));
        if (*ports[i].port<16) {
          config_write.print(F("0"));
        }
        config_write.println(*ports[i].port, HEX);
      }
      for (int i = 0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++){
        config_write.print(F("# "));
        config_write.println(features[i].desc);
        config_write.print(features[i].name);
        config_write.print(F("="));
        if ( *features[i].feature ) {
          config_write.println(F("true"));
        }
        else {
          config_write.println(F("false"));
        }
      }
      
      message(STORAGE, F("#OK"));
    }
    else {

      message(STORAGE, F("#FAILED to open file for writing"));

    }

    config_write.close();

    SPI_lock = false;

    message(STORAGE, F("#Done!"));
  }

  if (fatfs.exists("/config.txt")) {
    File mainPy = fatfs.open("/config.txt", FILE_READ);
    Serial.println("Printing main.py...");
    while (mainPy.available()) {
      char c = mainPy.read();
      Serial.print(c);
    }
    Serial.println();
  }
  else {
    Serial.println("No main.py found...");
  }

}

#else

#endif
