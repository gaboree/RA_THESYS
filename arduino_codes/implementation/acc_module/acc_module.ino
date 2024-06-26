/*
    Railway hardware Areal Computer Control module
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

// initialize buffer values
const int data_size = 8;
const int train_num = 2;
char ui_package[2 * data_size];

// initialize radio network values
RF24 radio(9, 10);                  // NRF24L01 (CE,CSN)
RF24Network network(radio);         // include this module in the network
const uint16_t node_acc = 00;      // master node adress
const uint16_t node_vacancy = 01;     // vacancy control module adress
const uint16_t node_signal = 02;    // signal netwrok control adress
const uint16_t node_train_1 = 03;   // train network control adress
const uint16_t node_train_2 = 04;
int fromUI = 0;
const byte numChars = 3;
boolean newData = false;
String speed_serial = "";

/*  Signal data initialization
    The message contains infromation about the 8 signals and their aspectsa
    The 8 characters between represent in units of one a signal and a letter representing the signals aspect
 	The numeric identifier of a specific signal is represented by the index of the array
    Possible aspects: R - RED (also default value)
                      Y - YELLOW
                      G - GREEN
    Number of signals go from 1 to 8.
    Example: RRYGRYGR
              signal 1 has aspect RED
              signal 2 has aspect RED
              signal 3 has aspect YELLOW
              signal 4 has aspect GREEN
              signal 5 has aspect RED
              signal 6 has aspect YELLOW
              signal 7 has aspect GREEN
              signal 8 has aspect RED
*/
char central_signal[data_size] = "RRRRRRRR";

/*  Vacancy data initialization
    The message contains infromation about the 8 vacancy elements and their state
    The 8 characters represent in units of one a track/block and a letter representing the state
 	The numeric identifier of a specific block is represented by the index of the array
    Possible states: F - FREE (also default value)
                     O - OCCUPIED
    Number of blocks go from 1 to 8.
    Example: FFOFFOFF
              block 1 is FREE
              block 2 is FREE
              block 3 is OCCUPIED
              block 4 is FREE
              block 5 is FREE
              block 6 is OCCUPIED
              block 7 is FREE
              block 8 is FREE
*/
char central_vacancy[data_size] = "FFFFFFFF";

/* Train data initialization
   The messeage contains data about the trains present in the network and their speed aspect
   The character represents the train state and speed
   Based on layout design at most 2 trains can be present in the rail network at a time
   Possible states:  N - NOT (also default, means train is not on track)
                     S - STOP (train on track and stopped)
                     H - HALF (train on track and moves with half speed)
                     M - MAX (train on track and moves with max speed)
*/

char central_train[train_num] = "NN";

//setup run at startup once
void setup() {
  //initialize radio frequency network to communicate between modules
  SPI.begin();
  radio.begin();
  network.begin(90, node_acc);
  //setup Serial Port communication with UI on the control PC, once in loop it is guaranteed that Serial connection is set up, this way no connection is done
  Serial.begin(19200);
  establishContact();
}

void loop() {
  // connect to RTF network
  network.update();
  //receive data from network
  while (network.available()) {
    RF24NetworkHeader master_header;
    char receivedData[data_size];
    network.read( master_header, &receivedData, sizeof(receivedData));

    //receiving and load into local buffer information from vacancy module
    if (master_header.from_node == node_vacancy) {
      for (int i = 0; i < sizeof(receivedData); i++)
        central_vacancy[i] = receivedData[i];
    }
  }
  //calculate signal aspect sequencing
  interlocking();

  //Read from Serial port incomming train speed from TN-UI and send update to ui
  recvSerial();
  // send new speed data via RFC to SM
  send_signal_data();
  // send new aspect data via RFC to TM
  send_train_data();
  //wait a litle so all data has time to arrive
  delay(10);
  
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}


void recvSerial() {
  if (Serial.available() > 0) {
    fromUI = Serial.read();
    //handle cases for fromUI
    convert_speed_to_char(fromUI);
    for (int i = 0; i < data_size; i++)
      Serial.write(central_vacancy[i]);
    for (int i = 0; i < data_size; i++)
      Serial.write(central_signal[i]);
  }
}

void convert_speed_to_char(char data) {
  switch (data) {
    case 'B':
      central_train[0] = 'S';
      central_train[1] = 'S';

      break;
    case 'C':
      central_train[0] = 'S';
      central_train[1] = 'H';
      break;
    case 'D':
      central_train[0] = 'S';
      central_train[1] = 'M';
      break;
    case 'E':
      central_train[0] = 'H';
      central_train[1] = 'S';
      break;
    case 'F':
      central_train[0] = 'H';
      central_train[1] = 'H';
      break;
    case 'G':
      central_train[0] = 'H';
      central_train[1] = 'M';
      break;
    case 'H':
      central_train[0] = 'M';
      central_train[1] = 'S';
      break;
    case 'I':
      central_train[0] = 'M';
      central_train[1] = 'H';
      break;
    case 'J':
      central_train[0] = 'M';
      central_train[1] = 'M';
      break;
    default:
      central_train[0] = 'S';
      central_train[1] = 'S';
      break;
  }
}

