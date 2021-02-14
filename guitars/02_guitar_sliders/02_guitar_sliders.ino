/* Guitar with sliders

1-bit algorithm - simple thresholding.

Sliders:
1 - controls volume
2 - controls sample rate
3 - controls bias for ADC zero level, in our case it means PWM, sensitivity for guitar.
 
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

const byte sliders_gnd_pin = 5;
const byte sliders_5v_pin = 6;
;
//slider1 affects volume output directly, without arduino
const byte slider2_analog_pin = A4;   //sample rate
const byte slider3_analog_pin = A5;   //pwm

int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Guitar with sliders (simple thresholding), v. 1.1 for Arduino Uno or Mega");
  Serial.println("Program gets sound from microphone (A0) and immediately outputs"); 
  Serial.println("it to buzzer (pin 2) using simple thresholding.");
  Serial.println("Note: please use trimmer resistor 10KOhm to move silence mic level to 2.5V (or, equally, 512 on A0)");
  Serial.println("send '1' to on/off debug print");

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinMode(pin_buz, OUTPUT); //activate buzzer

  //sliders
  pinMode(sliders_gnd_pin, OUTPUT); 
  pinMode(sliders_5v_pin, OUTPUT); 
  digitalWrite(sliders_gnd_pin, LOW);
  digitalWrite(sliders_5v_pin, HIGH);

}


//--------------------------------------------------------------
//Constants
const int audio_delay_mcs0 = 0;   
const int audio_delay_mcs1 = 400;   
const int audio_thresh0 = 512;     
const int audio_thresh1 = 512+10;     


//Sound parameters
int audio_delay_mcs = 10;   //delay in sound loop
int audio_thresh = 512;     //threshold for PWM, sensitivity

int audio_input_ = 0;
//--------------------------------------------------------------
inline void control_step() {
  if (Serial.available() > 0) { //we expect only "1" rare to on/off debugging
    int key = Serial.read();
    if (key == '1') {
      debug = 1 - debug;
      Serial.print("Debug "); Serial.println(debug);
    }    
  }
  
  //TODO make each step at separate loop to minimize delay
  int slider2 = analogRead(slider2_analog_pin);  //0..1023
  int slider3 = analogRead(slider3_analog_pin);  //0..1023

  audio_delay_mcs = map(slider2,0,1023,audio_delay_mcs0,audio_delay_mcs1);
  audio_thresh = map(slider3, 0, 1023, audio_thresh1, audio_thresh0); //reverted range

  //debug print
  if (debug) {
    //use audio input for setting up trimmer resistor so it print 512
    Serial.print("audio input "); Serial.print(audio_input_);
    Serial.print("  sliders "); Serial.print(slider2); 
    Serial.print(","); Serial.print(slider3);
    Serial.print("  audio_delay_mcs "); Serial.print(audio_delay_mcs); 
    Serial.print("  audio_thresh "); Serial.print(audio_thresh);

    Serial.println();
  }
}


//--------------------------------------------------------------
void loop() {

  control_step();
  
  //run loop to measure time and deduce sample rate
  int n = map(audio_delay_mcs, audio_delay_mcs0, audio_delay_mcs1, 1000, 200);   
  //NOTE: n audio loops are used to make control rate less than audio rate
  //currently formula for n just empirical, and control changing when delay mcs was changed.
  
  unsigned long time0 = micros();
  for (int i=0; i<n; i++) {
    //get sample
    audio_input_ = analogRead(A0);
    //output
    if (audio_input_ > audio_thresh) digitalWrite(pin_buz, HIGH);
    else digitalWrite(pin_buz, LOW);

    //delay
    delayMicroseconds(audio_delay_mcs);  
  }

  //estimate sample rate, runs once
  static byte was_measured = 0;
  if (!was_measured) {
    was_measured = 1;
    unsigned long delta = micros() - time0;
    long int audio_sample_rate = 1000000.0 * n / delta;
    Serial.print("Computed audio sample rate: "); Serial.println(audio_sample_rate);
  }  

}
