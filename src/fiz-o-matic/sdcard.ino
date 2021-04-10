/****************************************************
 * FIZ-o-matic
 * https://fiz-o-matic.net/
 *
 *  This code is based on the SdFat library from Bill Greiman.
 *  GIT: https://github.com/greiman/SdFat
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
 ****************************************************/


#ifdef SDCARD

#include <SPI.h>
#include "SdFat.h"

SdFat SD;
SdFile logfile;


void sdcard_init() {
  message(STORAGE, F("#->enable_sdcard\n"));
  if ( !SPI_lock ) {
    SPI_lock = true;
    for (int i = 0; i<5; i++) {

      SPI_lock = false;
      notify(BOOTMSG, F("#Init SD Card"));
      SPI_lock = true;
      // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
      // breadbords.  use SPI_FULL_SPEED for better performance.
      if (!SD.cardBegin(SD_CS, SPI_HALF_SPEED)) {
        SPI_lock = false;
        notify(BOOTMSG, F("cardBegin failed"));
        message(ERROR, F("cardBegin failed\n"));
        notify(2, F("SD failed"));
        SPI_lock = true;
        delay(500);
      }
      else {
        i = 100;
        SDmount = true;
        SPI_lock = false;
        notify(BOOTMSG, F("SD Card ok"));
        SPI_lock = true;

        if (!SD.fsBegin()) {
          SPI_lock = false;
          notify(BOOTMSG, F("FS init failed."));
          message( ERROR, F("FS init failed\n"));
          notify(2, F("SD failed"));
          SPI_lock = true;
          delay(1000);
          SDmount = false;
        }
        else {
          sd_ok = true;
        }
      }
    }

    SPI_lock = false;
  }
}

void sdcard_open_config() {
  message(STORAGE, F("#->open_config\n"));
  String tmp;
  int n;

  notify(BOOTMSG, F("Open Config"));

  SdFile rdfile("config.txt", O_READ);

  // check for open error
  if (!rdfile.isOpen()) {
    notify(BOOTMSG, F("can't read config"));
  }

  while ((n = rdfile.fgets(replybuffer, sizeof(replybuffer))) > 0) {
    tmp = String(replybuffer);
    if ( tmp.startsWith("#") ) {
      //SD_DEBUG_PRINTLN(F("found comment"));
    }
    else {
      parse_config_string(tmp);
    }

    lastfile = lastfile_config * 10;


  }
}

void log_to_sdcard() {
  if ( log_timer < millis() ) {
    log_timer = millis() + LOG_TIMER;
    message(STORAGE, F("#->log_to_sdcard\n"));
    if ( !SDmount ) return;
    if ( !SPI_lock ) {
      SPI_lock = true;
      digitalWrite(8, HIGH);


      if (filename[0] == '-') {
        message(STORAGE,F("#no file opened"));
      }
      else {
        if ( gps_fixstatus ) {
          logfile.print(gps_longitude, DEC);
          logfile.print(F(","));
          logfile.print(gps_latitude, DEC);
          logfile.print(F(","));
          logfile.println(gps_altitude, DEC);
          logfile.flush();
        }


        second = rtc.getSeconds();
        minute = rtc.getMinutes();
        hour = rtc.getHours();
        day = rtc.getDay();
        month = rtc.getMonth();
        year = 2000 + rtc.getYear();

        long running = engine_running_total + engine_running_sec;

        logfile.print(F("<!--"));
        logfile.print(year);
        logfile.print(F("/"));
        logfile.print(month);
        logfile.print(F("/"));
        logfile.print(day);
        logfile.print(F(" "));
        logfile.print(hour);
        logfile.print(F(":"));
        logfile.print(minute);
        logfile.print(F(":"));
        logfile.print(second);
        logfile.print(F(","));
        logfile.print(gps_fixstatus);
        logfile.print(F(","));
        logfile.print(gps_latitude, DEC);
        logfile.print(F(","));
        logfile.print(gps_longitude, DEC);
        logfile.print(F(","));
        logfile.print(gps_altitude, DEC);
        logfile.print(F(","));
        logfile.print(gps_speed, DEC);
        logfile.print(F(","));
        logfile.print(gps_course, DEC);
        logfile.print(F(","));
        logfile.print(gps_view_satellites, DEC);
        logfile.print(F(","));
        logfile.print(gps_used_satellites, DEC);
        logfile.print(F(","));
        logfile.print(gps_distance, DEC);
        logfile.print(F(","));
        //logfile.print(bord_voltage, 1);
        //logfile.print(F(","));
        logfile.print(engine_running_total, DEC);
        logfile.print(F(","));

        logfile.print(running, DEC);
        logfile.print(F(","));
        logfile.println(F("-->"));

        logfile.flush();

        if (logfile.getWriteError()) {
          Serial.println(F("#write error"));
          SDerror = true;
        }
        if (!logfile.sync()) {
          Serial.println(F("#sync error"));
          SDerror = true;
        }
      }
      SPI_lock = false;
    }
    else {
      message(STORAGE, F("#SPI Bus locked for log_to_sdcard\n"));
    }

    digitalWrite(8, LOW);

  }
}

