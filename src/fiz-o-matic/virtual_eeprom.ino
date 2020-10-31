/*
 * Read the virtual EEPROM
 */
void read_virtual_eeprom() {

  int addr = 0;

  if (EEPROM.isValid()) {
    message(INFO_MSG, F("#read EEPROM!\n"));

    message(DEBUG_MSG, F("#config ->"));
    for (int i=0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++) {
      message(DEBUG_MSG, F(" "));
      message(DEBUG_MSG, String(EEPROM.read(addr), DEC));

      *config[i].config = EEPROM.read(addr);
      addr++;
    }

    message (DEBUG_MSG, F("\n#ports ->"));
    for (int i=0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++) {
      message(DEBUG_MSG, F(" "));
      message(DEBUG_MSG, String(EEPROM.read(addr), DEC));

      *ports[i].port = EEPROM.read(addr);
      addr++;
    }

    message (DEBUG_MSG, F("\n#features ->"));
    for (int i=0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++) {
      message(DEBUG_MSG, F(" "));
      message(DEBUG_MSG, String(EEPROM.read(addr), DEC));

      if ( EEPROM.read(addr) == 1 ) {
        *features[i].feature = true;
      }
      else {
        *features[i].feature = false;
      }
      addr++;
    }

    message (DEBUG_MSG, F("\n"));



    char_config = flash_char_config.read();

    sim_pin = String(char_config.sim_pin);
    apn = String(char_config.apn);
    apn_user = String(char_config.apn_user);
    apn_pass = String(char_config.apn_pass);
    blynk_key = String(char_config.blynk_key);
    sms_keyword = String(char_config.sms_keyword);
    my_number = String(char_config.my_number);


    message (DEBUG_MSG, F("\n#sim_pin -> "));
    message (DEBUG_MSG, char_config.sim_pin);
    message (DEBUG_MSG, F("\n#apn -> "));
    message (DEBUG_MSG, char_config.apn);
    message (DEBUG_MSG, F("\n#apn_user -> "));
    message (DEBUG_MSG, char_config.apn_user);
    message (DEBUG_MSG, F("\n#apn_pass -> "));
    message (DEBUG_MSG, char_config.apn_pass);
    message (DEBUG_MSG, F("\n#blynk_key -> "));
    message (DEBUG_MSG, char_config.blynk_key);
    message (DEBUG_MSG, F("\n#sms_keyword -> "));
    message (DEBUG_MSG, char_config.sms_keyword);
    message (DEBUG_MSG, F("\n#my_number -> "));
    message (DEBUG_MSG, char_config.my_number);
    message (DEBUG_MSG, F("\n"));

  } else {
    notify(BOOTMSG, F("config flash is empty"));
  }
}


/*
 * Write virtual EEPROM
 */
void write_virtual_eeprom() {

  int config_i = 0;
  int port_i = 0;
  int addr = 0;
  message(INFO_MSG, F("#Writing config to virtual EEPROM\n"));
  for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
    EEPROM.write(addr, *config[i].config);
    config_i = i;
    addr++;
  }
  config_i++;
  for (int i = 0; i <= (sizeof(ports) / sizeof(ports[0])) - 1; i++){
    EEPROM.write(addr, *ports[i].port);

    port_i = i;
    addr++;
  }
  port_i++;
  for (int i = 0; i <= (sizeof(features) / sizeof(features[0])) - 1; i++){
    if ( *features[i].feature ) {
      EEPROM.write(addr, 1);
    }
    else {
      EEPROM.write(addr, 0);
    }
    addr++;
  }

  char_config = flash_char_config.read();
  sim_pin.toCharArray( char_config.sim_pin, 6);
  apn.toCharArray( char_config.apn, 36);
  apn_user.toCharArray( char_config.apn_user, 12);
  apn_pass.toCharArray( char_config.apn_pass, 8);
  blynk_key.toCharArray( char_config.blynk_key, 36);
  sms_keyword.toCharArray( char_config.sms_keyword, 36);
  my_number.toCharArray( char_config.my_number, 16);

  flash_char_config.write(char_config);

  EEPROM.commit();
  message(INFO_MSG, F("#Done!\n"));
  if ( EEPROM.isValid() ) {
    message(INFO_MSG, F("#EEPROM is valid\n"));
  }

  read_virtual_eeprom();

}
