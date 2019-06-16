/***************************************************
 *  This sketch is to read the local analog ports.
 *
 *  Author: Brun
 *
 ****************************************************/

int vw_temp [][2] PROGMEM = {  {1000, 20},
                        {800, 26},
                        {700, 30},
                        {500, 40},
                        {425, 44},
                        {350, 50},
                        {300, 55},
                        {250, 60},
                        {212, 65},
                        {180, 69},
                        {150, 75},
                        {125, 80},
                        {107, 84},
                        {89, 90},
                        {75, 95},
                        {60, 101},
                        {53, 105},
                        {45, 110},
                        {38, 114},
                        {32, 119},
                        {26, 125}};

unsigned long IO_timer = 0;

void IO_init() {

  #ifdef ALARM_OUT
  /*pinMode(ALARM_PORT, OUTPUT);

  digitalWrite(ALARM_PORT, HIGH);
  delay(100);
  digitalWrite(ALARM_PORT, LOW);
  delay(100);*/
  #endif //ALARM_OUT


  //pinMode(A0,INPUT_PULLUP)
  /*digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);*/

  /*#if defined HW_FEATHER
  pinMode(A4, INPUT);           // set pin to input
  pinMode(A5, INPUT);           // set pin to input
  digitalWrite(A4, HIGH);       // turn on pullup resistors
  digitalWrite(A5, HIGH);       // turn on pullup resistors
  attachInterrupt(A4, interrupt_A4, FALLING); // attach interrupt
  attachInterrupt(A5, interrupt_A5, FALLING);
  #endif*/
  #ifdef A0_MULTIPLICATOR
  reg_port(0x01, TYPE_Volt);
  #endif
  #ifdef A1_MULTIPLICATOR
  reg_port(0x02, TYPE_Volt);
  #endif
  #ifdef A2_MULTIPLICATOR
  reg_port(0x03, TYPE_Volt);
  #endif
  #ifdef A3_MULTIPLICATOR
  reg_port(0x04, TYPE_Volt);
  #endif
  #ifdef A4_MULTIPLICATOR
  reg_port(0x05, TYPE_Volt);
  #endif
  #ifdef A5_MULTIPLICATOR
  reg_port(0x06, TYPE_Volt);
  #endif

  #ifdef A4_COUNTER
  pinMode(A4, INPUT);           // set pin to input
  digitalWrite(A4, HIGH);       // turn on pullup resistors
  attachInterrupt(A4, interrupt_A4, FALLING); // attach interrupt
  reg_port(0x05, TYPE_Hz);      // register port
  #endif

  #ifdef A5_COUNTER
  pinMode(A5, INPUT);           // set pin to input
  digitalWrite(A5, HIGH);       // turn on pullup resistors
  attachInterrupt(A5, interrupt_A5, FALLING); // attach interrupt
  reg_port(0x06, TYPE_Hz);      // register port
  #endif

  #ifdef GPIO6
  reg_port(0x07, TYPE_undef);
  #endif

  #ifdef GPIO13
  reg_port(0x08, TYPE_undef);
  #endif


  #ifdef U8G2_DISPLAY_BG_LED
  analogWrite(U8G2_DISPLAY_BG_LED, DIMMER_MIN);
  #endif


  read_ports();

}

void IO_loop() {

  if ( IO_timer < millis() ) {
    //TRACE_PRINTLN(F("#analog_loop()"));
    IO_timer = millis() + IO_TIMER;

    // read the voltage of each port
    read_ports();

    // get values from OneWire
    #ifdef ONEWIRE
    onewire_loop();
    #endif // ONEWIRE

    // get values from I2C
    #ifdef I2C
    i2c_loop();
    #endif // SI7021

    vw_water_temp();

    get_bord_voltage();

    #ifdef U8G2_DISPLAY_BG_LED
    dimmer();
    #endif

    get_rpm();

    get_clima_out();

    get_speed();

    get_fuel();


  }

}

