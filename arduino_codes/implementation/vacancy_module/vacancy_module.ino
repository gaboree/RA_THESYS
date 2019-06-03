const int ledPinStatus = 8;
const int photorPin = A0;

void setup(){
  Serial.begin(9600);
  pinMode(ledPinStatus, OUTPUT);
  pinMode(photorPin, INPUT);
}

void loop() {
  int ldrStatus = analogRead(photorPin);
  if (ldrStatus <= 300) {
    digitalWrite(ledPinStatus, LOW);
    Serial.print("Its DARK, Turn on the LED : ");
    Serial.println(ldrStatus);
  }else{
    digitalWrite(ledPinStatus, HIGH);
    Serial.print("Its BRIGHT, Turn off the LED : ");
    Serial.println(ldrStatus);
  }
}
