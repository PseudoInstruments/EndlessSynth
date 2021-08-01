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

//Audio:
//- simplest connection - connect Arduino's Gnd and A0 to passive's buzzer inputs, or to audio jack 
//- connection with volume control: connect 10 kOhm pot's inputs to Gnd and pin 12, and pot's output to buzzer or audio jack input, 
//and also connect there Arduino's Gnd to a second input.

//---------------------------------------------------------------
void setup() {
  // Serial setup
  Serial.begin(500000);
  Serial.println("06_SynthToyPwmUnoMono - PWM-monosynth for toy keyboard and Arduino Uno/Nano.");

  // Keyboard, see file "keyboard"
  keyboard_setup();

  // Sound, see file "sound"
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

  // Sound
  sound_update();
  
  delay(2); //control rate - so 1000/2 = 500 Hz
}

//----------------------------------------------------------
