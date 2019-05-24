import processing.serial.*;

Serial myport;
String val;
boolean firstContact;

void setup(){
  size(200,200);
  String portname = Serial.list()[0];
  myport = new Serial(this, portname, 9600);
}

void draw(){
  if (keyPressed == true){
    switch(key){
      case 'r':
        myport.write('r');
        println("r");
        break;
       case 'y':
         myport.write('y');
         println("y");
         break;
       case 'g':
         myport.write('g');
         println("g");
         break;
       default:
         myport.write('0');
         println("0");
         break;
    }
  }
}