void dump_sd_card() {
  notify(BOOTMSG, F("dumping ..."));

  File dir = SD.open("/");

  // Open next file in root.  The volume working directory, vwd, is root.
  // Warning, openNext starts at the current position of sd.vwd() so a
  // rewind may be neccessary in your application.
  SD.vwd()->rewind();

  while (logfile.openNext(SD.vwd(), O_READ)) {
    if (logfile.isDir()) {
      // Indicate a directory.
      Serial.write('/');
    }
    char chfilename[14];
    logfile.getName(chfilename, 13);
    Serial.println(chfilename);
    String filename = String(chfilename);
    int filetest = filename.indexOf(".KML");
    if ( filetest == 8 ) {
      notify(BOOTMSG, F("dumping "));
      notify(BOOTMSG, chfilename);
      while (logfile.available()) {
        Serial.write(logfile.read());
      }
    }


    logfile.close();
  }

  Serial.println(F("#Dump Complete"));
}

void dump_file(int filenumber) {
  char filename_tmp[16];
  char filename[16];

  strcpy(filename, "TRACK000.KML");
  filename[5] = '0' + filenumber / 100;
  filename[6] = '0' + filenumber / 10 - (filenumber / 100 * 10);
  filename[7] = '0' + filenumber % 10;

  File dataFile = SD.open(filename);
  while (dataFile.available()) {
    Serial.write(dataFile.read());
  }
  Serial.println();
}





void get_last_log(void) {
  message(STORAGE, F("#->get_last_log\n"));
  notify(BOOTMSG, F("Check Files"));

  if ( !SDmount ) return;

  if ( !SPI_lock ) {
    SPI_lock = true;

    char filename_tmp[16];
    char filename[16];
    char ch;
    int field = 0;
    int i=0;
    boolean comment = false;
    boolean fileclosed = false;
    byte nofound_cnt = 0;



    strcpy(filename_tmp, "TRACK000.KML");
    for (uint16_t i = (lastfile_config * 10); i < 1000; i++) {
      filename_tmp[5] = '0' + i / 100;
      filename_tmp[6] = '0' + i / 10 - (i / 100 * 10);
      filename_tmp[7] = '0' + i % 10;
      message(STORAGE, F("#Check file: "));
      message(STORAGE, filename_tmp);
      if (! SD.exists(filename_tmp)) {
        message(STORAGE, F(" doesn't exist\n"));
        nofound_cnt++;
        if ( nofound_cnt >= 50 ) break;
      }
      else {
        message(STORAGE, F(" exist\n"));
        strcpy(filename, filename_tmp);
        lastfile = i;
        lastfile_config = i / 10;
      }
    }

    message(STORAGE, F("#Last Log:"));
    message(STORAGE, filename);
    message(STORAGE, F("\n"));
    File dataFile = SD.open(filename);
    unsigned long filesize = dataFile.size();

    message(STORAGE, F("#FileSize: "));
    message(STORAGE, String(filesize, DEC));
    message(STORAGE, F("\n"));

    filesize -= 500;
    dataFile.seek(filesize);
    if (dataFile) {
      while (dataFile.available()) {
        ch = dataFile.read();
        if (ch == '\n') {
          field = 0;
          i = 0;
          //clear the buffer
          for (i = 0; i < 200; i++) {
            replybuffer[i] = '\0';
          }
          comment = true;
        } else {
          // searching for field delimiter
          if (ch == ',') {
            if (comment) {
              switch (field) {
                case 2:
                  gps_latitude = atof(replybuffer);
                  message(STORAGE, F("#gps_latitude: "));
                  message(STORAGE, String(gps_latitude));
                  message(STORAGE, F("\n"));
                  break;
                case 3:
                  gps_longitude = atof(replybuffer);
                  message(STORAGE, F("#gps_longitude: "));
                  message(STORAGE, String(gps_longitude));
                  message(STORAGE, F("\n"));
                  break;
                case 9:
                  gps_distance = atoi(replybuffer);
                  message(STORAGE, F("#gps_distance: "));
                  message(STORAGE, String(gps_distance));
                  message(STORAGE, F("\n"));
                  break;
                case 11:
                  engine_running_total = atoi(replybuffer);
                  message(STORAGE, F("#engine_running_total: "));
                  message(STORAGE, String(engine_running_total));
                  message(STORAGE, F("\n"));
              }

              //clear the buffer
              for (i = 0; i < 200; i++) {
                replybuffer[i] = '\0';
              }

              field++;
              i = 0;
            }


          }
          else if ( ch == '!' ) {
            comment = true;
          }
          else if ( ch == '%' ) {
            fileclosed = true;
            //message(STORAGE, "#last file is closed\n");
          }
          else {
            replybuffer[i] = ch;
            i++;
          }
        }

      }
      dataFile.close();

      if ( !fileclosed ) {
        message(STORAGE, F("#last file is not closed...\n"));
        if (!dataFile.open(filename, O_RDWR | O_CREAT | O_AT_END)) {
          SD.errorHalt(F("opening test.txt for write failed"));
          //delay(2000);
        }
        else {
          dataFile.println(F("</coordinates>"));
          dataFile.println(F("</LineString>"));
          dataFile.println(F("</Placemark>"));
          dataFile.println(F("</kml>"));
          dataFile.print(F("<!--%-->"));
          Serial.println(F("#last file now closed"));
        }
        dataFile.close();
      }
      else {
        message(STORAGE, "#last file is closed\n");
      }
    }
    SPI_lock = false;
  }
  else {
    message(TRACE_MSG, F("#SPI locked\n"));
  }
}

