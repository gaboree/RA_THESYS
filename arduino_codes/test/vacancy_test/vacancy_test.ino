const int stateOut[] = {3, 4, 5};
const int photoIn[] = {A0, A1, A2};
int stateArray[] = {0, 0, 0};
bool first = false;
bool second = true;
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(stateOut[i], OUTPUT);
    pinMode(photoIn[i], INPUT);
  }
}

void loop() {
  int inputState[] = {0, 0, 0};
  // read inputs
  for (int i = 0; i < 3; i++) {
    inputState[i] = analogRead(photoIn[i]);
  }
  for ( int i = 0; i < 3; i++) {
    if (inputState[i]=> 300) {
		if( first == false && second == true){
			digitalWrite(stateOut[i], HIGH);
			first = true;
			second = false;
		}else{
			first = false;
			second = true;
			digitalWrite(stateOut[i], LOW);
		}
    } else {
		if( second == true)
			digitalWrite(stateOut[i], LOW);
		else
			digitalWrite(stateOut[i], HIGH);
	}
  }
}
