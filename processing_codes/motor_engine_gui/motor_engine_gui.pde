import controlP5.*; //import ControlP5 library
import processing.serial.*;

Serial port;

ControlP5 cp5; //create ControlP5 object
PFont font;

void setup(){ //same as arduino program

  size(300, 450);    //window size, (width, height)
  
  //printArray(Serial.list());   //prints all available serial ports
  
  port = new Serial(this, "COM7", 9600);  //i have connected arduino to com3, it would be different in linux and mac os
  
  //lets add buton to empty window
  
  cp5 = new ControlP5(this);
  font = createFont("calibri light bold", 20);    // custom fonts for buttons and title
  
  cp5.addButton("forward")     //"red" is the name of button
    .setPosition(100, 50)  //x and y coordinates of upper left corner of button
    .setSize(120, 70)      //(width, height)
    .setFont(font)
  ;   

  cp5.addButton("backword")     //"yellow" is the name of button
    .setPosition(100, 150)  //x and y coordinates of upper left corner of button
    .setSize(120, 70)      //(width, height)
    .setFont(font)
  ;

  cp5.addButton("stop")     //"blue" is the name of button
    .setPosition(100, 250)  //x and y coordinates of upper left corner of button
    .setSize(120, 70)      //(width, height)
    .setFont(font)
  ;
  
}

void draw(){  //same as loop in arduino

  background(150, 0 , 150); // background color of window (r, g, b) or (0 to 255)
  
  //lets give title to our window
  fill(0, 255, 0);               //text color (r, g, b)
  textFont(font);
  text("TRAIN CONTROL", 80, 30);  // ("text", x coordinate, y coordinat)
}

//lets add some functions to our buttons
//so whe you press any button, it sends perticular char over serial port

void forward(){
  port.write('f');
  
}

void backward(){
  port.write('b');
}

void stop(){
  port.write('s');
}
