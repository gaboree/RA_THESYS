int slider1 = 0;
int button1 = 1;
int slider2 = 0;
int button2 = 1;
int slider3 = 0;
int button3 = 1;
int photoCell = 0;

int circleX = 325;
int circleY = 225;
int circleSize = 25;
int circleGray = 200;
int squareX = 325;
int squareY = 225;
int squareSize = 25;
int squareGray = 200;

int buttonPressed = 0;

import processing.serial.*;
Serial usbPort;
int[] sensors = null;
boolean firstContact = false;  


void setup () {
  size(700, 500);
  usbPort = new Serial (this, Serial.list( ) [0], 9600);
  usbPort.bufferUntil ('\n');
}

void draw () {
  if (button1 == 0) {
    buttonPressed = 1;
  }
  if (button2 == 0) {
    buttonPressed = 2;
  }
  background(200);
  if (buttonPressed == 1) {
    dotDrawer(slider1, slider2);
  }  
  if (buttonPressed == 2) {
    squareDrawer(slider1, slider2);
  }
}

void serialEvent (Serial usbPort) {
  String usbString = usbPort.readStringUntil ('\n');
  if (usbString != null) {
    usbString = trim(usbString);
    println(usbString);

    if (firstContact == false) {
      if (usbString.equals("Hello")) {
        usbPort.clear();
        firstContact = true;
        usbPort.write('A');
        println("contact");
      }
    }
    else {
      int sensors[ ] = int(split(usbString, ','));
      for (int sensorNum = 1; sensorNum < sensors.length; sensorNum++) {
        //println(sensorNum + " " + sensors[sensorNum]);
      }

      slider1 = sensors[1];
      button1 = sensors[2];
      slider2 = sensors[3];
      button2 = sensors[4];
      slider3 = sensors[5];
      button3 = sensors[6];
      photoCell = sensors[8];
    }
  }
}

