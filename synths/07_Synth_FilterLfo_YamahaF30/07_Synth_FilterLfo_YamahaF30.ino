// **07_Synth_FilterLfo_YamahaF30** - - synthesizer for Arduino Mega and keyboard YAMAHA PSS F30
//New:
//- Filter-LFO engine for sound. It's "audio_step" value which corresponds to diffusion properties
//- optimized sound engine computations (removed "/" in wave_table[...] computation, so higher audio rate
//- Added sliders
//- New timbres - triangle and noised

//Output goes through pin 2 to minijack.
//8 potentiometers:
//1 Tone
//2 Filter Value 1
//3 Filter Value 2
//4 Filter LFO speed
//5 Filter LFO shape - sine, ramp
//6 Sample Rate  (Sound settings)
//7 Diffusion
//8 Timbre      sine, triangle, sawtooth, noised
//Note: If you have no sliders, set SLIDERS_ENABLED=0 below

//right F,G - switch timbre


//--------------------------------------------
//Features:
//--------------------------------------------
//- 37 keys - from YAMAHA PSS F30 
//send '1' to debug, '2' to print sliders
//- two sliders connected to A4 and A5 controls 1-bit algorithm.
//-   - slider 1: volume
//-   - slider 2: sample rate 
//-   - slider 3: tone adjustment
//-   You have an option to not connect sliders - set SLIDERS_ENABLED to 0 above
//- arpeggiator ("note" and "string" keys)
//- harsh sound
//- switch octaves
//- F,G - switch timbre
//- ADSR-engine for sound 
//- optimized sound engine computations (removed "/" in wave_table[...] computation, so higher audio rate

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

//YAMAHA PSS F30 Keyboard Reader
//This is truly polyphonic keyboard with 37 keys.

//Keyboards connection: see "keyboard.ino"
//Sliders connection: see "sliders.ino"


//--------------------------------------------

//This parameter enables/disables using sliders
//So if you want check synth without connecting sliders, set it to 0:
const int SLIDERS_ENABLED = 1;
  //0;


byte debug = 0;       //enabled by '1'  from Serial
byte debug_now = 0;  //signal for debug
long int last_debug_time = 0;

byte sliders_debug = 0;     //debug mode for sliders - enabled by '2' from Serial
byte adsr_debug = 0;    //debug for ASDR, enabled by '3' from Serial
byte demo_play = 0;   //enabled by '0' from Serial

const int FPS = 200;  //control FPS. note: really a much less because we use simple delay(1000/FPS) for FPS control.
const int FPS_delay = 1000 / FPS;

//---------------------------------------------------------------
//number of notes to play simultaneously, 1..4
#define POLYPHONY (3)
//#define POLYPHONY (4) //For some reason 4 halts the system :)


//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  prln();
  prln("----------------------------------------------------------------");
  prln("EndlessSynth 07_Synth_FilterLfo_YamahaF30, v. 1.2 for Arduino Mega and Yamaha PSSF30 keyboard");
  prln("Features:");
  prln("3 polyphony, ADSR envelope, timbres: sine, tri, saw, sine+noise");
  prln("How to play:  hold up to three notes by left hand and press 3 hit white \"string\" keys.");
  prln("Combination of note key and string key plays a note");
  prln("To switch octave use three right black keys");
    
  //prln("To switch timbre right F and G");
  //prln("Press mic button to pass mic sound to output. The last slider is kind of mic sensitivity.");

  prln("----------------------------------------------------------------");
  pr("Control FPS: "); prln(FPS);
  mic_setup();
  keyboard_setup();
  timbres_setup();
  sliders_setup();
  ADSR_setup();
  sound_setup();

  prln("----------------------------------------------------------------");
  prln("Synth is ready to play.");
  prln("Send '1' to on/off debug print, '2' to debug sliders, '3' to debug ADSR, '0' to run demo play");
  prln("----------------------------------------------------------------");

}

//----------------------------------------------------------
//universal function for switching values
void toggle_debug(byte key, byte key_expected, const char *text, byte &value) {
  if (key == key_expected) {
    value = 1 - value;
    pr(text); prln(value);
  }
}

//----------------------------------------------------------
void loop() {
  //Serial commands
  if (Serial.available() > 0) { //we expect only "1" rare to on/off debugging
    int key = Serial.read();
    toggle_debug(key, '1', "Debug ", debug);
    toggle_debug(key, '2', "Sliders_debug ", sliders_debug);
    toggle_debug(key, '3', "ASDR_debug ", adsr_debug);
    toggle_debug(key, '0', "Demo_play ", demo_play);
  }

  //Control update at ~200 (really much less) fps - see FPS value
  unsigned long time = millis();
  ADSR_loop(time);
  sliders_loop();
  mic_loop();
  keyboard_loop(time);
  sound_loop();

  //Delay for FPS
  delay(FPS_delay);  

  //compute signal for "rare" debug print
  if (debug) {
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
