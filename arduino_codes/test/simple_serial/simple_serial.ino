char central_train[2] = "SS";
String central_vacancy = "FFFFFFFFRRRRRRRR";
String speed_serial= "";
int train1_stop = 3;
int train1_half = 4;
int train1_max = 5;
int train2_stop = 6;
int train2_half = 7;
int train2_max = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(train1_stop, OUTPUT);
  pinMode(train1_half, OUTPUT);
  pinMode(train1_max, OUTPUT);
  pinMode(train2_stop, OUTPUT);
  pinMode(train2_half, OUTPUT);
  pinMode(train2_max, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 2) {

      speed_serial = speed_serial + char( Serial.read());
    
    central_train[0] = speed_serial.charAt(0);

    central_train[1] = speed_serial.charAt(1);
  

  if (central_train[0] == 'S') {
    digitalWrite(train1_stop, HIGH);
    digitalWrite(train1_half, LOW);
    digitalWrite(train1_max, LOW);
  }
  if (central_train[0] == 'H'){
    digitalWrite(train1_stop, LOW);
    digitalWrite(train1_half, HIGH);
    digitalWrite(train1_max, LOW);
  }
  if (central_train[0] == 'M'){
    digitalWrite(train1_stop, LOW);
    digitalWrite(train1_half, LOW);
    digitalWrite(train1_max, HIGH);
  }{
    digitalWrite(train2_stop, HIGH);
    digitalWrite(train2_half, LOW);
    digitalWrite(train2_max, LOW);
  }
  if (central_train[1] == 'H'){
    digitalWrite(train2_stop, LOW);
    digitalWrite(train2_half, HIGH);
    digitalWrite(train2_max, LOW);
  }
  if (central_train[1] == 'M'){
    digitalWrite(train2_stop, LOW);
    digitalWrite(train2_half, LOW);
    digitalWrite(train2_max, HIGH);
  }
  }
  Serial.println(central_vacancy);


}
