//Testing Arduino Due sound output to DAC0 and DAC1 with resolution 12 bit
//It generates sine wave tone and pause, again and again.
//Note: I use oscilloscope for measurement, and also buzzer,
//but for some reason DAC0 - was broken.
//May be need to use 500 Ohm resistors to protect DAC0,1!

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  //set analogWrite resolution (for DAC)
  analogWriteResolution(12);
  
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   
  for (long int t=0; t<10000; t++) {
    float val = sin(t*0.2);
    val = val*0.5 + 0.5;
    int v = int(val*4095);
    analogWrite(DAC0,v);        //Output sound, no need set "OUTPUT" mode
    analogWrite(DAC1,v);        

    delayMicroseconds(100);
  }
  analogWrite(DAC0,0);
  analogWrite(DAC1,0);
  
  digitalWrite(LED_BUILTIN, LOW);    
  delay(1000);                       
}
