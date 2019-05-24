void dotDrawer(int slider1, int slider2) {  
  int slider1_Scaled = int (map (slider1, 0, 1023, 0, width)) ;
  int slider2_Scaled = int (map (slider2, 0, 1023, 0, height));
  while (button3 == 0) {
    fill(int(map(photoCell, 0, 1023, 0, 255)));
    ellipse(slider1_Scaled, slider2_Scaled, slider3, slider3);
  }
  //point(slider1_Scaled, slider2_Scaled);
  ellipse(slider1_Scaled, slider2_Scaled, slider3, slider3);
  circleX = slider1_Scaled;
  circleY = slider2_Scaled;
  circleSize = slider3;
  circleGray = int(map(photoCell, 0, 1023, 0, 255));
  rect(squareX, squareY, squareSize, squareSize);
}