void vw_water_temp() {
  //TRACE_PRINTLN(F("#vw_water_temp()"));
  float val=0;
  float temp = 0;

  water_temp = 0;

  switch (water_temp_port) {
    case 1: val = a0_V; break;
    case 2: val = a1_V; break;
    case 3: val = a2_V; break;
    case 4: val = a3_V; break;
  }

  float ohm = SERIESRESISTOR * (val / (10 - val));
  water_temp_ohm = ohm;

  for (int i=0; i<sizeof(vw_temp)/sizeof(vw_temp[0]);i++) {
    if (ohm > vw_temp[i][0] ) {
      water_temp = pgm_read_word_near(&vw_temp[i][1]);
      i=sizeof(vw_temp)/sizeof(vw_temp[0]);
    }
  }
}

void get_bord_voltage() {
  //TRACE_PRINTLN(F("#bord_voltage()"));
  /*switch (bord_voltage_port) {
    case 1: bord_voltage = a0_V; break;
    case 2: bord_voltage = a1_V; break;
    case 3: bord_voltage = a2_V; break;
    case 4: bord_voltage = a3_V; break;
  }*/

  bord_voltage = get_port_value(bord_voltage_port);

  //TRACE_PRINT(F("#BORD VOLTAGE: "));
  //TRACE_PRINTLN(bord_voltage);

  /*if (bord_voltage > 4) {
    if (!running) {
      running = true;
      start_engine();
    }
  }
  else {
    if (running) {
      running = false;
      stop_engine();
    }
  }*/
}

#ifdef U8G2_DISPLAY_BG_LED
void dimmer() {
  dimmer_V = get_port_value(dimmer_port);

  int dimmer_val = 0;

  int dimmer_pct = ((100000 / (DIMMER_MAX_mV - DIMMER_MIN_mV)) * (dimmer_V * 1000 - DIMMER_MIN_mV)) / 1000;

  if ( dimmer_pct < 5 ) {
    if (running) {
      dimmer_pct = 5;
    } else {
      if ( dimmer_active_timer > millis() ) {
        dimmer_pct = (dimmer_active * 100 ) / dimmer_max;
      }
      else {
        dimmer_pct = 0;
      }
    }
  }

  //dimmer_val = ( dimmer_pct * DIMMER_MAX ) / 100;
  dimmer_val = ((dimmer_max - dimmer_min) * dimmer_pct) / 100;
  if (alarm_counter == 0) {
    analogWrite(U8G2_DISPLAY_BG_LED, dimmer_val);
  }

  #ifdef OFFLINE
  analogWrite(U8G2_DISPLAY_BG_LED, 180);
  #endif

}
#endif


/*
 * RPM
 */
void get_rpm() {
  int val = 0;
  switch (rpm_port) {
    case 5: val = a4_hz; a4_hz = 0; break;
    case 6: val = a5_hz; a5_hz = 0; break;
  }

  rpm = (float(val) / rpm_multipl) * 60;

  //TRACE_PRINT(F("#RPM: "));
  //TRACE_PRINTLN(rpm);

}

void get_clima_out() {
  /*switch (temp_out_port) {
    case 1:
      temp_out = lm75_1_temp;
      hum_out = -1000;
      break;
    case 2:
      temp_out = ds18b20_temp;
      hum_out = -1000;
      break;
    case 3:
      temp_out = si7021_temp;
      hum_out = si7021_hum;
      break;
  }*/

  temp_out = get_port_value(temp_out_port);
}


/*
 * Speed
 */
void get_speed() {
  int val = 0;
  uint32_t speed_sum = 0;
  switch (speedpulse_port) {
    case 5: val = a4_hz; a4_hz = 0; break;
    case 6: val = a5_hz; a5_hz = 0; break;
  }

  speedpulse_tmp[SPEEDPULSEARRAY-1] = ( val * 3600 ) / 4000;

  for (int i = 0; i < SPEEDPULSEARRAY-1; i++) {
    speedpulse_tmp[i] = speedpulse_tmp[i+1];
    speed_sum += speedpulse_tmp[i];
  }
  speed_sum += speedpulse_tmp[SPEEDPULSEARRAY-1];
  speedpulse = speed_sum / SPEEDPULSEARRAY;

  //TRACE_PRINT(F("#SPEEDPULSE: "));
  //TRACE_PRINTLN(speedpulse);

  switch (speed_source) {
    case 0:
      speed = 0;
      #ifdef GPS
      if (gps_fixstatus) {
        if (gps_speed >= 5) {
          speed = int(gps_speed);
        }
        else {
          speed = 0;
        }
      }
      else {
        speed = 0;
      }
      #endif

      if (speed == 0) {
        speed = speedpulse;
      }
      break;
    case 1:
      speed = speedpulse;
      break;
    case 2:
      speed = int(gps_speed);
      break;
  }

}

