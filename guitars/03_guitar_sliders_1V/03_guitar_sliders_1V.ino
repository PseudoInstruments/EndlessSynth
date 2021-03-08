/* Endless Guitar is a 1-string 1-bit guitar with controllable volume, sample rate and sensitivity, with 1V ADC ARef.
In this version I increased sensitivity by 5 times using ARef 1.1V (instead default 5V) for ADC.


Program gets sound from A0 and immediately outputs it to buzzer (pin 2) using thresholding, without diffusion. 
Controller: Arduino Uno or Nano

----------------------------------------
Adjustment before playing
----------------------------------------
  - Built-in LED lighing when zero-level is achieved, so helping adjust trimming resistor without computer. 
  So, before start playing, adjust resistor in silence to achieve LED flashing. It means zero level it tuned correctly.


-------------------------------------------
Play tricks
-------------------------------------------
Now it's possible to play guitar very delicately touching strings.
Also I found it's ok to loosen the string. 
I use 3th bass string in guitar.
-------------------------------------------
Sliders:
1 - controls volume - connected directly to DAC and Arduino audio output.
2 - controls sample rate
3 - controls sensitivity bias for ADC zero level, in our case it means PWM, sensitivity for guitar.

//Note: nonlinearity - slider slows start TODO linearize sliders values
 
----------------------------------------
Connection
----------------------------------------
Audio input
1) Guitar pickup:
  connect it to Gnd, 5V, and output signal connect to A0.
  
2) Trimmer resistor 10 kOhm for pulling-up Guitar input to A0 = 512 in silence:
Note, that pickup gives -0.1V..0.1V, 
so to digitize signal carefully we need to pull up it to 0..1.1V range for A0.
As a solution, I use compact 10KOm trimmer resistor, 
connect left and right pins to Gnd, 5V, and output to A0 too.
Next, I adjust trimmer resistor to obtain value A0 = 512 when pickup is in a silence.
To see it, send "1" to Arduino using Arduino IDE'a Monitor Port to enable debug print mode.

3) Trimmer resistor 10 kOhm for decreasing power to sliders. 
It's inputs connected to pins 5 (Gnd) and 6 (5V)

Audio output
3) Audio output to mini-jack (or buzzer). If you have no slider for volume control, 
connect audio output directly to pin 2 and Gnd.
If you have slider for volume control (I potentiometer 10 kOhm) , connect Gnd and pin 2 to slider's inputs,
then connect minijack (buzzer) slider's output and Gnd.

4) Sliders - in this sketch used two sliders (I mean potentiometers 10kOhm) for controlling sound parameters.
Also you need trimmer resistor 10 kOhm.
From Arduino we will use output power: pin 5 (Gnd) and pin 6 (5V).

The final goal is to connect:
A4 - sample rate
A5 - sensitivity

Connect both sliders and trimmer resistor's inputs to pin 5 (Gnd).
Connect trimmer's resistor second input to pin 6 (5V).
The output of trimmer's resistor connect to second input of sliders.
Finally, connect sliders outputs to A4 and A5.
The trimmer resostor is reqired because in this sketch we are using Aref 1.1V for Arduino analog inputs, 
so without trimming sliders output value 1023 in the middle of slider's position.
Using trimmer resistor allows to adjust power to slider.
So after start sketch, send in Monitor Port "1" to start debug print, and adjust resistor to obtain sliders going to 1023 only at final position.
(Remark: using trimmer resistor instead fixed resistor is because it's flexible to adjust for different number of sliders and various Aref.)

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

const byte pin_buz = 2; //Audio output
const byte pin_led = 13;  //Built-in led pin

const byte sliders_gnd_pin = 5;
const byte sliders_5v_pin = 6;

//slider1 affects volume output directly, without arduino
const byte slider2_analog_pin = A4;   //sample rate
const byte slider3_analog_pin = A5;   //pwm

const unsigned int analog_min = 20;     //it's appears minimal value is 20, not 0 in the current setup
const unsigned int analog_max = 1023;


int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Guitar with sliders and 1.1V ARef (no diffusion), v. 1.4 for Arduino Uno, Nano");
  Serial.println("Program gets sound from microphone (A0) and immediately outputs.");
  Serial.println("Audio output: pin 2");
  Serial.println("Slider 1 - between pin2 and audio output, Slider 2 - A4, Slider 3 - A5.");
  Serial.println("Note: ARef is 1.1V for increased sensitivity.");
  Serial.println("Note: please adjust trimmer resistor 10KOhm to move silence audio level 512 on A0 in debug print mode.");
  Serial.println("You can do it without computer - see built-in LED, it lighing when zero-level is achieved.");
  Serial.println("Send '1' to on/off debug print");
  

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinMode(pin_buz, OUTPUT); //activate buzzer

  pinMode(pin_led, OUTPUT); //activate led


  //sliders
  pinMode(sliders_gnd_pin, OUTPUT); 
  pinMode(sliders_5v_pin, OUTPUT); 
  digitalWrite(sliders_gnd_pin, LOW);
  digitalWrite(sliders_5v_pin, HIGH);

  //ARef - set to 1.1V, for increasing sensitivity
  analogReference(INTERNAL);

}


//--------------------------------------------------------------
//Constants
const int audio_delay_mcs0 = 0;   
const int audio_delay_mcs1 = 1000;//400;   
const int audio_thresh_slider0 = 512;     
const int audio_thresh_slider1 = 512+20;   
const int audio_thresh_hyster = 2;     

//Sound parameters
int audio_delay_mcs = 10;   //delay in sound loop

//zero value range for adjusting trimming resistor - inside range the led is lighting
const int audio_thresh_adj0 = 512-5;  
const int audio_thresh_adj1 = 512+5;  


int audio_thresh0 = 512;     //two thresholds for hysteresis (stability)
int audio_thresh1 = audio_thresh0 + audio_thresh_hyster;     

int audio_input_ = 0;

int loops_ = 1000;

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

  audio_delay_mcs = map(slider2, analog_min, analog_max,audio_delay_mcs0,audio_delay_mcs1);
  audio_thresh0 = map(slider3, analog_min, analog_max, audio_thresh_slider1, audio_thresh_slider0); //reverted range
  audio_thresh1 = audio_thresh0 + audio_thresh_hyster;

  //lighting LED if audio inside adjusting range - helping for adjusting trimming resistor
  if (audio_input_ >= audio_thresh_adj0 && audio_input_ <= audio_thresh_adj1) {
    digitalWrite(pin_led, HIGH);
  }
  else {
    digitalWrite(pin_led, LOW);
  }
  
  //debug print
  if (debug) {
    //use audio input for setting up trimmer resistor so it print 512
    Serial.print("audio input "); Serial.print(audio_input_);
    Serial.print("  audio loops "); Serial.print(loops_);
    Serial.print("  sliders "); Serial.print(slider2); 
    Serial.print(","); Serial.print(slider3);
    Serial.print("  audio_delay_mcs "); Serial.print(audio_delay_mcs); 
    Serial.print("  audio_thresh "); Serial.print(audio_thresh0);

    Serial.println();
  }
}


//--------------------------------------------------------------
void loop() {

  //run control step
  control_step();

  //run audio loop 
  loops_ = map(audio_delay_mcs, audio_delay_mcs0, audio_delay_mcs1, 500, 40); //1000, 10);  
  //NOTE - this simple formula gives good sound but in the middle part of slider 2 it freezes a control rate a bit

  //this algorithm more precise, but sounding not so good:
  
  //long int freq = (1000000 / (audio_delay_mcs + 100));
  //Serial.println(freq);
  //loops_ = map(freq, 9000, 200, 1000, 30); //TODO make as params and tune. I see debug speed to tune 
  //loops_ = constrain(loops_, 30, 1000);
     
  unsigned long time0 = micros();
  for (int i=0; i<loops_; i++) {
    //get sample
    audio_input_ = analogRead(A0);
    //output
    //two thresholds for hysteresis (stability)
    if (audio_input_ >= audio_thresh1) digitalWrite(pin_buz, HIGH);
    else {
      if (audio_input_ <= audio_thresh0) digitalWrite(pin_buz, LOW);
    }

    //delay
    delayMicroseconds(audio_delay_mcs);  
  }

  //estimate sample rate, runs once
  static byte was_measured = 0;
  if (!was_measured) {
    was_measured = 1;
    unsigned long delta = micros() - time0;
    long int audio_sample_rate = 1000000.0 * loops_ / delta;
    Serial.print("Computed audio sample rate: "); Serial.println(audio_sample_rate);
  }  

}
