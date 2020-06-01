/****************************************************
 * FIZ-o-matic
 * https://fiz-o-matic.net/
 *
 *  This sketch is to read the local analog ports.
 *
 * Author: Brun
 * License: Creative Common (CC BY-NC-SA 4.0)
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


  #ifdef DISPLAY_BG_LED
  analogWrite(DISPLAY_BG_LED, DIMMER_MIN);
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

    /**
    #ifdef DISPLAY_BG_LED
    dimmer();
    #endif
    **/

    get_rpm();

    get_clima();

    get_speed();

    get_fuel();


  }

}

void vw_water_temp() {
  //TRACE_PRINTLN(F("#vw_water_temp()"));
  float val=0;
  float temp = 0;

  int array_size = sizeof(vw_temp)/sizeof(vw_temp[0]);

  water_temp = 0;

  val = get_port_value(water_temp_port);

  /*switch (water_temp_port) {
    case 1: val = a0_V; break;
    case 2: val = a1_V; break;
    case 3: val = a2_V; break;
    case 4: val = a3_V; break;
  }*/

  float ohm = SERIESRESISTOR * (val / (10 - val));
  water_temp_ohm = ohm;

  //Serial.print(F("#WATER OHM: "));
  //Serial.println(ohm);

  if ( ohm > vw_temp[0][0] ) {
    water_temp = 0;
  }
  else if ( ohm < vw_temp[array_size-1][0] ) {
    //Serial.println(vw_temp[array_size-1][0]);
    water_temp = 255;
  }
  else {
    for (int i=0; i<array_size;i++) {
      if (ohm > vw_temp[i][0] ) {
        //temp = pgm_read_word_near(&vw_temp[i][1]);
        //temp = pgm_read_word_near(&vw_temp[i-1][1]);

        float temp_ohm = pgm_read_word_near(&vw_temp[i-1][0]) - ohm;
        float ohm_diff = pgm_read_word_near(&vw_temp[i][0]) - pgm_read_word_near(&vw_temp[i-1][0]);
        float temp_diff = pgm_read_word_near(&vw_temp[i-1][1]) - pgm_read_word_near(&vw_temp[i][1]);
        water_temp = int( pgm_read_word_near(&vw_temp[i][1]) + (temp_diff / ohm_diff ) * temp_ohm );

        i=sizeof(vw_temp)/sizeof(vw_temp[0]);
      }
    }
  }

  //Serial.print(F("#WATER TEMP: "));
  //Serial.println(water_temp);

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

void get_clima() {

  if ( temp_out_port != 0 ) {
    temp_out = get_port_value(temp_out_port);
  }
  if ( hum_out_port != 0 ) {
    hum_out = get_port_value(hum_out_port);
  }
  if ( temp_in_port != 0 ) {
    temp_in = get_port_value(temp_in_port);
  }
  if ( hum_in_port != 0 ) {
    hum_in = get_port_value(hum_in_port);
  }

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
  fuel_V = get_port_value(fuel_port);


  float ohm = FUEL_GAUGERESISTOR * (fuel_V  / (10 - fuel_V ));
  fuel_ohm = ohm;

  //message(DEBUG_IO, String(fuel_ohm));
  float fuel_pct =(fuel_ohm - FUEL_FULL) * 100 / (FUEL_EMPTY - FUEL_FULL);
  fuel_pct = 100 - fuel_pct;
  fuel_l = fuel_pct * FUEL_L / 100;
  //fuel_l = fuel_pct;


}
