import controlP5.*;
import java.util.*;

ControlP5 cp5;
int count_elements;
//String[] list_of_elements;
//List<String> list_of_elements = new ArrayList<>();

void setup(){
  size(1280,900);
  cp5 = new ControlP5(this);
  /*load the topology from the config file
  String[] lines = loadStrings("C:/Users/zsolt.gabor/Documents/Processing/my_projects/gui_v1/element_config.txt");
  for (int i = 0 ; i < lines.length; i++) {
    
       list_of_elements.add(lines.split(" ")); 
    
  }
  */
  
}

void draw(){
 background(120);
  /*
  //ellipse 1
 stroke(0);
 fill(0,255,0);
 ellipse(200, 100, 40, 40);
 //line 1
 stroke(0);
 fill(0,100,255);
 rect(200,90,200,20,30);
  //line 1
  */
 stroke(0);
 fill(0,0,255);
 rotate(-0.6);
 rect(-90,190,200,20,30);

}

void draw_custom_track(int type){
  
}

class Interlocking_View{
  
  Interlocking_View(String input[]){
    
  }
}
