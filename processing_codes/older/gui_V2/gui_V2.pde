// interlocking_gui

import controlP5.*;

ControlP5 gui_layout;

int default_color = color (69,69,69);


void setup(){
    size(1440,900);
    noStroke();
    gui_layout = new ControlP5(this);
    gui_layout.addButton("signal")
              .setValue(0)
              .setPosition(450,250)
              .setImages(loadImage("signal_aspect_default.png"))
              .updateSize();
    
}

void draw(){
   background(default_color); 
}
