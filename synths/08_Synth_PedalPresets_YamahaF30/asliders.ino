//Sliders
//See description of controls in the main file
//See connections in "aconnections"
//Note: If you have no sliders, set SLIDERS_ENABLED=0 in the main sketch file.

//---------------------------------------------
//Pots, switches, keys
//---------------------------------------------
//  Block 1 - main settings:

float slider_tone_adjust_ = 0; //-1..1 - adjustment of tone


const byte pot_Tone = A2;
const byte pot_Pedal = A3;
const byte pin_Timbre1 = ;
const byte pin_Timbre2;
const byte pin_Timbre3;
const byte pin_Timbre4;
const byte pin_Timbre5;

const byte pin_Arp;
const byte pin_Latch Left;
const byte pin_Latch Right;
const byte pot_Diffusion = A5;
//  Block 2 - synth
const byte pin_ADSR;
const byte pot_Decay = A6;
const byte pit_Sustain;
const byte pot_Attack = A7;
const byte pot_Release = A8;
const byte pot_LFO_Range = A9;
const byte pot_LFO_Rate = A10;
const byte pin_LFO_Shape1;
const byte pin_LFO_Shape2;
const byte pot_Digital_Volume = A11;  //(sound effect 1)
const byte pot_Filter = A12;          //(sound effect 2)
const byte pot_Sample_Rate = A13;     //(sound effect 3)
const byte pin_Enable_Pedal1;
const byte pin_Enable_Pedal2;
const byte pin_Enable_Pedal3;
const byte pin_Enable_LFO1;
const byte pin_Enable_LFO2;
const byte pin_Enable_LFO3;

//Block 3 - presets
const byte pin_Presets;
const byte pin_Preset1;
const byte pin_Preset2;
const byte pin_Preset3;
const byte pin_Preset4;
const byte pin_Preset5;
const byte pin_Preset6;
const byte pin_Preset7;
const byte pin_Preset8;



//---------------------------------------------------------------
void sliders_setup() {
  if (SLIDERS_ENABLED)  {
    prln("Sliders: Tone: A2, ADSR: A3456, sample rate: A8, Diffusion: A9, Timbre A10, sliders power: pin 4 as Gnd, pin 5 as 5V");
  }
  else prln("[Sliders disabled]");

  if (SLIDERS_ENABLED) {
    //set up pins
    pinMode(slider_5V_pin, OUTPUT);
    pinMode(slider_Gnd_pin, OUTPUT);
    digitalWrite(slider_5V_pin, HIGH);
    digitalWrite(slider_Gnd_pin, LOW);
  }
}

//---------------------------------------------------------------
void sliders_loop() {
  //set diff step and Diffusion propagation
  int Tone = (SLIDERS_ENABLED) ? analogRead(apin_Tone) : 512; //0..1023
  int Attack = (SLIDERS_ENABLED) ? analogRead(apin_Attack) : 0;
  int Decay = (SLIDERS_ENABLED) ? analogRead(apin_Decay) : 1023;
  int Sustain = (SLIDERS_ENABLED) ? analogRead(apin_Sustain) : 0;
  int Release = (SLIDERS_ENABLED) ? analogRead(apin_Release) : 0;

  int Sample_rate = (SLIDERS_ENABLED) ? analogRead(apin_Sample_rate) : 0;
  int Diffusion = (SLIDERS_ENABLED) ? analogRead(apin_Diffusion) : 512;
  int Timbre = (SLIDERS_ENABLED) ? analogRead(apin_Timbre) : 512;


  sound_set_sliders(Tone, Sample_rate, Diffusion);
  set_timbre_slider(Timbre);
  set_adsr_sliders(Attack, Decay, Sustain, Release);

  //Debug print
  if (sliders_debug) {
    static byte t = 0;
    t = (t + 1) % 10;
    if (t == 0) {
      pr("SLIDERS Tone="); pr(Tone);
      pr(", Attack="); pr(Attack);
      pr(", Decay="); pr(Decay);
      pr(", Sustain="); pr(Sustain);
      pr(", Release="); pr(Release);

      pr(", Sample_rate="); pr(Sample_rate);
      pr(", Diffusion="); pr(Diffusion);
      pr(", Timbre="); pr(Timbre);

      prln("");
    }
  }
}


//---------------------------------------------------------------
