//Reading Arduino text and drawing it in Processing

import processing.serial.*;  

Serial serial; 


int K = 0;  //number of line
String[] Lines;
int maxLines = 20;

PFont f;   //Font


void setup()
{
  size(1920, 1080);
  frameRate(60); 
  String port = Serial.list()[0]; 
  int baud = 250000;
  serial = new Serial(this, port, baud); 

  Lines = new String[maxLines];


  f = createFont("Arial", 16, true); //Create Font
}

void draw() {
  //Read
  if (serial.available()>0) { 
    String input = serial.readStringUntil('\n'); //read line
    if (input!=null) { //null strings - ignore
      //print(input);

      if (input.charAt(0) == '-') { //"------------")) {
        K = 0;  //start new reading
      }

      if (K < maxLines) {
        Lines[K] = input;
      }
      K++;
    }
  }

  //Draw
  background(255);

  //stroke(0,0,0);  
  textFont(f, 16);                  //font to be used
  fill(0);                         //font color
  for (int i=0; i<maxLines; i++) {
    if (Lines[i] != null) {
      //split by spaces and new lines
      String[] list = splitTokens(Lines[i]);
      if (list.length >= 10) {   //NOTE: ignoring short lines 
        for (int j=0; j<list.length; j++) {
          text(list[j], 30 + j*70, 50 + 30 * i);
        }
      }
    }
  
  }
  
}
