
import controlP5.*;
import processing.serial.*;

//CP5 object for buttons
ControlP5 cp5;

//dinamic window and size parameters
float rn_ui_w, rn_ui_h, tn_ui_w, tn_ui_h;
float start_x, start_y;
int num_blocks = 8;
float network_size;

//track components
//String track_states = "FFOFFOFF";

//signal components
PImage signal_def, signal_grn, signal_red, signal_ylw, acc_disc_msg, acc_con_msg;
//String signal_aspects = "GYRGYRGG";
ArrayList<PVector> signal_positions = new ArrayList<PVector>();

//serial communication with ACC and data containers
Serial comPort;
String received_data_from_acc = null;
boolean connected_to_acc = false;
char train_1_state = 'S';
char train_2_state = 'S';
char[] track_states = {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'};
char[] signal_aspects ={'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'};
boolean serial_state;

RadioButton r1, r2;

//Font for text
PFont pfont = new PFont();
String send_train_speeds;

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
  start_x = displayWidth*0.273;
  start_y = displayWidth*0.0555;
  rn_ui_h = displayHeight*0.8;
  rn_ui_w = rn_ui_h;
  tn_ui_w = rn_ui_w/2;
  network_size = displayWidth *0.38;
  pfont = createFont("Calibri Bold", 32);
  //preload signal aspects and acc connection states
  signal_def=loadImage("signal_default.jpg");
  signal_grn=loadImage("signal_green.jpg");
  signal_red=loadImage("signal_red.jpg");
  signal_ylw=loadImage("signal_yellow.jpg");
  acc_con_msg = loadImage("acc_connected.png");
  acc_disc_msg = loadImage("acc_disconected.png");
  //setup signal coodinates
  load_signal_positions();
  //add train control buttons
  cp5 = new ControlP5(this);
  add_train_control_buttons();
  //attempt serial connection
  attemptSerial();
  delay(1000);
}





//repeating function
void draw() {
  if (serial_state) {
    // serial is up and running

    try {
      // nothing here serial event function handles incomming data...
    } 
    catch (RuntimeException e) {
      serial_state = false;
    }
  } else {
    attemptSerial();
  }
  smooth();
  strokeWeight(4);
  load_default_platform();

  //draw the track network with 8 blocks and state
  draw_network(num_blocks);
  //static text for tracks
  set_track_text();
  //set signals with aspect
  set_signal_aspect();
  //static text for signals
  set_signal_text();
  set_train_status_text();
}


/*
* Implementation of functions
 */
void attemptSerial() {
  try {
    comPort = new Serial(this, Serial.list()[1], 9600);
    comPort.bufferUntil('\n');
    serial_state = true;

    image(acc_con_msg, start_x, start_y*9.2);
  }
  catch(Exception e) {
    e.printStackTrace();
    serial_state = false;
    image(acc_disc_msg, start_x, start_y*9.2);
  }
}

void set_train_status_text() {
  if (train_1_state == 'N') {
    r1.hide();
    text("Train 1 not detected", start_x*0.26, start_y*2);
  } else
    r1.show(); 
  if (train_2_state == 'N') {
    r2.hide();
    text("Train 2 not detected", start_x*2.81, start_y*2);
  } else
    r2.show();
}

void serialEvent(Serial comPort) {
  String inString;
  if (comPort.available()>0) {
    //while ( comPort.available() > 0) {
      inString = comPort.readStringUntil('\n');
      track_states[0] = inString.charAt(0);
      track_states[1] = inString.charAt(1);
      track_states[2] = inString.charAt(2);
      track_states[3] = inString.charAt(3);
      track_states[4] = inString.charAt(4);
      track_states[5] = inString.charAt(5);
      track_states[6] = inString.charAt(6);
      track_states[7] = inString.charAt(7);
      signal_aspects[0] = inString.charAt(8);
      signal_aspects[1] = inString.charAt(9);
      signal_aspects[2] = inString.charAt(10);
      signal_aspects[3] = inString.charAt(11);
      signal_aspects[4] = inString.charAt(12);
      signal_aspects[5] = inString.charAt(13);
      signal_aspects[6] = inString.charAt(14);
      signal_aspects[7] = inString.charAt(15);
      //comPort.clear();
      //comPort.stop();
    //}
  } 
  comPort.clear();
  //
  for (int i =0; i < 8; i++)
    print(track_states[i]);
  println();
}



