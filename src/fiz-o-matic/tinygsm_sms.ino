



void tinygsm_sms() {

  // exit if SIM is not OK
  if ( !tinygms_ok ) return;

  //Watchdog.disable();

  int numofsms = getNumSMS();


  char sender[18];

  message(TINYGSM_MSG, F("Read SMS"));
  for (int i = 1; i < numofsms + 1; i++) {
    message(TINYGSM_MSG, F("Read SMS# "));
    message(TINYGSM_MSG, String(i, DEC));
    String smsmsg = readSMS(i, sender);
    smsmsg.toLowerCase();
    //    sender.replace('"', 0);
    message(TINYGSM_MSG, F("#Sender: "));
    message(TINYGSM_MSG, sender);
    message(TINYGSM_MSG, F("Message: "));
    message(TINYGSM_MSG, F("##############################"));
    message(TINYGSM_MSG, smsmsg);
    message(TINYGSM_MSG, F("##############################"));

    // Check if keyword present
    if ( smsmsg.indexOf(sms_keyword) >= 0 ) {
      message(TINYGSM_MSG, F("SMS Keyword is okay"));
      if ( smsmsg.indexOf(F("on")) >= 0 ) {
        go_online = true;
        //message(TINYGSM_MSG,F("delete SMS"));
        //deleteSMS(i);
      }
      else if ( smsmsg.indexOf("off") >= 0 ) {
        Blynk.notify(F("disconnect Blynk"));

        if ( tinygsm_go_offline() ) {
          online = false;
          //message(TINYGSM_MSG,F("delete SMS"));
          //deleteSMS(i);
          message(TINYGSM_MSG,F("Disconnected"));
        }
      }
      /*else if ( smsmsg.indexOf("status") >= 0 ) {
        //Watchdog.reset();
        modem.sendSMS(sender, F("I'm okay."));
        //Watchdog.reset();
        //message(TINYGSM_MSG,F("delete SMS"));
        //deleteSMS(i);
      }*/
      else {
        String request_msg = smsmsg.substring(smsmsg.indexOf(" "));
        request_msg.trim();
        request_msg = request_msg.substring(0, smsmsg.indexOf("\n"));
        Serial.println(request_msg);

        if ( parse_config_string(request_msg) ) {
          modem.sendSMS(sender, "OK");
        }
        else {
          //parse_config_string( smsmsg.substring( smsmsg.indexOf(" ") ) );

          String return_msg = parse_ui_string(request_msg, SRC_SMS);
          Serial.println(return_msg);
          modem.sendSMS(sender, return_msg);
        }


        //message(TINYGSM_MSG,F("delete SMS"));
        //deleteSMS(i);
      }
    }
    else {
      //modem.sendSMS(, return_msg);
    }

    message(TINYGSM_MSG,F("delete SMS"));
    deleteSMS(i);

  }
  //Watchdog.enable(4000);
  yield();
}

void tinygsm_send_sms(String message) {
  modem.sendSMS(my_number, message);
}
void tinygsm_sms_alarm() {

  message(INFO_MSG ,F("tinygsm_sms_alarm"));
  //modem.sendSMS(MYNUMBER, F("ALARM!!!"));
  modem.sendSMS(my_number, F("ALARM!!!"));
  Blynk.notify(F("ALARM!!!"));

  blynk_msg(F("ALARM!!!"));

  alarm_led.on();

  if (geo_fence_alarm) {
    geo_fence_led.on();
  }
}


/*
*
* Special function which are not implemented in the TinyGSM library
*
*/

int getNumSMS() {

    modem.sendAT(GF("+CMGF=1"));
    if (modem.waitResponse() != 1) {
        return 0;
    }

    modem.sendAT(GF("+CPMS?"));
    if (modem.waitResponse(GF(GSM_NL "+CPMS:")) != 1) {
        return 0;
    }
    String res = modem.stream.readStringUntil('\n');
    int index1 = res.indexOf(",");
    int index2 = res.indexOf(",", index1+1);
    String tmp = res.substring(index1+1,index2);
    modem.waitResponse();
    //res.trim();
    tmp.trim();
    //return res;
    //int num = tmp.toInt();
    return tmp.toInt();
}

String readSMS(int num, char *sender) {
    String buffer;

    modem.sendAT(GF("+CMGF=1"));
    if (modem.waitResponse() != 1) {
        return "";
    }
    modem.sendAT(GF("+CSDH=1"));
    if (modem.waitResponse() != 1) {
        return "";
    }


    modem.sendAT(GF("+CMGR="), num, GF(""));
    if (modem.waitResponse(GF(GSM_NL "+CMGR:")) != 1) {
        return "";
    }

    modem.stream.readStringUntil(',');
    buffer = modem.stream.readStringUntil(',');
    buffer.replace('"', ' ');
    buffer.trim();
    buffer.toCharArray(sender, 18);

    modem.stream.readStringUntil('\r');

    buffer = "";

    delay(20); // Wait a moment to get data into the buffer
    while (modem.stream.available()) {
        char c = modem.stream.read();
        buffer = buffer + c;
    }

    String res = buffer;
    modem.waitResponse();
    res.trim();
    return res;
}

boolean deleteSMS(int num) {
    modem.sendAT(GF("+CMGD="), num, GF(""));
    if (modem.waitResponse(GF(GSM_NL "+CMGD:")) != 1) {
        return false;
    }
    return true;
}
