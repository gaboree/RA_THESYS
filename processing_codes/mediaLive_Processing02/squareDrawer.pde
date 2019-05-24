void squareDrawer(int slider1, int slider2) {
  int slider1_Scaled = int (map (slider1, 0, 1023, 0, width)) ;
  int slider2_Scaled = int (map (slider2, 0, 1023, 0, height));
  rect(slider1_Scaled, slider2_Scaled, slider3, slider3);
  squareX = slider1_Scaled;
  squareY = slider2_Scaled;
  squareSize = slider3;
  fill(circleGray);
  ellipse(circleX, circleY, circleSize, circleSize);
}



