// signal network control module, comm with central module

int redPin = 10;
int yellowPin = 9;
int greenPin = 8;

boolean isLedOn = false;
//
char val = ;

void setup(){
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop(){
	//receive radio
	
	//execute signal comands based on receive value
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