void get_fuel() {
  //TRACE_PRINTLN(F("#get_fuel()"));
  switch (fuel_port) {
    case 1: fuel_V = a0_V; break;
    case 2: fuel_V = a1_V; break;
    case 3: fuel_V = a2_V; break;
    case 4: fuel_V = a3_V; break;
  }

  //TRACE_PRINT(F("#FUEL VOLTAGE: "));
  //TRACE_PRINTLN(fuel_V);

  fuel_ohm = FUEL_GAUGERESISTOR * (fuel_V  / (10 - fuel_V ));

  float fuel_pct =(fuel_ohm - FUEL_FULL) * 100 / (FUEL_EMPTY - FUEL_FULL);
  fuel_pct = 100 - fuel_pct;
  fuel_l = fuel_pct * FUEL_L / 100;


}


/*
 * Read the voltage of each Port
 * The Voltage is used in other functions to calculate diffrent values
 */
void read_ports() {
  //TRACE_PRINTLN(F("#read_ports("));

  float a0_sum = 0;
  float a1_sum = 0;
  float a2_sum = 0;
  float a3_sum = 0;


  //a0_V = analogRead(A0) * A0_MULTIPLICATOR;
  //a1_V = analogRead(A1) * A1_MULTIPLICATOR;
  //a2_V = analogRead(A2) * A2_MULTIPLICATOR;
  //a3_V = analogRead(A3) * A3_MULTIPLICATOR;


  // for port A0 on address 0x01
  #ifdef A0_MULTIPLICATOR
  a0_tmp[IO_ARRAY-1] = analogRead(A0) * A0_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {

    a0_tmp[i] = a0_tmp[i+1];
    a0_sum += a0_tmp[i];
  }
  a0_sum += a0_tmp[IO_ARRAY-1];
  a0_V = a0_sum / IO_ARRAY;

  update_port_value( 0x01, analogRead(A0) * A0_MULTIPLICATOR);
  #endif

  #ifdef A1_MULTIPLICATOR
  // for port A1
  a1_tmp[IO_ARRAY-1] = analogRead(A1) * A1_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {

    a1_tmp[i] = a1_tmp[i+1];
    a1_sum += a1_tmp[i];
  }
  a1_sum += a1_tmp[IO_ARRAY-1];
  a1_V = a1_sum / IO_ARRAY;

  update_port_value( 0x02, analogRead(A1) * A1_MULTIPLICATOR);
  a1_V = analogRead(A1) * A1_MULTIPLICATOR;
  #endif

  #ifdef A2_MULTIPLICATOR
  // for port A2
  a2_tmp[IO_ARRAY-1] = analogRead(A2) * A2_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {

    a2_tmp[i] = a2_tmp[i+1];
    a2_sum += a2_tmp[i];
  }
  a2_sum += a2_tmp[IO_ARRAY-1];
  a2_V = a2_sum / IO_ARRAY;

  update_port_value( 0x03, analogRead(A2) * A2_MULTIPLICATOR);
  #endif

  #ifdef A3_MULTIPLICATOR
  // for port A3
  a3_tmp[IO_ARRAY-1] = analogRead(A3) * A3_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {

    a3_tmp[i] = a3_tmp[i+1];
    a3_sum += a3_tmp[i];
  }
  a3_sum += a3_tmp[IO_ARRAY-1];
  a3_V = a3_sum / IO_ARRAY;

  update_port_value( 0x04, analogRead(A3) * A3_MULTIPLICATOR);
  #endif

  // updatin print_port_values
  update_port_value(0x05, a4_hz);
  update_port_value(0x06, a5_hz);


  #ifdef GPIO13
  //Serial.print("#GPIO13");
  for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
    if ( port_values[i].port == 0x08 ) {
      if ( port_values[i].output ) {
        //Serial.println("#GPIO13 is anoutput port");
        if ( port_values[i].value != 0 ) {
          //Serial.println("#13 on");
          digitalWrite(13, HIGH);
        }
        else {
          digitalWrite(13, LOW);
          //Serial.println("#13 off");
        }
      }
      break;
    }
  }
  #endif

  #ifdef GPIO6
  for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
    if ( port_values[i].port == 0x07 ) {
      if ( port_values[i].output ) {
        if ( port_values[i].value != 0 ) {
          digitalWrite(6, HIGH);
        }
        else {
          digitalWrite(6, LOW);
        }
      }
      break;
    }
  }
  #endif


  message(DEBUG_IO, F("#Ports: "));
  message(DEBUG_IO, String(a0_V));
  message(DEBUG_IO, F(" V, "));
  message(DEBUG_IO, String(a1_V));
  message(DEBUG_IO, F(" V, "));
  message(DEBUG_IO, String(a2_V));
  message(DEBUG_IO, F(" V, "));
  message(DEBUG_IO, String(a3_V));
  message(DEBUG_IO, F(" V, "));
  message(DEBUG_IO, String(a4_hz));
  message(DEBUG_IO, F(" Hz, "));
  message(DEBUG_IO, String(a5_hz));
  message(DEBUG_IO, F(" Hz\n"));
}

