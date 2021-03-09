// **07_Synth_ADSR_YamahaF30** - - synthesizer for Arduino Mega and keyboard YAMAHA PSS F30
//New:
//- ADSR-engine for sound 
//- optimized sound engine computations (removed "/" in wave_table[...] computation, so higher audio rate

//Output goes through pin 2 to minijack.
//8 potentiometers:
//1 Tone
//2 Attack       (ADSR settings)
//3 Decay
//4 Sustain 
//5 Release
//6 Sample Rate  (Timbre settings)
//7 Diffusion
//8 [Reserved]
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

byte demo_play = 0;   //enabled by '3' from Serial

const int FPS = 200;  //control FPS. note: really a bit less because we use simple delay(1000/FPS) for FPS control.
const int FPS_delay = 1000 / FPS;

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
void prln() {
  Serial.println();
}

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  prln();
  prln("----------------------------------------------------------------");
  prln("EndlessSynth 06_SynthArpegiatorMega_YamahaPSSF30, v. 1.2 for Arduino Mega and Yamaha PSSF30 keyboard");
  prln("Features:");
  prln("3 polyphony, sine wave synthesis.");
  prln("How to play:  hold up to three notes by left hand and press 3 hit white \"string\" keys.");
  prln("Combination of note key and string key plays a note");
  prln("To switch octave use three right black keys");
  
  if (SLIDERS_ENABLED)  prln("Sliders enabled: Slider2 (A4) - sample rate, Slider 3 (A5) - tone.");
  else prln("[Sliders disabled]");
  
  prln("To switch timbre right F and G");
  //prln("Press mic button to pass mic sound to output. The last slider is kind of mic sensitivity.");

  prln("----------------------------------------------------------------");
  pr("Control FPS: "); prln(FPS);
  mic_setup();
  keyboard_setup();
  sound_setup();
  sliders_setup();

  prln("----------------------------------------------------------------");
  prln("Synth is ready to play.");
  prln("Send '1' to on/off debug print, '2' to debug sliders, '3' to run demo play");
  prln("----------------------------------------------------------------");

}

//----------------------------------------------------------
void loop() {
  //Serial commands
  if (Serial.available() > 0) { //we expect only "1" rare to on/off debugging
    int key = Serial.read();
    if (key == '1') {
      debug = 1 - debug;
      pr("Debug "); prln(debug);
    }
    if (key == '2') {
      sliders_debug = 1 - sliders_debug;
      pr("Sliders_debug "); prln(sliders_debug);
    }
    if (key == '3') {
      demo_play = 1 - demo_play;
      pr("Demo_play "); prln(demo_play);      
    }
  }

  //Control update at 200 fps - see FPS value
  sliders_loop();
  mic_loop();
  keyboard_loop();
  sound_loop();

  //Delay, so ~200 fps
  delay(FPS_delay);  

  //compute signal for "rare" debug print
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
