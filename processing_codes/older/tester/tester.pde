import controlP5.*; //import ControlP5 library
import processing.serial.*;

Serial port;

ControlP5 cp5; //create ControlP5 object

PFont font;

void setup(){ //same as arduino program

  size(200, 350);    //window size, (width, height)
  
  //printArray(Serial.list());   //prints all available serial ports
  
  //port = new Serial(this, "COM3", 9600);  //i have connected arduino to com3, it would be different in linux and mac os
  
  //lets add buton to empty window
  background(150, 0 , 150);
  cp5 = new ControlP5(this);
  font = createFont("calibri light bold", 15);    // custom fonts for buttons and title
  
  fill(204, 102, 0);
  rect(0,0,200,170);
  fill(0, 255, 0);
  rect(0,170,200,130);
  fill(255, 0, 0);
  rect(0,300,200,50);
  
  cp5.addButton("INDITAS")     //"red" is the name of button
    .setPosition(80, 10)  //x and y coordinates of upper left corner of button
    .setSize(100, 30)      //(width, height)
    .setFont(font)
  ; 

  cp5.addButton("MIN_SEB")     //"yellow" is the name of button
    .setPosition(80, 50)  //x and y coordinates of upper left corner of button
    .setSize(100, 30)      //(width, height)
    .setFont(font)
  ;

  cp5.addButton("MAX_SEB")
    .setPosition(80, 90)
    .setSize(100, 30)
    .setFont(font)
    ;
  
  cp5.addButton("STOP")
    .setPosition(80, 130)
    .setSize(100, 30)
    .setFont(font)
  ;
  


  cp5.addButton("RED")
    .setPosition(80, 180)
    .setSize(100, 30)
    .setFont(font)
  ;
  cp5.addButton("YELLOW")
    .setPosition(80, 220)
    .setSize(100, 30)
    .setFont(font)
    .setLabel("sig_yellow")
  ;
  cp5.addButton("GREEN")
    .setPosition(80, 260)
    .setSize(100, 30)
    .setFont(font)
  ;
  
  cp5.addButton("CHECK")
    .setPosition(80,310)
    .setSize(100,30)
    .setFont(font)
  ;
  
}

void draw(){  //same as loop in arduino

   // background color of window (r, g, b) or (0 to 255)
  
  //lets give title to our window
  fill(0, 0, 0);               //text color (r, g, b)
  textFont(font);
  text("TRA", 10, 20);  // ("text", x coordinate, y coordinat)
  text("SIG",10,190);
  text("VAC",10,320);
}

//lets add some functions to our buttons
//so whe you press any button, it sends perticular char over serial port

void red(){
  port.write('R');
}

void yellow(){
  port.write('Y');
}

void green(){
  port.write('G');
}

void inditas(){
  port.write('f');
}