//interlocking caluclations based on received data
void interlocking() {
  //verify number of trains in network
  int train_num =  count_train();
  //run SIAS algorithm
  sias(train_num);
}

// Signal Aspect Sequencing Algorithm
void sias(int t) {
  //overwrite previous aspects, in each new calculation the old aspects must be disregarded, as a security feature, the algorithm makes calculations on actual data
  for ( int i = 0; i < data_size; i++) {
    central_signal[i] = 'D';
  }
  // value to hold overflow of circular buffer boundry
  int end_ar = 0;
  //loop through vacancy array and set signal aspects according to Occupied blocks
  for ( int i = 0; i < data_size; i++) {
    // since division is used on the iterator, handle case of negative value
    if ( i - 1 < 0)
      end_ar = (i - 1) % data_size + data_size;
    //no conflict case, plenty of distance between blocks
    if (  (central_vacancy[i % data_size] == 'O' && central_vacancy[(i + 1) % data_size] == 'F' && central_vacancy[(i + 2) % data_size] == 'F' && central_vacancy[(i + 3) % data_size] == 'O'  ) ||
          (central_vacancy[i % data_size] == 'O' && central_vacancy[(i + 1) % data_size] == 'F' && central_vacancy[(i + 2) % data_size] == 'F' && central_vacancy[(i + 3) % data_size] == 'F' && central_vacancy[(i + 4) % data_size] == 'O')) {
      central_signal[i % data_size] = 'R';
      central_signal[(i + 1) % data_size] = 'G';
      if ( i - 1 < 0) {
        central_signal[end_ar] = 'Y';
      } else {
        central_signal[(i - 1) % data_size] = 'Y';
      }
      //cout<<"OFFO OFFFO"<<"\n";
      //issue SPEED_MAX command
    }
    // OFO conflict, since upcomming Red or Yellow aspect can not be overwritten
    if (central_vacancy[i % data_size] == 'O' && central_vacancy[(i + 1) % data_size] == 'F' && central_vacancy[(i + 2) % data_size] == 'O' ) {
      central_signal[i % data_size] = 'R';
      central_signal[(i + 1) % data_size] = 'Y';
      central_signal[(i + 2) % data_size] = 'R';
      central_signal[(i + 3) % data_size] = 'G';
      if ( i - 1 < 0) {
        central_signal[end_ar] = 'Y';
      } else {
        central_signal[(i - 1) % data_size] = 'Y';
      }
      //issue SPEED_MIN command
      //cout<<"OFO"<<"\n";
    }
    // OO conflict, adjacent blocks are occupied, the train downstream can't overwrite upstream Red aspect
    if (central_vacancy[i % data_size] == 'O' && central_vacancy[(i + 1) % data_size] == 'O' ) {
      central_signal[i % data_size] = 'R';
      central_signal[(i + 1) % data_size] = 'R';
      central_signal[(i + 2) % data_size] = 'G';
      if ( i - 1 < 0) {
        central_signal[end_ar] = 'Y';
      } else {
        central_signal[(i - 1) % data_size] = 'Y';
      }
      //issue STOP command
      //cout<<"OO"<<"\n";
    }
    // simple case of one train, and case of 4 block distance between two trains
    if ((t == 1 && central_vacancy[i % data_size] == 'O') ||
        (central_vacancy[i % data_size] == 'O' && central_vacancy[(i + 1) % data_size] == 'F' && central_vacancy[(i + 2) % data_size] == 'F' && central_vacancy[(i + 3) % data_size] == 'F' && central_vacancy[(i + 4) % data_size] == 'F' && central_vacancy[(i + 5) % data_size] == 'O')) {
      central_signal[i % data_size] = 'R';
      central_signal[(i + 1) % data_size] = 'G';
      if ( i - 1 < 0) {
        central_signal[end_ar] = 'Y';
      } else {
        central_signal[(i - 1) % data_size] = 'Y';
      }
      // issue SPEED_MAX command
    }
  }
  // since D-default value is not a valid aspect, replace the new signal aspect array D values with Red aspect
  for (int i = 0; i < data_size; i++) {
    if (central_signal[i] == 'D')
      central_signal[i] = 'R';
  }
}

//central_train buffer can be inaccurate, field sensor input is considered more valid, so based on it we calculate the number of trains in the network
int count_train() {
  int val = 0;
  for ( int i = 0; i < data_size; i++) {
    if (central_vacancy[i] == 'O')
      val++;
  }
  return val;
}

//send updated signal data to signal network hardware
void send_signal_data() {
  RF24NetworkHeader header_to_sig(node_signal);
  bool ok_sig = network.write(header_to_sig, &central_signal, sizeof(central_signal));
}

//send received data from UI to control train module
void send_train_data() {
  char speed_t1 = central_train[0];

  RF24NetworkHeader headert1(node_train_1);
  bool ok_t1 = network.write(headert1, &speed_t1, sizeof(speed_t1));
  }
