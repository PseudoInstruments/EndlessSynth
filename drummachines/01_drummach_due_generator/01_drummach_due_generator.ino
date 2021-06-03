//01_drummach_due_generator - drummachine sound test for Arduino Due.
//Use resistors 500 Ohm to prevent burning DAC0,1! - see details below.

//--------------------------------------------
//Notes:
//--------------------------------------------
//This sketch generates percussion sounds for Arduino Due.
//It's based on drums/06_drum_pedal_2tones_timbres,
//I love sound instabilities, so run sound loop at main loop, and control steps are called by timer!
//Sound is still 1 bit, and I use 12-bit DAC for volume control.
//
//For testing simple Arduino Due sound generation, see tests/due_analog_01

//--------------------------------------------
//Controller: Arduino Due
//Additional Libs to install: Due Timer

//Connections:
//Using 250000 Baud for Serial, because Due can't work properly with 500000 which I would prefer...
//https://forum.arduino.cc/t/arduino-due-baudrate-higher-than-115200/539463/4

//Connect Jack output to Due:
//- Due Gnd -> Jack Gnd
//- Due DAC0 -> Resistor on 500 Ohm -> Jack Signal (if mono, if stereo - to both signals).
//Use resistors to prevent burning DAC0,1 !

//--------------------------------------------

//Links on sound generating with Due:
//http://groovuino.blogspot.com/
//http://rcarduino.blogspot.com/2012/12/arduino-due-dds-part-1-sinewaves-and.html
//https://github.com/Gaetino/Groovuino

//--------------------------------------------
//Types
typedef unsigned char uint8;
typedef char int8;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef int int32;

const int max_u16 = 65535;
const int u16_n = 65536;

typedef long long int int64;
typedef unsigned long long int uint64;


//Stereo sample for playing
const uint16 max_audio = 4095;
struct AudioSample {
  uint16 left;
  uint16 right;
  uint16 pause_mcs;
};

struct DrumSound {
  float vol=0.5;
  float pan=0.5;
  float tmb = 0;  //timbre  tone/noise
  float frq_note = 69;  //freq in MIDI-notes
  float frq_delta=0;
  float vol_delta=0;
};

//--------------------------------------------
#include <DueTimer.h>

const int FPS = 100;          //Control rate
const int Sample_Rate = 16000; //Sound generator rate

//-----------------------------------------------------------------------
void setup() {
  Serial.begin(250000);
  prln();
  prln("----------------------------------------------------------------");
  prln("Endless Drummachine 01_drummach_due_generator - drummachine sound test for Arduino Due, v. 1.1");
  prln("----------------------------------------------------------------");
  pr("Control rate: "); prln(FPS);

  //Set analogWrite resolution (for DAC)
  analogWriteResolution(12);

  //Run timers
  //Timer0.attachInterrupt(loop1).setFrequency(44100).start();
  Timer8.attachInterrupt(control_loop).setFrequency(FPS).start();

  seq_setup();
}

//-----------------------------------------------------------------------
//Control loop - called by timer
int time_ = 0;

void control_loop() {
  seq_update();
  //start sound each second
  //if (time_ == 0) {
  //  prln("play");
  //}
  //time_++;
  //time_ %= 100;
}

//-----------------------------------------------------------------------
//Main loop - used for audio
void loop() {
  //static long int t = 0;
  //t++;
  //prln(t);

  AudioSample out = synth_audio_out();
  analogWrite(DAC0, out.left);       //Output sound, no need set "OUTPUT" mode
  analogWrite(DAC1, out.right);
  delayMicroseconds(out.pause_mcs);
}

//-----------------------------------------------------------------------
