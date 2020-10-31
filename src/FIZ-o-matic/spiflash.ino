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

File myFile;
File spiflash_config;


//------------------------------------------------------------------------------
void init_spiflash() {
  message(STORAGE, "Initializing Filesystem on external flash...");

  // Init external flash
  flash.begin();

  // Init file system on the flash
  fatfs.begin(&flash);

  message(STORAGE, "initialization done.");
}

void spiflash_open_config() {
  String tmp;

  spiflash_config = fatfs.open("config.txt");
  if (spiflash_config) {
    Serial.println("config.txt:");

    // read from the file until there's nothing else in it:
    while (spiflash_config.available()) {
      //Serial.write(spiflash_config.read());
      tmp = spiflash_config.readStringUntil('\r');
      tmp.trim();
      if ( !tmp.startsWith("#") ) {
        message(STORAGE, tmp);
        parse_config_string(tmp);
      }
    }
    // close the file:
    spiflash_config.close();
  } else {
    // if the file didn't open, print an error:
    spiflash_config.println("error opening config.txt");
  }
}

void spiflash_save_config() {
  message(STORAGE, F("#Writing config to SPI FLASH"));

  if ( !SPI_lock ) {
    SPI_lock = true;

    if ( !fatfs.remove("config.txt")) {
      message(STORAGE, F("#can't remove config.txt"));
    }

    spiflash_config = fatfs.open("config.txt", FILE_WRITE);

    if (spiflash_config) {
      spiflash_config.println(F("#This is the configuration file"));

      // TinyGSM
      spiflash_config.println(F("#SIM PIN"));
      spiflash_config.print(F("sim_pin="));
      spiflash_config.println(sim_pin);
      spiflash_config.println(F("#Provider APN"));
      spiflash_config.print(F("apn="));
      spiflash_config.println(apn);
      spiflash_config.println(F("#username for the APN"));
      spiflash_config.print(F("apn_user="));
      spiflash_config.println(apn_user);
      spiflash_config.println(F("#password for the APN"));
      spiflash_config.print(F("apn_pass="));
      spiflash_config.println(apn_pass);
      spiflash_config.println(F("#BLYNK authentication key"));
      spiflash_config.print(F("blynk_key="));
      spiflash_config.println(blynk_key);
      spiflash_config.println(F("#SMS Keyword for authentication"));
      spiflash_config.print(F("sms_keyword="));
      spiflash_config.println(sms_keyword);
      spiflash_config.println(F("#My Mobile Number for Notifications"));
      spiflash_config.print(F("my_number="));
      spiflash_config.println(my_number);




      for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
        spiflash_config.print(F("# "));
        spiflash_config.println(config[i].desc);
        spiflash_config.print(config[i].name);
        spiflash_config.print(F("="));
        spiflash_config.println(*config[i].config, DEC);
      }

      for (int i = 0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++){
        spiflash_config.print(F("# "));
        spiflash_config.println(ports[i].desc);
        spiflash_config.print(ports[i].name);
        spiflash_config.print(F("=0x"));
        if (*ports[i].port<16) {
          spiflash_config.print(F("0"));
        }
        spiflash_config.println(*ports[i].port, HEX);
      }
      for (int i = 0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++){
        spiflash_config.print(F("# "));
        spiflash_config.println(features[i].desc);
        spiflash_config.print(features[i].name);
        spiflash_config.print(F("="));
        if ( *features[i].feature ) {
          spiflash_config.println(F("true"));
        }
        else {
          spiflash_config.println(F("false"));
        }
      }


      spiflash_config.flush();


    }


    SPI_lock = false;
    message(STORAGE, F("#OK"));
    message(STORAGE, F("#Done!"));
  }
}

#else

#endif
