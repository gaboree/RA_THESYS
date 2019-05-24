// signal network control module, comm with central module

int redPin = 10;
int yellowPin = 9;
int greenPin = 8;


#include <RF24Network.h>
#include <RF24Network_config.h>
#include <Sync.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>


boolean isLedOn = false;
//
char val;
const uint16_t node_signal = 02; 


void setup(){
	SPI.begin();
	radio.begin();
	network.begin(90, this_node);
	pinMode(redPin, OUTPUT);
	pinMode(yellowPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
}

void loop(){
	//receive radio
	network.update();
	while(network.available()){
		RF24NetworkHeader header;
		network.read(header, &val, sizeof(val));
	}
	//execute signal comands based on receive value
	set_all_leds_low(isLedOn);
	switch(val){
		case 'r':
		  if( isLedOn){
			//digitalWrite(redPin, HIGH);
			blink_led(redPin);
			isLedOn = true;
		  }else{
			//digitalWrite(redPin, HIGH);
			blink_led(redPin);
			isLedOn = true;
		  }
		  break;
		case 'y':
		  if( isLedOn){
			blink_led(yellowPin);
			isLedOn = true;
		  }else{
			blink_led(yellowPin);
			isLedOn = true;
		  }
		  break;
		case 'g':
		  if( isLedOn){
			blink_led(greenPin);
			isLedOn = true;
		  }else{
			blink_led(greenPin);
			isLedOn = true;
		  }
		  break;
	}
	
	//send radio comfirmation
	if(isLedOn == false){
		
	}else{
		
	}
}

void set_all_leds_low(boolean val){
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
  val = false;
}

void blink_led(int pin){
  digitalWrite(pin, HIGH);
  delay(150);
  digitalWrite(pin, LOW);
  delay(150);
}