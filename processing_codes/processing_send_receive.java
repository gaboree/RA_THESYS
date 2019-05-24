    
import processing.serial.*;    // Importing the serial library to communicate with the Arduino 

Serial myPort;      // Initializing a vairable named 'myPort' for serial communication

float background_color ;   // Variable for changing the background color




void setup ( ) {

size (500,  500);     // Size of the serial window, you can increase or decrease as you want

myPort  =  new Serial (this, "COM3",  9600); // Set the com port and the baud rate according to the Arduino IDE

myPort.bufferUntil ( '\n' );   // Receiving the data from the Arduino IDE

} 

void serialEvent  (Serial myPort) {

background_color  =  float (myPort.readStringUntil ( '\n' ) ) ;  // Changing the background color according to received data

} 




void draw ( ) {

background ( 150, 50, background_color );   // Initial background color, when we will open the serial window 

if ( mousePressed  &&  ( mouseButton  ==  LEFT ) ) { // if the left mouse button is pressed

myPort.write ( '1' ) ;       // send a '1' to the Arduino IDE

} 

if  ( mousePressed  &&  ( mouseButton == RIGHT ) ) {  // if the right mouse button is pressed

myPort.write ( '0' ) ;     // Send a '0' to the Arduino IDE

}

}