void draw_network(int blocks) {
  pushMatrix();
  translate(displayWidth/2, displayHeight/2); 
  noFill();
  stroke(0);
  ellipse(0, 0, network_size, network_size);
  for (int i = 0; i < blocks; i++) {
    rotate(radians(360/float(blocks)));
    //println(track_states[i]);
    switch(track_states[i]) {
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

  for (int i = 0; i< signal_aspects.length; i++) {
    switch(signal_aspects[i]) {
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


void load_default_platform() {
  // RN-UI space
  fill(20, 95, 200);
  rect(start_x, start_y, rn_ui_w, rn_ui_h);  
  fill(180);
  stroke(80);
  // TN-UI-1 space
  rect(start_x*0.1, start_y, displayWidth*0.24, displayHeight*0.5); 
  fill(0);
  textSize(32);
  text("TRAIN 1", start_x*0.4, start_y*1.6);
  //TN-UI-2 space
  fill(180);
  stroke(80);
  rect(start_x*2.665, start_y, displayWidth*0.24, displayHeight*0.5);
  fill(255);
  textSize(32);
  text("TRAIN 2", start_x*2.95, start_y*1.6);
}

void add_train_control_buttons() {

  r1 = cp5.addRadioButton("trainControl1")
    .setPosition(start_x*0.26, start_y*2)
    .setItemWidth(Math.round(displayWidth*0.08))
    .setItemHeight(Math.round(displayHeight*0.1))
    .addItem("    V1 Max", 0)
    .addItem("    V1 MED", 1)
    .addItem("    T1 STOP", 2)
    .setColorLabel(color(0))
    .setColorForeground(color(120))
    .setColorActive(color(255, 165, 0))
    .setColorLabel(color(255))
    .activate(2);
  r2 = cp5.addRadioButton("trainControl2")
    .setPosition(start_x*2.81, start_y*2)
    .setItemWidth(Math.round(displayWidth*0.08))
    .setItemHeight(Math.round(displayHeight*0.1))
    .addItem("    V2 Max", 0)
    .addItem("    V2 MED", 1)
    .addItem("    T2 STOP", 2)
    .setColorLabel(color(0))
    .setColorForeground(color(120))
    .setColorActive(color(255, 165, 0))
    .setColorLabel(color(255))
    .activate(2);

  for (Toggle t : r1.getItems()) {
    t.getCaptionLabel().setColorBackground(color(80));
    t.getCaptionLabel().getStyle().setMarginTop(-38);
    t.getCaptionLabel().getStyle().backgroundWidth = 170;
    t.getCaptionLabel().getStyle().backgroundHeight = 108;
    t.getCaptionLabel().setFont(pfont);
  }
  for (Toggle t : r2.getItems()) {
    t.getCaptionLabel().setColorBackground(color(80));
    t.getCaptionLabel().getStyle().setMarginTop(-38);
    t.getCaptionLabel().getStyle().backgroundWidth = 170;
    t.getCaptionLabel().getStyle().backgroundHeight = 108;
    t.getCaptionLabel().setFont(pfont);
  }
}



public void trainControl1(int val) {
  switch(val) {
    case(0):
    println("T1 MAX");
    train_1_state = 'M';
    break;
    case(1):
    println("T1 MED");
    train_1_state = 'H';
    break;
    case(2):
    train_1_state = 'S';
    println("T1 STO");
    break;
  }
  send_train_speeds = ""+train_1_state+train_2_state+'\n';
  comPort.write(send_train_speeds);
}

public void trainControl2(int val) {
  switch(val) {
    case(0):
    train_2_state = 'M';  
    break;
    case(1):
    train_2_state = 'H';
    break;
    case(2):
    train_2_state = 'S';
    break;
  }

  send_train_speeds = ""+train_1_state+train_2_state+'\n';
  //comPort.write(send_train_speeds);
}
