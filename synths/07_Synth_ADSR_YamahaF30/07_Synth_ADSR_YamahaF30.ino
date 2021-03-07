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
//7 
//8 


//F,G - switch timbre: pulse and sawtooth

//This parameters enables/disables using sliders
//So if you want check synth without connecting sliders, set it to 0:
const int SLIDERS_ENABLED = //1;
    0;


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
  Serial.println("EndlessSynth 06_SynthArpegiatorMega_YamahaPSSF30, v. 1.1 for Arduino Mega and Yamaha PSSF30 keyboard");
  Serial.println("3 polyphony, sine wave synthesis. Without attack-release.");
  Serial.println("How to play:  hold up to three notes by left hand and press 3 hit white \"string\" keys.");
  Serial.println("Combination of note key and string key plays a note");
  Serial.println("To switch octave use three right black keys");
  
  if (SLIDERS_ENABLED)  Serial.println("Sliders enabled: Slider2 (A4) - sample rate, Slider 3 (A5) - tone.");
  else Serial.println("[Sliders disabled]");
  
  Serial.println("To switch timbre right F (sine) ang G (sawtooth)");
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
