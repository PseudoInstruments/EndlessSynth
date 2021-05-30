//Testing Arduino Due sound output to DAC0 and DAC1 with resolution 12 bit
//It generates sine wave tone with fading and slowly decreasing volume, and pause, again and again.

//Result: Volume decreasing feels good, but the sine wave sounds aliased - hi-frequency noise...

//Important note on connection: use 500 Ohm resistors to protect DAC0,1 from burning:
//    DAC0,1 -> Resistor 500 Ohm -> Buzzer or Jack Signal. 
//    Gnd -> Buzzer of Jack Gnd

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  //set analogWrite resolution (for DAC)
  analogWriteResolution(12);

  //Write zeros to audio
  analogWrite(DAC0,0);
  analogWrite(DAC1,0);

}

//Decreasing
long int Max_Vol = 1000;
long int Vol = Max_Vol;

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   
  long int Fade = 500; //Fading

  for (long int t=0; t<10000; t++) {
    long int vol = (t<500)?t:(t>10000-Fade)?(10000-1-t):Fade; //0..Fade
    float val = sin(3*t*0.1);
    val = val*0.5 + 0.5;
    int v = int(val*4095*vol*Vol/(Fade * Max_Vol));
    analogWrite(DAC0,v);        //Output sound, no need set "OUTPUT" mode
    analogWrite(DAC1,v);        

    delayMicroseconds(10);
  }
  Vol -= 200;
  if (Vol == 0) Vol = 1000;
  
  analogWrite(DAC0,0);
  analogWrite(DAC1,0);
  
  digitalWrite(LED_BUILTIN, LOW);    
  delay(1000);                       
}
