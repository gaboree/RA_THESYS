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

//define  macros for communication protocol
#define SIGNAL $
#define VACANCY #
#define TRAIN %
#define DELIMITER

// initialize radio network values
int network_size = 10;
RF24 radio(10,9);                   // NRF24L01 (CE,CSN)
RF24Network network(radio);         // include this module in the network
const uint16_t this_node = 00;      // master node adress
const uint16_t node_train = 01;     // train control module adress
const uint16_t node_signal = 02;    // signal netwrok control adress
const uint16_t node_vacancy = 03;   // vacancy network control adress

/*  Signal data initialization
 *  The message contains infromation about the 8 signals and their aspects
 *  "<" character marks the begining of the message
 *  ">" character marks the end of the message
 *  The 16 characters between represent in units of two a signal number and letter representing the signals aspect
 *  Possible aspects: R - RED (also default value)
 *                    Y - YELLOW
 *                    G - GREEN
 *  Number of signals go from 1 to 8.
 *  Example: <1R2R3Y4G5R6Y7G8R>
 *            signal 1 has aspect RED
 *            signal 2 has aspect RED
 *            signal 3 has aspect YELLOW
 *            signal 4 has aspect GREEN
 *            signal 5 has aspect RED
 *            signal 6 has aspect YELLOW
 *            signal 7 has aspect GREEN
 *            signal 8 has aspect RED
 */
char central_signal[network_size] = "<DDDDDDDD>";

/*  Vacancy data initialization
 *  The message contains infromation about the 8 vacancy elements and their state
 *  "*" character marks the begining of the message
 *  "#" character marks the end of the message
 *  The 16 characters between represent in units of two a track number and a letter representing the state
 *  Possible states: F - FREE (also default value)
 *                   O - OCCUPIED
 *  Number of tracks go from 1 to 8.
 *  Example: *1F2F3O4F5F6O7F8F#
 *            track 1 is FREE
 *            track 2 is FREE
 *            track 3 is OCCUPIED
 *            track 4 is FREE
 *            track 5 is FREE
 *            track 6 is OCCUPIED
 *            track 7 is FREE
 *            track 8 is FREE
 */
char central_vacancy[network_size] = "<FFFFFFFF>";

/* Train data initialization
 * The messeage contains data about the trains present in the network and their speed aspect
 * The 4 characters between represent in units of two a track number and a letter representing train state and speed
 * Based on layout design at most 2 trains can be present in the rail network at a time
 * Possible states:  N - NOT (also default, means train is not on track)
 *                   S - STOP (train on track and stopped)
 *                   H - HALF (train on track and moves with half speed)
 *                   M - MAX (train on track and moves with max speed)
 */   
char central_train[]= "NN";

void setup() {
	//initialize radio frequency network to communicate between modules
	SPI.begin();
	radio.begin();
	network.begin(90, this_node);
	radio.setDataRate(RF24_2MBPS);
	//send in network initial values
	
	//setup Serial Port communication with UI on the control PC, once in loop it is guaranteed that Serial connection is set up, this way no connection is done 
	Serial.begin(9600);
	handShakeWithPC();
}

void loop() {
  // connect to radio communication network
  network.update();
  
  //receive data from network
  while(network.available()){
    RF24NetworkHeader master_header;
    char receivedData[8];
    network.read( master_header, &receivedData, sizeof(receivedData));
    // receiving from train module
    if (master_header.from_node == 01){
      process_train_data(receivedData);
    }
    //receiving from signal module
    if (master_header.from_node == 02){
      process_signal_data(receivedData);
    }
    //receiving from vacancy module
    if (master_header.from_node == 03){
      process_vacancy_data(receivedData);
    }
  }
  
  //handle interlocking based on received data
  interlocking(central_train, central_signal, central_vacancy);
  
  //update ui about changes
  update_data_to_ui(central_train,'t');
  update_data_to_ui(central_signal,'s');
  update_data_to_ui(central_train,'v');
  
  // send new data to network
  send_signal_data();
  send_vacancy_data();
  send_train_data();
}

// establish handshake contact with UI platform
void handShakeWithPC(){
	while(Serial.available() <= 0){
		Serial.println("Central module here.");
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