void interrupt_A4()
{

  unsigned long diff = micros() - a4_time;
  a4_time = micros();
  a4_hz = 1000000 / diff;


}

void interrupt_A5()
{

  unsigned long diff = micros() - a5_time;
  a5_time = micros();
  a5_hz = 1000000 / diff;

}


void print_port_values() {
  for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
    if ( port_values[i].port == 0x00 ) break;

    message(INFO_MSG, F("0x"));
    if (port_values[i].port<16) {
      message(INFO_MSG ,F("0"));
    }
    message(INFO_MSG , String(port_values[i].port, HEX));
    message(INFO_MSG, F(" = "));
    message(INFO_MSG, String(port_values[i].value, 2));
    message(INFO_MSG , F("\n"));
  }
}

void reg_port(byte port_address, byte type) {

  for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {

    // The array has a fixed size. So find the next unused position.
    if ( port_values[i].port == 0x00 ) {
      port_values[i].port = port_address;
      port_values[i].type = type;
      port_values[i].output = false;


      //message(INFO_MSG, F("Slot: "));
      //message(INFO_MSG, String(i, DEC));
      message(INFO_MSG ,F("#register device 0x"));
      if (port_address<16) {
        message(INFO_MSG ,"0");
      }
      message(INFO_MSG , String(port_values[i].port, HEX));
      message(INFO_MSG , F("\n"));
      break;
    }
  }
}

void set_port_output(byte port_address) {
  for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
    if ( port_values[i].port == port_address ) {
      port_values[i].output = true;
      switch (port_address) {
        case 0x07:
          pinMode(6, OUTPUT);
          digitalWrite(6, LOW);
          break;
        case 0x08:
          pinMode(13, OUTPUT);
          digitalWrite(13, LOW);
          break;
        default:
          message(INFO_MSG , F("#ERROR: port is not usable as output\n"));
          break;
      }
      break;
    }
  }
}

void update_port_value(unsigned int port_address, float port_value) {
  for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
    if ( port_values[i].port == port_address ) {
      port_values[i].value = port_value;
      break;
    }
  }
}

float get_port_value(unsigned int port_address) {
  for (int i = 0; i <= (sizeof(port_values) / sizeof(port_values[0])) - 1; i++) {
    if ( port_values[i].port == port_address ) {
      return port_values[i].value;
      break;
    }
  }
  return 0;
}
