//EndlessSynth, playing polyphonical sine wave.
//It supports up to 4 notes. Currently simple note on/off, without attack-release.
//How to play: press up to 4 keys ("note") at the left and 1-4 right most white keys ("string").
//Combination of note key and string key plays a note

//See "sound" for connecting sound details, 
//"keyboard" for connecting keyboard details.

//Monitor port:
//- baud must be set to 500000!
//---------------------------------------------------------------
//number of notes to play simultaneously, 1..4
//const byte POLYPHONY = 2;
//const byte POLYPHONY = 3;
const byte POLYPHONY = 4;

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
  Serial.println("EndlessSynth Sine Wave Synth, v. 1.1 for Arduino Mega");
  Serial.println("To hear a sound, press \"note\" keys at the left and 4 rightmost white \"string\" keys on piano toy keyboard");

  keyboard_setup();
  sound_setup();
  
}

//----------------------------------------------------------
void loop() {
  keyboard_loop();
  sound_loop();

  delay(2);
}




//---------------------------------------------------------------
