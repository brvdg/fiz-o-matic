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
  float a4_sum = 0;
  float a5_sum = 0;


  //a0_V = analogRead(A0) * A0_MULTIPLICATOR;
  //a1_V = analogRead(A1) * A1_MULTIPLICATOR;
  //a2_V = analogRead(A2) * A2_MULTIPLICATOR;
  //a3_V = analogRead(A3) * A3_MULTIPLICATOR;

  //message(DEBUG_IO, F("#Ports: "));

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

  //message(DEBUG_IO, String(a0_V));
  //message(DEBUG_IO, F(" V, "));

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

  //message(DEBUG_IO, String(a1_V));
  //message(DEBUG_IO, F(" V, "));

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

  //message(DEBUG_IO, String(a2_V));
  //message(DEBUG_IO, F(" V, "));

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

  //message(DEBUG_IO, String(a3_V));
  //message(DEBUG_IO, F(" V, "));

  #endif

  #ifdef A4_MULTIPLICATOR
  // for port A3
  a4_tmp[IO_ARRAY-1] = analogRead(A4) * A4_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {

    a4_tmp[i] = a4_tmp[i+1];
    a4_sum += a4_tmp[i];
  }
  a4_sum += a4_tmp[IO_ARRAY-1];
  a4_V = a4_sum / IO_ARRAY;

  update_port_value( 0x05, analogRead(A4) * A4_MULTIPLICATOR);

  //message(DEBUG_IO, String(a4_V));
  //message(DEBUG_IO, F(" V, "));

  #endif

  #ifdef A5_MULTIPLICATOR
  // for port A3
  a5_tmp[IO_ARRAY-1] = analogRead(A5) * A5_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {

    a5_tmp[i] = a5_tmp[i+1];
    a5_sum += a5_tmp[i];
  }
  a5_sum += a5_tmp[IO_ARRAY-1];
  a5_V = a5_sum / IO_ARRAY;

  update_port_value( 0x06, analogRead(A5) * A5_MULTIPLICATOR);

  //message(DEBUG_IO, String(a5_V));
  //message(DEBUG_IO, F(" V, "));

  #endif


  // updatin print_port_values
  #ifdef A4_COUNTER
  update_port_value(0x05, a4_hz);
  //message(DEBUG_IO, String(a4_hz));
  //message(DEBUG_IO, F(" Hz, "));
  #endif

  #ifdef A5_COUNTER
  update_port_value(0x06, a5_hz);
  //message(DEBUG_IO, String(a5_hz));
  //message(DEBUG_IO, F(" Hz\n"));
  #endif


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
      String msg = F("register device 0x");
      //message(INFO_MSG ,F("#register device 0x"));
      if (port_address<16) {
        //message(INFO_MSG ,"0");
        msg += F("0");
      }
      msg += String(port_values[i].port, HEX);
      //message(INFO_MSG , String(port_values[i].port, HEX));
      //message(INFO_MSG , F("\n"));
      message(INFO_MSG , msg);
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
