/* Simple boombox code.
Copied from 01_sound_passthrough_simple
Performance video: https://youtu.be/3pW5L71EReI

Program gets sound from microphone (A0) 
and immediately outputs it to buzzer (pin 2) using simple thresholding, without diffusion.
Can be used for electo-guitar connection too.


Controller: Arduino Uno

----------------------------------------
Connection
----------------------------------------
1) Microphone:
Take microphone unit for Arduino:
  connect it to Gnd, 5V, and output signal connect to A0.
  
2) Trimmer resistor for pulling-up microphone:
Note, that microphone unit gives -2.5V...2.5V output, 
so to digitize signal carefully we need to pull up it to 0..5V range for A0.
As a solution, I use compact 10KOm trimmer resistor, 
connect left and right pins to Gnd, 5V, and output to A0 too.
Next, I adjust trimmer resistor to obtain 2.5V when Mic is in a silence (or, equally, 512 on A0).
Now microphone outputs 0..5V to A0!

3) Buzzer (or audio output) to pin 2.

----------------------------------------
Programming details
----------------------------------------
By default Arduino ADC works on 8Khz - ok for us.
But can speedup:
https://www.instructables.com/Make-Your-Own-Spy-Bug-Arduino-Voice-Recorder/
Recorder:
https://github.com/TMRh20/TMRpcm
http://microsin.net/programming/avr/real-time-digital-audio-processing-using-arduino.html 

 */

const byte pin_buz = 2; //Buzzer pin

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Sound passtrough test (simple thresholding), v. 1.1 for Arduino Uno or Mega");
  Serial.println("Program gets sound from microphone (A0) and immediately outputs"); 
  Serial.println("it to buzzer (pin 2) using simple thresholding.");
  Serial.println("Note: please use trimmer resistor 10KOhm to move silence mic level to 2.5V (or, equally, 512 on A0)");

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinMode(pin_buz, OUTPUT); //activate buzzer

}

//--------------------------------------------------------------
void loop() {
  
  //run loop to measure time and deduce sample rate
  int n = 1000;
  int v;
  unsigned long time0 = micros();
  for (int i=0; i<n; i++) {
    //get sample
    v = analogRead(A0);
    //output
    if (v > 512) digitalWrite(pin_buz, HIGH);
    else digitalWrite(pin_buz, LOW);

    //delay
    delayMicroseconds(10);  //need to adjust delay to achieve 8000Hz rate
  }

  //estimate sample rate, runs once
  static byte was_measured = 0;
  if (!was_measured) {
    was_measured = 1;
    unsigned long delta = micros() - time0;
    long int audio_sample_rate = 1000000.0 * n / delta;
    Serial.print("Computed audio sample rate: "); Serial.println(audio_sample_rate);
  }  

  //debug - use for setting up trimmer resistor so it print 512
  Serial.println(v);
  //delay(30);
  
}
