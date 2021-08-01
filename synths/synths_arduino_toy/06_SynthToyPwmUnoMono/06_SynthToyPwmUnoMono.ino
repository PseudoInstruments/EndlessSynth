//06_SynthToyPwmUnoMono - PWM-monosynth for toy keyboard and Arduino Uno/Nano.
//--------------------------------------------
//Features:  
//--------------------------------------------
//- 1-bit PWM mono synth for Arduino Uno/Nano
//- Controlled by keyboard from toy synthesizer
//- Optionally you can connect several pots for controlling PWM, octave, tune, attack, release.

//--------------------------------------------
//Aditional libraries:
//--------------------------------------------
//- install "TimerOne" library using Library Manager

//--------------------------------------------
//Monitor port speed: 
//--------------------------------------------
//  Serial port baud must be set to 500000.

//--------------------------------------------
//Conection:
//--------------------------------------------
//Keyboard:
//- Connect Arduino's pins 5,4,3,2 to keyboard's ribbon cable with 4 contacts - for choosing keys blocks
//- Сonnect Arduino's pins 6,7,8,9,10,11,12,13 to to keyboard's ribbon cable with 8 contacts - for choosing key inside block

//Potentiometers:
//Use five 10 KOhm linear pots
//- Connect Arduino's Gnd and 5V to pots inputs, 
//and Arduino's pins A1,A2,A3,A4,A5 to pot's outputs.
//This pots control Pitch, Attack, Release, PWM, Octave.

//Audio output:
//- mono audio output as two wires "Signal" and "Gnd", 
//Connect this audio output wires to audio jack (and then to your speakers), 
//or to passive buzzer 5V or to compact D-class amplifier with compact speaker.
//Variant 1 - Simplest connection 
//   - Connect Arduino's Gnd and A0 to audio "Gnd" and "Signal"
//Variant 2 - Connection with volume control: 
//  - Connect 10 kOhm pot's inputs to Arduino's Gnd and pin A0 passed through 10 KOhm resistor,
//  - Connect pot's output to audio "Signal", 
//  - Connect Arduino's Gnd to audio "Gnd".
//  Note: resistor is required to to get audio signal of standard voltage 2.5V instead 5V which is output from A0), 

//---------------------------------------------------------------
void setup() {
  // Serial setup
  Serial.begin(500000);
  Serial.println("06_SynthToyPwmUnoMono - PWM-monosynth for toy keyboard and Arduino Uno/Nano.");

  // Keyboard, see file "keyboard"
  keyboard_setup();

  // Pots, see file "pots"
  pots_setup();

  // Sound geberation, see file "sound"
  sound_setup();

  Serial.println();
  Serial.println("Synth is ready to play.");

}

//----------------------------------------------------------
void loop() {
  //this is function for control - updating keyboard and sound state,
  //with control rate, which is much slower than SampleRate, used at audio_loop()
  // Keyboard 
  keyboard_update();

  // Pots
  pots_update();

  // Sound
  sound_update();
  
  delay(2); //control rate - so 1000/2 = 500 Hz
}

//----------------------------------------------------------
