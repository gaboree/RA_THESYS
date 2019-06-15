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

const int stateOut[] = {1, 2, 3, 4, 5, 6, 7, 8};
const int photoIn[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int block_num = 8;
char local_vacancy[block_num] = "FFFFFFFF";
int stateArray[] = {0, 0, 0, 0, 0, 0, 0, 0};
bool first = false;
bool second = true;

const uint16_t node_master = 00;      // master node adress
const uint16_t node_vacancy = 01;     // vacancy control module adress

RF24 radio(10, 9);                  // NRF24L01 (CE,CSN)
RF24Network network(radio);         // include this module in the network

void setup() {
  //initialize radio frequency network to communicate between modules
  SPI.begin();
  radio.begin();
  network.begin(90, node_vacancy);
  radio.setDataRate(RF24_2MBPS);
  //init analog and digital pins as out and input
  for (int i = 0; i < block_num; i++) {
    pinMode(stateOut[i], OUTPUT);
    pinMode(photoIn[i], INPUT);
  }
}

void loop() {
  network.update();

  int inputState[] = {0, 0, 0};
  // read inputs
  for (int i = 0; i < block_num; i++) {
    inputState[i] = analogRead(photoIn[i]);
  }

  // set state leds and vacancy package data
  for ( int i = 0; i < block_num; i++) {
    if (inputState[i] => 300) {
      if ( first == false && second == true) {
        digitalWrite(stateOut[i], HIGH);
        local_vacancy[i] = 'O';
        first = true;
        second = false;
      } else {
        first = false;
        second = true;
        digitalWrite(stateOut[i], LOW);
        local_vacancy[i] = 'F';
      }
    } else {
      if ( second == true) {
        digitalWrite(stateOut[i], LOW);
        local_vacancy[i] = 'F';
      } else {
        digitalWrite(stateOut[i], HIGH);
        local_vacancy[i] = 'O';
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
