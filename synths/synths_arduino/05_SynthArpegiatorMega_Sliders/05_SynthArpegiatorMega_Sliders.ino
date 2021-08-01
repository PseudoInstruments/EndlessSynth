// **05_SynthArpegiatorMega_Sliders** - - synthesizer for Arduino Mega and toy keyboard
//Output goes through buzzer or "Troika" audio amplifier to minijack.
//Slider 2 - sample rate.
//Slider 3 - tone.
//F,G - switch timbre: pulse and sawtooth
//Note: sometimes freezes three notes!

//--------------------------------------------
//Features:
//--------------------------------------------
//- sine wave synth, without attack and release
//- two sliders connected to A4 and A5 controls 1-bit algorithm.
//-   - slider 1: volume
//-   - slider 2: sample rate
//-   - slider 3: tone adjustment
//- arpeggiator ("note" and "string" keys)
//- harsh sound
//- switch octaves
//F,G - switch timbre: pulse and sawtooth
//Note: sometimes freezes three notes!

//--------------------------------------------
//How to play:
//--------------------------------------------
//Notes:
//   hold 1,2 or 3 notes by left hand and hit any of white rightest \"string\" keys.
//   Combination of note key and string key plays a note, just liko on the guitar.
//Octave switch:
//   use thee right black keys
//Sample rate switch:
//   use right F and G (not works while mic mode enabled)
//Press mic button so hear sound from mic.

//--------------------------------------------
//Board & Monitor port speed:
//--------------------------------------------
//  Arduino Mega (Note: Not Uno!)
//  Baud must be set to 500000!
//--------------------------------------------
//Aditional libraries:
//- install "TimerThree" using Library Manager
//--------------------------------------------
//Conection:
//--------------------------------------------
//Buzzer of sound output: pin 2
const byte pin_buz = 2;

//Keyboard:
//Toy Piano sensor was soldered prepared, so we have 32 out pins and pin for GND.
//- 32 keys and Gnd from Toy Piano to Arduino Mega:
//22,24,26,...,36,
//23,25,27,...,37,
//38,40,42,...,52,
//39,41,43,...,53
//and Gnd
//(key numbers are reverted)

//Sliders:
//We are using board with three 10 kOhm potentiomeners,
//Sliders 1,2 - step and diffusion of 1-bit algorithm.
//  A4,A5 - output of two of them,
//  pin 4 as 5V,
//  pin 5 as Gnd.
//Slider 3 - volume
//  pin 4 (common with sliders 1,2) - Gnd
//  pin 2 - sound output


//Microphone with button.
//  1) Take microphone unit for Arduino, connect it to Gnd, 5V, and output signal connect to A0.
//  2) Remove microphone from the module and replace it with some connection to toy microphone.
//  3) Pull-up output average from 0V to 2.5V. For that use trimmer resistor 10 kOhm.
//     Normally microphone unit gives -2.5V...2.5V output,
//     and to digitize signal carefully we need to pull up it to 0..5V range for A0.
//    As a solution, use compact 10kOm trimmer resistor,
//    connect left and right pins to Gnd, 5V, and output to A0 too.
//    Next, adjust trimmer resistor to obtain 2.5V when Mic is in a silence (or, equally, 512 on A0).
//    Now microphone outputs 0..5V to A0!
//  4) Add to microphone button - insert it inside or keep outside microphone.
//    Connect button to pins 8 and 9.

//--------------------------------------------

int debug = 0;
int debug_now = 0;  //signal for debug
long int last_debug_time = 0;

//---------------------------------------------------------------
//number of notes to play simultaneously, 1..4
#define POLYPHONY (3)
//#define POLYPHONY (4) //For some reason 4 halts the system :)

//print routines - use their for shortening code
void pr(const char *str) {
  if (debug) {
    Serial.print(str);
  }
}
void prln(const char *str) {
  if (debug) {
    Serial.println(str);
  }
}

void pr(int i) {
  if (debug) {
    Serial.print(i);
  }
}
void prln(int i) {
  if (debug) {
    Serial.println(i);
  }
}

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("----------------------------------------------------------------");
  Serial.println("EndlessSynth SynthArpegiatorMega_Sliders, v. 1.1 for Arduino Mega");
  Serial.println("3 polyphony, sine wave synthesis. Without attack-release.");
  Serial.println("How to play:  hold up to three notes by left hand and press 3 hit white \"string\" keys.");
  Serial.println("Combination of note key and string key plays a note");
  Serial.println("To switch octave use three right black keys");
  Serial.println("Slider2 (A4) - sample rate, Slider 3 (A5) - tone.");
  Serial.println("To switch timbre right F (pulse) ang G (sawtooth)");
  //Serial.println("Press mic button to pass mic sound to output. The last slider is kind of mic sensitivity.");

  Serial.println("----------------------------------------------------------------");

  mic_setup();
  keyboard_setup();
  sound_setup();
  sliders_setup();

  Serial.println();
  Serial.println("Synth is ready to play.");
  Serial.println("Send '1' to on/off debug print");

}

//----------------------------------------------------------
void loop() {
  if (Serial.available() > 0) { //we expect only "1" rare to on/off debugging
    int key = Serial.read();
    if (key == '1') {
      debug = 1 - debug;
      Serial.print("Debug "); Serial.println(debug);
    }
  }


  sliders_loop();
  mic_loop();
  keyboard_loop();
  sound_loop();

  delay(5); //2);

  //debug print "rare"
  if (debug) {
    long int time = millis();
    if (time > last_debug_time + 200) {
      debug_now = 1;
      last_debug_time = time;
    }
    else {
      debug_now = 0;
    }
  }
}



//---------------------------------------------------------------
