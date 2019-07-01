/*
   	Railway hardware train control module
          == Slave Node at adress 01 ==
                  by Gábor Zsolt
*/

/* include libraries for NRF24L01
    SPI.h for SPI communication via MOSI,MISO, SCK pins
    RF24.h for for controlling individual NRF24L01 modules via the CSN and CE pins
    RF24Network.h for the NRF24L01 module network
*/

#include <RF24Network.h>
#include <RF24Network_config.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

const uint16_t master_node = 00;	// master node adress
const uint16_t node_train = 03;		// train control module adress
RF24 radio(9, 10);					// NRF24L01 (CE,CSN)
RF24Network network(radio);			// include this module in the network
int controlPWMPin = 6;				// PWM pin of arduino to control speed
int controlTICPin = 8;
char previous_speed = 'N';			// default speed mode
int ack_speed = 0;

void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, node_train);
  radio.setDataRate(RF24_2MBPS);
  pinMode(controlPWMPin, OUTPUT);
  pinMode(controlTICPin, OUTPUT);
}

void loop() {
  network.update();
  //===== Receiving =====//
  while (network.available()) {
    RF24NetworkHeader master_header;
    char receivedData;
    network.read( master_header, &receivedData, sizeof(receivedData));
    // receiving from central module
    if (master_header.from_node == 00) {
      if (receivedData !=  previous_speed) {
        ack_speed = control_train_speed(receivedData);
      }
    }
  }
  if (ack_speed)
    digitalWrite(controlTICPin, HIGH);

}

/* Received train speed
   The messeage contains data about the trains present in the network and their speed aspect
   The 4 characters between represent in units of two a track number and a letter representing train state and speed
   Based on layout design at most 2 trains can be present in the rail network at a time
   Possible states:  N - NOT (also default, means train is not on track)
                     S - STOP (train on track and stopped)
                     H - HALF (train on track and moves with half speed)
                     M - MAX (train on track and moves with max speed)
*/

int control_train_speed(char data[]) {
  // range of PWM interval is from 0 (min) to 255 (max)
  int ack = 0;
  if ( data[0] == 'S' ) {
    analogWrite(controlPWMPin, 0);
    ack = 1;
  }
  if ( data[0] == 'H' ) {
    analogWrite(controlPWMPin, 115);
    ack = 1;
  }
  if ( data[0] == 'M' ) {
    analogWrite(controlPWMPin, 205);
    ack = 1;
  }
  previous_speed = data[0];
  return ack;
}
