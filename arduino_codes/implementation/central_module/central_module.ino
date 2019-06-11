/*
 *  Railway hardware network central control module
 *        == Master Node at adress 00 ==
 *                by Gábor Zsolt
 */

/* include libraries for NRF24L01 
 *  SPI.h for SPI communication via MOSI,MISO, SCK pins
 *  RF24.h for for controlling individual NRF24L01 modules via the CSN and CE pins
 *  RF24Network.h for the NRF24L01 module network
*/

#include <RF24Network.h>
#include <RF24Network_config.h>
#include <Sync.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

// initialize radio network values
int data_size = 8;
RF24 radio(10,9);                   // NRF24L01 (CE,CSN)
RF24Network network(radio);         // include this module in the network
const uint16_t node_acc = 00;      // master node adress
const uint16_t node_vacancy = 01;     // vacancy control module adress
const uint16_t node_signal = 02;    // signal netwrok control adress
const uint16_t node_train_1 = 03;   // train network control adress
const uint16_t node_train_2 = 04;

/*  Signal data initialization
 *  The message contains infromation about the 8 signals and their aspectsa
 *  The 8 characters between represent in units of one a signal and a letter representing the signals aspect
 *	The numeric identifier of a specific signal is represented by the index of the array
 *  Possible aspects: R - RED (also default value)
 *                    Y - YELLOW
 *                    G - GREEN
 *  Number of signals go from 1 to 8.
 *  Example: RRYGRYGR
 *            signal 1 has aspect RED
 *            signal 2 has aspect RED
 *            signal 3 has aspect YELLOW
 *            signal 4 has aspect GREEN
 *            signal 5 has aspect RED
 *            signal 6 has aspect YELLOW
 *            signal 7 has aspect GREEN
 *            signal 8 has aspect RED
 */
char central_signal[data_size] = "DDDDDDDD";

/*  Vacancy data initialization
 *  The message contains infromation about the 8 vacancy elements and their state
 *  The 8 characters represent in units of one a track/block and a letter representing the state
 *	The numeric identifier of a specific block is represented by the index of the array
 *  Possible states: F - FREE (also default value)
 *                   O - OCCUPIED
 *  Number of blocks go from 1 to 8.
 *  Example: FFOFFOFF
 *            block 1 is FREE
 *            block 2 is FREE
 *            block 3 is OCCUPIED
 *            block 4 is FREE
 *            block 5 is FREE
 *            block 6 is OCCUPIED
 *            block 7 is FREE
 *            block 8 is FREE
 */
char central_vacancy[data_size] = "FFFFFFFF";

/* Train data initialization
 * The messeage contains data about the trains present in the network and their speed aspect
 * The character represents the train state and speed
 * Based on layout design at most 2 trains can be present in the rail network at a time
 * Possible states:  N - NOT (also default, means train is not on track)
 *                   S - STOP (train on track and stopped)
 *                   H - HALF (train on track and moves with half speed)
 *                   M - MAX (train on track and moves with max speed)
 */   
char central_train[1]= "N";

void setup() {
	//initialize radio frequency network to communicate between modules
	SPI.begin();
	radio.begin();
	network.begin(90, this_node);
	radio.setDataRate(RF24_2MBPS);	
	//setup Serial Port communication with UI on the control PC, once in loop it is guaranteed that Serial connection is set up, this way no connection is done 
	Serial.begin(9600);
	handShakeWithPC();
}

void loop() {
  // connect to RTF network
  network.update();
  
  //receive data from network
  while(network.available()){
    RF24NetworkHeader master_header;
    char receivedData[8];
    network.read( master_header, &receivedData, sizeof(receivedData));
    //receiving from vacancy module
    if (master_header.from_node == node_vacancy){
      process_vacancy_data(receivedData);
    }
  }
  
  //handle interlocking based on received data
  interlocking(central_train, central_signal, central_vacancy);
  // send new data to network
  send_signal_data();
  send_train_data();
  //update ui about changes
  update_data_to_ui(central_train,'t');
  update_data_to_ui(central_signal,'s');
  update_data_to_ui(central_train,'v');
}

// establish handshake contact with UI platform
void handShakeWithPC(){
	while(Serial.available() <= 0){
		Serial.println("ACC here.");
		delay(300);
	}
}

//process train data received from hardware
void process_train_data(char data[] ){
  for ( int i = 0; i < sizeof(data); i++){
      central_train[i] = data[i];
  }
}

//process signal data received from signal network hardware
void process_signal_data(char data[]){ 
   for ( int i = 0; i < sizeof(data); i++){
      central_signal[i] = data[i];
  } 
}

//process vacancy data received from vacancy network hardware
void process_vacancy_data(char data[]){ 
  for ( int i = 0; i < sizeof(data); i++){
      central_vacancy[i] = data[i];
  }  
}

//send processed interlocking data to UI
void update_data_to_ui(char* data, char type){
  for( int i = 0; i < sizeof(data); i++){
    if (type == 't'){
      Serial.print("train" + data[i]);
    }
    if (type == 's'){
      Serial.print("signal" + data[i]);
    }
    if (type == 'v'){
      Serial.print("vacancy" + data[i]);
    }    
  }
  Serial.println("END");
}

//interlocking caluclations based on received data
void interlocking(char train_data[],char signal_data[],char vacancy_data[]){
 //to be added SIAS algorithm
}

//send updated signal data to signal network hardware
void send_signal_data(){}

//vacancy data does not need to be sent back, however if a failure is detected manual reset of vacancy element must be possible
void send_vacancy_data(){}

//send received data from UI to control train module
void send_train_data(){}
