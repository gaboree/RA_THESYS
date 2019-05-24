// central module comm with signal network


#include <RF24Network.h>
#include <RF24Network_config.h>
#include <Sync.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

const uint16_t this_node = 00; 
const uint16_t node_signal = 02; 

char val;

void setup(){
	SPI.begin();
	radio.begin();
	network.begin(90, this_node);
	radio.setDataRate(RF24_2MBPS);
	Serial.begin(9600);
}

void loop() {
  if (Serial.available()){
    val = Serial.read();
  }
  network.update();
  RF24NetworkHeader header_signal(node_signal);
  network.write(header_signal, &val, sizeof(val));
  delay(10);
}


