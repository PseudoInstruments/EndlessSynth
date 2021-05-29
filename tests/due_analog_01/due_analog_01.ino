//Testing Arduino Due sound output to DAC0 and DAC1 with resolution 12 bit


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  //set analogWrite resolution (for DAC)
  analogWriteResolution(12);
  
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   
  for (float x=0; x<10; x+=0.01) {
    float val = sin(x);
    val = val*0.5 + 0.5;
    int v = int(val*4095);
    analogWrite(DAC0,v);        //Output sound, no need set "OUTPUT" mode
    analogWrite(DAC1,v);        

    delayMicroseconds(1000);
  }
  
  digitalWrite(LED_BUILTIN, LOW);    
  delay(1000);                       
}
