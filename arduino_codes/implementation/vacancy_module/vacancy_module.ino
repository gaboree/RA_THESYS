/*
    Railway hardware network central control module
          == Master Node at adress 00 ==
                  by Gábor Zsolt
*/

/* include libraries for NRF24L01
    SPI.h for SPI communication via MOSI,MISO, SCK pins
    RF24.h for for controlling individual NRF24L01 modules via the CSN and CE pins
    RF24Network.h for the NRF24L01 module network
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

const int stateOut[] = {8, 7, 6, 5, 4, 3, 2, 1};
const int photoIn[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int block_num = 8;
char local_vacancy[block_num] = "FFFFFFFF";
int stateArray[] = {0, 0, 0, 0, 0, 0, 0, 0};
bool first = false;

const uint16_t node_master = 00;      // master node adress
const uint16_t node_vacancy = 01;     // vacancy control module adress

RF24 radio(9, 10);                  // NRF24L01 (CE,CSN)
RF24Network network(radio);         // include this module in the network

void setup() {
  //initialize radio frequency network to communicate between modules
  SPI.begin();
  radio.begin();
  network.begin(90, node_vacancy);
  //init analog and digital pins as out and input
  for (int i = 0; i < block_num; i++) {
    pinMode(stateOut[i], OUTPUT);
    pinMode(photoIn[i], INPUT);
  }
  
}

void loop() {
  network.update();
  int inputState[] = {0, 0, 0, 0, 0, 0, 0, 0};
  // read inputs
  for (int i = 0; i < block_num; i++) {
    inputState[i] = analogRead(photoIn[i]);
  }
  // set state leds and vacancy package data
  /*
    for ( int i = 0; i < block_num; i++) {
    int end_pos = 0;
    if ( (i - 1) < 0)
      end_pos = (i - 1) % block_num + block_num;
    if (inputState[i] >= 300 && local_vacancy[i] == 'F') {
      digitalWrite(stateOut[i], HIGH);
      Serial.println(i);
      local_vacancy[i] = 'O';
      if ( (i - 1) < 0) {
        if (local_vacancy[end_pos] == 'O') {
          digitalWrite(stateOut[end_pos], LOW);
          local_vacancy[end_pos] = 'F';
        }
      } else {
        if (local_vacancy[i - 1] == 'O') {
          digitalWrite(stateOut[i - 1], LOW);
          local_vacancy[i - 1] = 'F';
        }
      }
    }

    }

    `*/

  int end_pos = 0;
  for ( int i = 0; i < block_num; i++) {
    if ( i - 1 < 0)
      end_pos = (i - 1) % block_num + block_num;
    if ((inputState[i % block_num] > 300) && local_vacancy[i % block_num] == 'F') {
      digitalWrite(stateOut[i % block_num], HIGH);
      local_vacancy[i % block_num] = 'O';
      if ( i - 1 < 0) {
        if ( local_vacancy[end_pos] == 'O') {
          local_vacancy[end_pos] = 'F';
          digitalWrite(stateOut[end_pos], LOW);
        }
      } else {
        if ( local_vacancy[(i - 1) % block_num] == 'O') {
          local_vacancy[(i - 1) % block_num] = 'F';
          digitalWrite(stateOut[(i - 1) % block_num], LOW);
        }
      }
    }
  }

  //send vacancy data to ACC
  send_train_data();
  delay(10);

}


void send_train_data() {
  RF24NetworkHeader headert1(node_master);
  bool ok_vac = network.write(headert1, &local_vacancy, sizeof(local_vacancy));
  if (ok_vac) {
    //Serial.println("Data sent");
  }
}