void open_file() {
  message(STORAGE, F("#->open_file\n"));

  if ( !SDmount ) return;

  if ( !SPI_lock ) {
    SPI_lock = true;

    // make sure that no other file is opened
    //logfile.close();

    // set date time callback function
    SdFile::dateTimeCallback(dateTime);

    strcpy(filename, "TRACK000.KML");

    for (uint16_t i = lastfile; i < 1000; i++) {
      filename[5] = '0' + i / 100;
      filename[6] = '0' + i / 10 - (i / 100 * 10);
      filename[7] = '0' + i % 10;
      message(STORAGE, F("#Check file: "));
      message(STORAGE, filename);
      message(STORAGE, F("\n"));
      if (! SD.exists(filename)) {
        lastfile = i;
        lastfile_config = i / 10;
        break;
      }
    }

    if ( !logfile.open(filename, O_RDWR | O_CREAT | O_AT_END) ) {
      message(STORAGE, F("#Couldnt create "));
      message(STORAGE, filename);
      message(STORAGE, F("\n"));
    }
    message(INFO_MSG, "#Writing to ");
    message(INFO_MSG, filename);
    message(INFO_MSG, F("\n"));
    logfile.println(F("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
    logfile.println(F("<kml xmlns=\"http://earth.google.com/kml/2.2\">"));
    logfile.println(F("<Placemark>"));
    logfile.println(F("<LineString>"));
    logfile.println(F("<coordinates>"));

    SPI_lock = false;

    log_to_sdcard();

  }
}

void close_file() {
  message(STORAGE, F("#->close_file\n"));

  if ( !SDmount ) return;

  if ( !SPI_lock ) {
    SPI_lock = true;
    logfile.println(F("</coordinates>"));
    logfile.println(F("</LineString>"));
    logfile.println(F("</Placemark>"));
    logfile.println(F("</kml>"));
    logfile.print(F("<!--%-->"));
    logfile.close();
    message(INFO_MSG, F("#close file "));
    message(INFO_MSG, filename);
    message(INFO_MSG, F("\n"));
    strcpy(filename, "-");
    SPI_lock = false;
  }
}




void sdcard_save_config() {
  message(STORAGE, F("#->save_config\n"));
  message(INFO_MSG, F("#Writing config to SD\n"));

  if ( !SDmount ) return;

  if ( !SPI_lock ) {
    SPI_lock = true;

    if ( !SD.remove("config.txt")) {
      message(STORAGE, F("#can't remove config.txt\n"));
    }

    SdFile::dateTimeCallback(dateTime);

    if ( !logfile.open("config.txt", O_RDWR | O_CREAT | O_AT_END) ) {
      message(STORAGE, F("#Couldnt create config.txt\n"));
    }
    else {

      message(STORAGE, F("#Writing config.txt\n"));

      logfile.println(F("#This is the configuration file"));

      // TinyGSM
      logfile.println(F("#SIM PIN"));
      logfile.print(F("sim_pin="));
      logfile.println(sim_pin);
      logfile.println(F("#Provider APN"));
      logfile.print(F("apn="));
      logfile.println(apn);
      logfile.println(F("#username for the APN"));
      logfile.print(F("apn_user="));
      logfile.println(apn_user);
      logfile.println(F("#password for the APN"));
      logfile.print(F("apn_pass="));
      logfile.println(apn_pass);
      logfile.println(F("#BLYNK authentication key"));
      logfile.print(F("blynk_key="));
      logfile.println(blynk_key);
      logfile.println(F("#BLYNK Server (optional)"));
      logfile.print(F("blynk_server="));
      logfile.println(blynk_server);
      logfile.println(F("#BLYNK Server Port (optional)"));
      logfile.print(F("blynk_port="));
      logfile.println(blynk_port);
      logfile.println(F("#SMS Keyword for authentication"));
      logfile.print(F("sms_keyword="));
      logfile.println(sms_keyword);


      for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
        logfile.print(F("# "));
        logfile.println(config[i].desc);
        logfile.print(config[i].name);
        logfile.print(F("="));
        logfile.println(*config[i].config, DEC);
      }

      for (int i = 0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++){
        logfile.print(F("# "));
        logfile.println(ports[i].desc);
        logfile.print(ports[i].name);
        logfile.print(F("=0x"));
        if (*ports[i].port<16) {
          logfile.print(F("0"));
        }
        logfile.println(*ports[i].port, HEX);
      }
      for (int i = 0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++){
        logfile.print(F("# "));
        logfile.println(features[i].desc);
        logfile.print(features[i].name);
        logfile.print(F("="));
        if ( *features[i].feature ) {
          logfile.println(F("true"));
        }
        else {
          logfile.println(F("false"));
        }
      }


      logfile.flush();

      if (logfile.getWriteError()) {
        message(ERROR, F("#write error\n"));
        SDerror = true;
      }
      if (!logfile.sync()) {
        message(ERROR, F("#sync error\n"));
        SDerror = true;
      }

      logfile.close();

      if (logfile.getWriteError()) {
        message(ERROR, F("#write error\n"));
        SDerror = true;
      }
    }

    SPI_lock = false;
    message(STORAGE, F("#OK\n"));
    message(INFO_MSG, F("#Done!\n"));
  }
}


void sdcard_ls() {
  if ( !sd_ok ) {
    message( ERROR, F("SD not ready\n"));
  }

  SPI_lock = true;
  SD.ls("/", LS_R);
  SPI_lock = false;
}

void sdcard_free() {
  if ( !sd_ok ) {
    message( ERROR, F("SD not ready\n"));
  }

  uint32_t m = micros();
  uint32_t volFree = SD.vol()->freeClusterCount();
  uint32_t size = SD.card()->cardSize();
  float fs = 0.000512*volFree*SD.vol()->blocksPerCluster();
  message(F("SD Card size: "));
  message(String(size, 0));
  message(F(" MB\n"));
  message(F("free Space: "));
  message(String(fs, 0));
  message(F(" MB\n"));
}


/*
 * call back for file timestamps
 */
void dateTime(uint16_t* date, uint16_t* time) {
  message(TRACE_MSG, F("#->dateTime\n"));

  minute = rtc.getMinutes();
  hour = rtc.getHours();
  day = rtc.getDay();
  month = rtc.getMonth();
  year = 2000 + rtc.getYear();


  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(year, month, day);

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(hour, minute, second);
//  #endif
}

#else
// dummy functions
void sdcard_save_config()
{}
void sdcard_init()
{}
void sdcard_open_config()
{}
void get_last_log()
{}
void log_to_sdcard(){}
void sdcard_free()
{}
void dump_file(int filenumber) {}
void dump_sd_card()
{}
void sdcard_ls()
{}
void open_file()
{}
void close_file()
{}

#endif //SDCARD
