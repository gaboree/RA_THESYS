
import controlP5.*;

ControlP5 cp5;
float rn_ui_w,rn_ui_h, tn_ui_w,tn_ui_h;
float start_x, start_y;
int num_blocks = 8;
//regardless off monitor size put the program in fullscreen
void settings() {
  fullScreen();
}

void setup() {
  //size(displayWidth, displayHeight);
  background(155);
  noStroke();
  println(displayWidth);
  println(displayHeight);

  //starting point 5% of screen, creates a border
  start_x = displayWidth/3.65;
  start_y = displayWidth/18;
  rn_ui_h = displayHeight*0.8;
  rn_ui_w = rn_ui_h;
  tn_ui_w = rn_ui_w/2;
  
  tn_ui_h = 1;

  smooth();
  strokeWeight(4);
}

void draw() {
  
  // RN-UI space
  fill(20,95,200);
  stroke(0);
  rect(start_x,start_y, rn_ui_w, rn_ui_h);
  
  //draw the track network with 8 blocks
  draw_network(num_blocks);
  
  //static text
  set_track_text();
  
  // TN-UI-1 space
  fill(180);
  stroke(204, 102, 0);
  rect(start_x + rn_ui_h, 400, 150, 150);
  
  //TN-UI-2 space

}

void draw_network(int blocks){
  pushMatrix();
  translate(displayWidth/2, displayHeight/2); 
  noFill();
  ellipse(0, 0, 980, 980);
  for (int i = 0; i < blocks; i++) {
    rotate(radians(360/float(blocks)));
    if(i==0 || i==1)
      fill(255,0,0);
    else
      fill(0,255,0);
    ellipse(490, 0, 70, 70);
  }
  for (int i = 0; i < 2*blocks; i++) {
    rotate(radians(360/float(2*blocks)));    
    if(i%2 ==0){
      line(340, 340, 360, 360);
      
    }
  }
  popMatrix();
}

void set_track_text(){
  fill(0);
  textSize(32);
  text("T1",1525,1035);
  text("T2",1180,1180);
  text("T3",835,1035);
  text("T4",690,690);
  text("T5",835,345);
  text("T6",1180,198);
  text("T7",1525,345);  
  text("T8",1670,690);
}
