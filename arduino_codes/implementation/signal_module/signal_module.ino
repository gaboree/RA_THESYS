#include <RF24Network.h>
#include <RF24Network_config.h>
#include <Sync.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

RF24 radio(10, 9);                  // NRF24L01 (CE,CSN)
RF24Network network(radio);         // include this module in the network
const uint16_t node_acc = 00;
const uint16_t node_signal = 02;
const int data_size = 8;

char local_signal[data_size] = "DDDDDDDD";
char full_state_list[3 * data_size];
char register_1_data[data_size] = "00000000";
char register_2_data[data_size] = "00000000";
char register_3_data[data_size] = "00000000";
void setup() {
  // put your setup code here, to run once:
  //initialize radio frequency network to communicate between modules
  SPI.begin();
  radio.begin();
  network.begin(90, node_signal);
  radio.setDataRate(RF24_2MBPS);

}

void loop() {
  // put your main code here, to run repeatedly:
  // connect to RTF network
  network.update();
  //receive data from network
  while (network.available()) {
    RF24NetworkHeader header;
    char incomming_data[data_size];
    network.read( header, &incomming_data, sizeof(incomming_data));
    load_data(incomming_data);
  }

  decode_aspects_to_register();
  load_registers_simple();
}

void load_data(char data[]) {
  for (int i = 0; i < sizeof(data); i++)
    local_signal[i] = data[i];
}

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
