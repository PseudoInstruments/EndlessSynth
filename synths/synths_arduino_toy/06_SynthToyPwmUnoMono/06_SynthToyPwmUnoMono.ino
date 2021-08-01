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

#include <TimerOne.h>

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

byte pin_audio = A0;  //we will use A0 as digital output pin!

//---------------------------------------------------------------
int SampleRate = 10000;   //Sample rate of the synth
int TimerPeriod = long(1000000) / SampleRate;


//---------------------------------------------------------------
void setup() {
  // Serial setup
  Serial.begin(500000);
  Serial.println("06_SynthToyPwmUnoMono - PWM-monosynth for toy keyboard and Arduino Uno/Nano.");
  Serial.println();

  // Keyboard, see file "keyboard"
  keyboard_setup();

  // Starting timer for sound generation
  Timer1.initialize(TimerPeriod);   //set timer's period
  Timer1.attachInterrupt(audio_out); //set function which is called by timer
  
  Serial.println("Synth is ready to play.");

}

//----------------------------------------------------------
void loop() {
  // Keyboard 
  keyboard_loop();
  
  delay(2);
}


//----------------------------------------------------------
//Main sound function which is called by timer SampleRate times per second
void audio_out() {
  /*if (ledState == LOW) {
    ledState = HIGH;
    blinkCount = blinkCount + 1;  // increase when LED turns on
  } else {
    ledState = LOW;
  }
  digitalWrite(led, ledState);*/
}



//---------------------------------------------------------------
