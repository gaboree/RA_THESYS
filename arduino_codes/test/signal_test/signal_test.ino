int latchPin = 5;  // Latch pin of 74HC595 is connected to Digital pin 5
int clockPin = 6; // Clock pin of 74HC595 is connected to Digital pin 6
int dataPin = 4;  // Data pin of 74HC595 is connected to Digital pin 4
int led_State = 7;
byte leds = 0;    // Variable to hold the pattern of which LEDs are currently turned on or off
char register_1_data[] = "00101010";

/*
   setup() - this function runs once when you turn your Arduino on
   We initialize the serial connection with the computer
*/

void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop()
{
  leds = 0;
  updateShiftRegister();
  delay(500);
  for (int i = 0; i < 8; i++)
  {
    if (register_1_data[i] == '1')
      bitSet(leds, i);
    else
      bitClear(leds, i);
  }
  updateShiftRegister();
  delay(500);
}

void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}
