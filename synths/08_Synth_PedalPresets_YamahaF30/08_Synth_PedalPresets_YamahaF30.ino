// **08_Synth_PedalPresets_YamahaF30** - - synthesizer for Arduino Mega and keyboard YAMAHA PSS F30
//New:
//- Expression Pedal for foot-controlled sound
//- Presets
//- Routing to LFO/Predal
//- Discrete timbre switch

//--------------------------------------------
//Features:
//--------------------------------------------
//- 37 keys - from YAMAHA PSS F30 
//send '1' to debug, '2' to print sliders
//- You have an option to not connect sliders - set SLIDERS_ENABLED to 0 above
//- arpeggiator mode ("note" and "string" keys)
//- latch
//- presets
//- switch octaves
//- ADSR-engine for sound 
//- three sound effects (digital volume, filter, sample rate), controlled manually, by LFO and from expression pedal 
//- optimized sound engine computations (removed "/" in wave_table[...] computation, so higher audio rate


//--------------------------------------------
//Communication:
//--------------------------------------------
//Audio output goes through pin 2 to volume pot and to Jack connector.

//Pots, switches, keys: (I using linear pots 10 kOhm, see "sliders" file for the pins map)
//  Block 1 - main settings:
//1 - Volume pot (directly to sound out, without Arduino)
//2 - Tone pot
//3 - Pedal pot (range -max..max - value of effect on other parameters)
//4 - Timbre switch on 5 positions
//5 - Arp switch (enables key to work in "guitar" mode)
//6 - Latch Left switch (enables latch of the left part of keyboard, that is stores last presses)
//7 - Latch Right switch (enabled latch fo the right part of keyboard)
//8 - Diffusion pot ("Original"/"Square" sound)
//  Block 2 - synth
//1 - ADSR switch (enables ADSR envelope)
//2 - Decay pot
//3 - Sustain pot
//4 - Attack pot
//5 - Release pot
//6 - LFO Range pot
//7 - LFO Rate pot
//8 - LFO Shape 1 switch (two shape switches allows to choose 4 LFO shapes)
//9 - LFO Shape 2 switch
//10 - Digital Volume pot (sound effect 1)
//11 - Filter pot (sound effect 2)
//12 - Sample Rate pot (sound effect 3)
//13,14,15 - Enable Pedal switches (for sound effects 1,2,3)
//16,17,18 - Enable LFO switches (for sound effects 1,2,3)
//Block 3 - presets
//1 - Presets switch (Enabled presets)
//2,3,4,5,6,7,8,9 - Choose Preset keys (8 presets, double pressing stores preset to memory)

//Note: If you have no sliders, set SLIDERS_ENABLED=0 below

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
//Connection:
//--------------------------------------------

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
  prln("EndlessSynth 08_Synth_PedalPresets_YamahaF30, v. 1.1 for Arduino Mega and Yamaha PSSF30 keyboard");
  prln("Features:");
  prln("3 polyphony, ADSR envelope, timbres, expression pedal, presets");
  prln("How to play:  hold up to three notes by left hand and press 3 hit white \"string\" keys.");
  prln("Combination of note key and string key plays a note");
  prln("To switch octave use three right black keys");
  prln("----------------------------------------------------------------");
  pr("Control FPS: "); prln(FPS);
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
