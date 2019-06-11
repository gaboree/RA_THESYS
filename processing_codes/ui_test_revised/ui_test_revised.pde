
import controlP5.*;

ControlP5 cp5;

//dinamic window and size parameters
float rn_ui_w, rn_ui_h, tn_ui_w, tn_ui_h;
float start_x, start_y;
int num_blocks = 8;
float network_size;

//track components
String track_states = "FFOFFOFF";

//signal components
PImage signal_def, signal_grn, signal_red, signal_ylw;
String signal_aspects = "GYRGYRGG";
ArrayList<PVector> signal_positions = new ArrayList<PVector>();

//train control components
Knob speedControlT1;
Knob speedControlT2;


//setting function
void settings() {
  //regardles of monitor size put program in fullscreen
  fullScreen();
}

//init function
void setup() {
  background(155);
  noStroke();
  //dinamic size of dimensions
  start_x = displayWidth/3.65;
  start_y = displayWidth/18;
  rn_ui_h = displayHeight*0.8;
  rn_ui_w = rn_ui_h;
  tn_ui_w = rn_ui_w/2;
  network_size = displayWidth *0.38;
  //preload signal aspects
  signal_def=loadImage("signal_default.jpg");
  signal_grn=loadImage("signal_green.jpg");
  signal_red=loadImage("signal_red.jpg");
  signal_ylw=loadImage("signal_yellow.jpg");
  //setup signal coodinates
  load_signal_positions();
  smooth();
  strokeWeight(4);
  cp5 = new ControlP5(this);
}

//repeating function
void draw() {


  // RN-UI space
  fill(20, 95, 200);
  rect(start_x, start_y, rn_ui_w, rn_ui_h);

  //draw the track network with 8 blocks and state
  draw_network(num_blocks);
  //static text for tracks
  set_track_text();

  //set signals
  set_signal_aspect();
  //static text for signals
  set_signal_text();
  fill(180);
  stroke(80);

  // TN-UI-1 space
  rect(start_x*0.1, start_y, displayWidth*0.24, displayHeight*0.5);  
  fill(0);
  textSize(32);
  text("TRAIN 1", start_x*0.4, start_y*1.3);
  cp5.addButton("T1MED")
    .setValue(100)
    .setPosition(100, 120)
    .setSize(200, 19)
    ;
  cp5.addButton("T1MAX")
    .setValue(100)
    .setPosition(100, 120)
    .setSize(200, 19)
    ;
  cp5.addButton("T1STOP")
    .setValue(100)
    .setPosition(100, 120)
    .setSize(200, 19)
    ;
  //TN-UI-2 space
  fill(180);
  stroke(80);
  rect(start_x*2.665, start_y, displayWidth*0.24, displayHeight*0.5);
  fill(0);
  textSize(32);
  text("TRAIN 2", start_x*2.95, start_y*1.3);
}


/*
* Implementation of functions
 */
void draw_network(int blocks) {
  pushMatrix();
  translate(displayWidth/2, displayHeight/2); 
  noFill();
  stroke(0);
  ellipse(0, 0, network_size, network_size);
  for (int i = 0; i < blocks; i++) {
    rotate(radians(360/float(blocks)));
    switch(track_states.charAt(i)) {
    case 'O':
      fill(255, 0, 0);
      break;
    case 'F':
      fill(0, 255, 0);
      break;
    default:
      fill(0);
    }
    ellipse(network_size/2, 0, 70, 70);
  }
  for (int i = 0; i < 2*blocks; i++) {
    rotate(radians(360/float(2*blocks)));    
    if (i%2 ==0) {
      line(network_size*0.34, network_size*0.34, network_size*0.36, network_size*0.36);
    }
  }
  popMatrix();
}


void load_signal_positions() {
  PVector temp;
  temp = new PVector(network_size*1.61, network_size*0.89);
  signal_positions.add(0, temp);
  temp = null;
  temp = new PVector(network_size*1.42, network_size*1.1);
  signal_positions.add(1, temp);
  temp = null;
  temp = new PVector(network_size*1.10, network_size*1.1);
  signal_positions.add(2, temp);
  temp = null;
  temp = new PVector(network_size*0.9, network_size*0.89);
  signal_positions.add(3, temp);
  temp = null;
  temp = new PVector(network_size*0.9, network_size*0.55);
  signal_positions.add(4, temp);
  temp = null;
  temp = new PVector(network_size*1.10, network_size*0.34);
  signal_positions.add(5, temp);
  temp = null;
  temp = new PVector(network_size*1.42, network_size*0.34);
  signal_positions.add(6, temp);
  temp = null;
  temp = new PVector(network_size*1.61, network_size*0.55);
  signal_positions.add(7, temp);
  temp = null;
}


void set_signal_aspect() {
  for (int i = 0; i< signal_aspects.length(); i++) {
    switch(signal_aspects.charAt(i)) {
    case 'R':
      image(signal_red, signal_positions.get(i).x, signal_positions.get(i).y);
      break;
    case 'Y':
      image(signal_ylw, signal_positions.get(i).x, signal_positions.get(i).y); 
      break;
    case 'G':
      image(signal_grn, signal_positions.get(i).x, signal_positions.get(i).y);
      break;
    default:
      image(signal_def, signal_positions.get(i).x, signal_positions.get(i).y);
    }
  }
}


void set_track_text() {
  fill(0);
  textSize(32);
  text("T1", network_size*1.64, network_size*1.11);
  text("T2", network_size*1.29, network_size*1.26);
  text("T3", network_size*0.93, network_size*1.11);
  text("T4", network_size*0.79, network_size*0.76);
  text("T5", network_size*0.93, network_size*0.405);
  text("T6", network_size*1.29, network_size*0.26);
  text("T7", network_size*1.64, network_size*0.405);  
  text("T8", network_size*1.785, network_size*0.76);
}

void set_signal_text() {
  fill(0);
  textSize(24);
  text("SIG1", network_size*1.63, network_size*0.88);
  text("SIG2", network_size*1.44, network_size*1.09);
  text("SIG3", network_size*1.12, network_size*1.09);
  text("SIG4", network_size*0.92, network_size*0.88);
  text("SIG5", network_size*0.92, network_size*0.54);
  text("SIG6", network_size*1.12, network_size*0.33);
  text("SIG7", network_size*1.44, network_size*0.33);
  text("SIG8", network_size*1.63, network_size*0.54);
}

void knob(int theValue) {
  println("a knob event. setting background to "+theValue);
}
