//EndlessSynth SineSynthArpegiatorMega, playing polyphonical sine wave.
//3 polyphony, sine wave synthesis. Without attack-release.
//How to play:  hold up to three notes by left hand and press 3 hit white \"string\" keys.
//Combination of note key and string key plays a note
//To switch octave use thee right black jeys

//See "sound" for connecting sound details, 
//"keyboard" for connecting keyboard details.

//Monitor port:
//- baud must be set to 500000!
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
  Serial.println("EndlessSynth SineSynthArpegiatorMega, v. 1.1 for Arduino Mega");
  Serial.println("3 polyphony, sine wave synthesis. Without attack-release.");
  Serial.println("How to play:  hold up to three notes by left hand and press 3 hit white \"string\" keys.");
  Serial.println("Combination of note key and string key plays a note");
  Serial.println("To switch octave use thee right black keys");
  Serial.println("----------------------------------------------------------------");  
  
  keyboard_setup();
  sound_setup();

  Serial.println();
  Serial.println("Synth is ready to play.");
}

//----------------------------------------------------------
void loop() {
  keyboard_loop();
  sound_loop();

  delay(2);
}




//---------------------------------------------------------------
