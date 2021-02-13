//EndlessSynth SineSynthArpegiatorMega, playing polyphonical sine wave using threshold+decay diffusion.
//--------------------------------------------
//Features:  
//--------------------------------------------
//- sine wave synth, without attack and release
//- two sliders connected to A4 and A5 controls 1-bit algorithm.
//    slider 1: "step" - kind of "level of sound generation", the higher - the more dithering
//    slider 2: "keep" - memory of dithering, the higher - the more harshness
//- arpeggiator ("note" and "string" keys)
//- harsh sound
//- switch octaves


//--------------------------------------------
//How to play:  
//--------------------------------------------
//Notes: 
//   hold 1,2 or 3 notes by left hand and hit any of white rightest \"string\" keys.
//   Combination of note key and string key plays a note, just liko on the guitar.
//Octave switch: 
//   use thee right black keys

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

//--------------------------------------------

//---------------------------------------------------------------
//number of notes to play simultaneously, 1..4
#define POLYPHONY (3)
//#define POLYPHONY (4) //For some reason 4 halts the system :)

//print routines - use their for shortening code
void pr(const char *str) {
  Serial.print(str);
}
void prln(const char *str) {
  Serial.println(str);
}

void pr(int i) {
  Serial.print(i);
}
void prln(int i) {
  Serial.println(i);
}

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("----------------------------------------------------------------");  
  Serial.println("EndlessSynth SineSynthArpegiatorMega_Sliders, v. 1.1 for Arduino Mega");
  Serial.println("3 polyphony, sine wave synthesis. Without attack-release. Sliders controls 1-bit sound settings.");
  Serial.println("How to play:  hold up to three notes by left hand and press 3 hit white \"string\" keys.");
  Serial.println("Combination of note key and string key plays a note");
  Serial.println("To switch octave use three right black keys");
  Serial.println("----------------------------------------------------------------");  
  
  keyboard_setup();
  sound_setup();
  sliders_setup();

  Serial.println();
  Serial.println("Synth is ready to play.");
}

//----------------------------------------------------------
void loop() {
  sliders_loop();
  keyboard_loop();
  sound_loop();

  delay(5); //2);
}




//---------------------------------------------------------------
