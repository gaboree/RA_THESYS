/*
          Railway hardware Signal module
          == Slave Node at adress 02 ==
                  by Gábor Zsolt
*/

/* include libraries for NRF24L01
    SPI.h for SPI communication via MOSI,MISO, SCK pins
    RF24.h for for controlling individual NRF24L01 modules via the CSN and CE pins
    RF24Network.h for the NRF24L01 module network
*/

//Headers for Radio network
#include <RF24Network.h>
#include <RF24Network_config.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

//Pin declarations to control shift registers
const int s1_clock = 0;
const int s1_latch = 1;
const int s1_data = 2;
const int s2_clock = 3;
const int s2_latch = 4;
const int s2_data = 5;
const int s3_clock = 6;
const int s3_latch = 7;
const int s3_data = 8;

//
RF24 radio(10, 9);                  // NRF24L01 (CE,CSN)
RF24Network network(radio);         // include this module in the network
const uint16_t node_acc = 00;
const uint16_t node_signal = 02;
const int data_size = 8;

// char arrays for data storage and processing
char local_signal[data_size] = "DDDDDDDD";
char full_state_list[3 * data_size];
char register_1_data[data_size] = "00000000";
char register_2_data[data_size] = "00000000";
char register_3_data[data_size] = "00000000";
//bytes used for data shifting
byte aspects_sr_1 = 0;
byte aspects_sr_2 = 0;
byte aspects_sr_3 = 0;

//setup run at startup once
void setup() {
  //initialize radio frequency network to communicate with ACC
  SPI.begin();
  radio.begin();
  network.begin(90, node_signal);
  radio.setDataRate(RF24_2MBPS);
  //init pin mode
  pinMode(s1_clock,OUTPUT);
  pinMode(s1_latch,OUTPUT);
  pinMode(s1_data,OUTPUT);
  pinMode(s2_clock,OUTPUT);
  pinMode(s2_latch,OUTPUT);
  pinMode(s1_data,OUTPUT);
  pinMode(s3_clock,OUTPUT);
  pinMode(s3_latch,OUTPUT);
  pinMode(s3_data,OUTPUT);
}

void loop() {
  // connect to RTF network
  network.update();
  //receive aspect to load from ACC via RFC network
  while (network.available()) {
    RF24NetworkHeader header;
    char incomming_data[data_size];
    network.read( header, &incomming_data, sizeof(incomming_data));
    load_data(incomming_data);
  }
  //Incoming data type is R, Y or G aspect type, convert it to int and put it in a 3xdata_size buffer
  decode_aspects_to_register();
  //split the data in to 3 buffers, one for each shift register
  load_registers_simple();
  //clear data in shift registers
  aspects_sr_1 = 0;
  aspects_sr_2 = 0;
  aspects_sr_3 = 0;
  
  //apply cleared data, allow time for the registers to be loaded
  updateShiftRegisters();
  delay(10);
  
  //apply new data, allow time for thre registers to be loaded
  load_data_to_byte(aspects_sr_1, register_1_data);
  load_data_to_byte(aspects_sr_2, register_2_data);
  load_data_to_byte(aspects_sr_3, register_3_data);
  updateShiftRegisters();
  delay(10);
  
}

//according to the shitregisters int values in the buffer, set the byte to be latched to bitwise 1's and 0's
void load_data_to_byte(byte out, char in[]){
   for (int i = 0; i < 8; i++){
	   if (in[i] == '1')
		   bitSet(out, i);
	   else
		   bitClear(out, i);
   }
}
//bring latch pin to low, put the byte data on the data pin, and pull latch back to high to shift it out
void updateShiftRegisters(){
	digitalWrite(s1_latch, LOW);
	digitalWrite(s2_latch, LOW);
	digitalWrite(s3_latch, LOW);
	shiftOut(s1_data, s1_clock, MSBFIRST, aspects_sr_1);
	shiftOut(s2_data, s2_clock, MSBFIRST, aspects_sr_2);
	shiftOut(s3_data, s3_clock, MSBFIRST, aspects_sr_3);
	digitalWrite(s1_latch, HIGH);
	digitalWrite(s2_latch, HIGH);
	digitalWrite(s3_latch, HIGH);
}


//load in to local buffer data received from ACC
void load_data(char data[]) {
  for (int i = 0; i < sizeof(data); i++)
    local_signal[i] = data[i];
}

//convert RYG aspects to ints
void decode_aspects_to_register() {
  int j = 0;
  for (int i = 0; i < sizeof(local_signal); i++) {
    switch (local_signal[i]) {
      case 'R':
        full_state_list[j] = 1;
        full_state_list[j + 1] = 0;
        full_state_list[j + 2] = 0;
        break;
      case 'Y':
        full_state_list[j] = 0;
        full_state_list[j + 1] = 1;
        full_state_list[j + 2] = 0;
        break;
      case 'G':
        full_state_list[j] = 0;
        full_state_list[j + 1] = 0;
        full_state_list[j + 2] = 1;
        break;
    }
    j = j + 3;
  }
}

//split the converted int values between the 3 registers
void load_registers_simple() {
  for (int i = 0; i < sizeof(full_state_list); i++) {
    if ( i < 8)
      register_1_data[i] = full_state_list[i];
    if ( i > 7 && i < 16)
      register_2_data[i] = full_state_list[i];
    if (i > 15)
      register_3_data[i] = full_state_list[i];
  }
}
