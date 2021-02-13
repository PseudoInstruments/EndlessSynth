// **04_SineSynthArpegiatorMega_Sliders_Boombox** - - synthesizer for Arduino Mega and toy keyboard 
//with two sliders controlling 1-bit dithering algorithm. Output goes through buzzer or "Troika" audio amplifier to minijack.
//Also to A0 connected toy microphone coupled with potentiometer to output to A0 512 in silence.
//Also connected button from microphone. When button is pressed is sends signal instead synthesizing.
//So synthesizer works as boombox 1-bit engine too.

//--------------------------------------------
//Features:  
//--------------------------------------------
//- 3 polyphony
//- sine wave synthesis
//- without attack-release
//- two sliders connected to A4 and A5 controls 1-bit algorithm.
//    slider 1: "step" - kind of "level of sound generation", the higher - the more dithering
//    slider 2: "keep" - memory of dithering, the higher - the more harshness
//- microphone with button. Microphone is connected to A0 and button to pins 8 and 9.
//    when button is pressed, the sound from mic sends to 1-bit audio output instead keys.
//    microphone is connected using Arduino mic module with added 10kOhm potentiometer 
//    to achieve values from A0 around 512 at silence for sound reading from ADC works correct.

//- the third slider controls sound volume.

//--------------------------------------------
//How to play:  
//--------------------------------------------
//Notes: 
//   hold 1,2 or 3 notes by left hand and hit any of white rightest \"string\" keys.
//   Combination of note key and string key plays a note, just liko on the guitar.
//Octave switch: 
//   use thee right black keys
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


//Toy microphone with button. 
//  Toy microphone is connected to Arduino's standard mic module, where original mic removed 
//  and soldered wires to connect to toy mic. 
//  Mic module is connected to A0, Gnd and 5V, and button is connected to pins 8 and 9.
//  To Mic module also added potentiometer (connected to A0, Gnd and 5V too) on 10 kOhm
//  pulling up the mic module output, so the module with potentiometer outputs value 512 in silenc.

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
