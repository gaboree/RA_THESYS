PImage signal_def, signal_gr,track_vertical_free;
boolean signal_flag=true;

int myColor = color(0);
int counter;
int s = second();

void setup() {
  //fullScreen();
  size(640,480);
  signal_def=loadImage("signal_default.jpg");
  signal_gr=loadImage("signal_green.jpg");
  //track_vertical_free=loadImage("track_vertical_free.jpg");

}

void draw(){
  background(73,73,73);
  
  blink_element(signal_flag, signal_def, signal_gr, 150, 150);
  signal_flag =! signal_flag;
/*
  if(signal_flag){
      image(signal_def,150,150);
    signal_flag = false;
  }else{
     image(signal_gr,150,150);
     signal_flag = true;
  }
*/
  //image(track_vertical_free,100,250);
  textSize(28);
  fill(0,0,255);
  text("TRACK_00",180,390);
  delay(500);

}

void blink_element(boolean blink_flag, PImage passive_state, PImage active_state, int x, int y){
  if(blink_flag){
    image(passive_state,x,y);
    blink_flag = false;
    println("default");
  }else{
    image(active_state,x,y);
    blink_flag = true;
    println("active");
  }
  //blink_flag =! blink_flag;
}
