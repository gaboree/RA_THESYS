
//Motor control of train
//declare control pins for H bridge
const int forward1 = 5;
const int forward2 = 4;
const int backward1 = 3;
const int backward2 = 2;
char value;
boolean movement_progress;
unsigned long delay_time = 500;

void setup(){
  //set H bridge pins as outputs
  pinMode (forward1, OUTPUT);
  pinMode (forward2, OUTPUT);
  pinMode (backward1, OUTPUT);
  pinMode (backward2, OUTPUT);
  //setup Serial connection to GUI
  Serial.begin(9600);
}

void loop(){
  //check for serial connection
  if(Serial.available()){
    //read incomming commands from GUI
    value = Serial.read();
    switch(value){
      //if f is received start forward movement
      case 'f':
        //handle direction change from backward to forward in order not to force the motor
        if(movement_progress){
          stop_movement();
          delay(delay_time);
          forward_movement();
        }
        else
          forward_movement();
      break;
      //if b is received start backward movement
      case 'b':
        //handle direction change from forward to backward in order not to force the motor
        if(movement_progress){
          stop_movement();
          delay(delay_time);
          backward_movement();
        }
        else
          backward_movement();
        break;
      //in all other cases stop all movement
      default:
        stop_movement();
    }
  }
}

void backward_movement(){
  analogWrite(backward1, 150);
  digitalWrite (backward2, HIGH);
  digitalWrite (forward1, LOW);
  digitalWrite (forward2, LOW);
  movement_progress = true;
}

void forward_movement(){
  analogWrite(forward1, 150);
  digitalWrite (forward2, HIGH);
  digitalWrite (backward1, LOW);
  digitalWrite (backward2, LOW);
  movement_progress = true;
}

void stop_movement(){
  analogWrite(backward1, LOW);
  digitalWrite (backward2, LOW);
  digitalWrite (forward1, LOW);
  digitalWrite (forward2, LOW);
  movement_progress = false;
}
