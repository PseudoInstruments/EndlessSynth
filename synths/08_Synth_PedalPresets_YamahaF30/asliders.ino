//Sliders
//See description of controls in the main file
//See connections in "aconnections"
//Note: If you have no sliders, set SLIDERS_ENABLED=0 in the main sketch file.

//---------------------------------------------
//Pots, switches, keys
//---------------------------------------------
//  Block 1 - main settings:

float Pot_Tone = 0; //adjustment of tone, -3..3
int Pot_Diffusion = 0;
int Pot_Sample_Rate = 0;


/*const byte pot_Pedal = A3;
const byte pin_Timbre1;
const byte pin_Timbre2;
const byte pin_Timbre3;
const byte pin_Timbre4;
const byte pin_Timbre5;

const byte pin_Arp;
const byte pin_Latch_Left;
const byte pin_Latch_Right;


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
*/


//---------------------------------------------------------------
void sliders_setup() {
  if (SLIDERS_ENABLED)  {
    prln("[Sliders enabled]");  }
  else prln("[Sliders disabled]");

  if (SLIDERS_ENABLED) {
    //set up pins
    pinMode(pin_Sliders_5V, OUTPUT);
    pinMode(pin_Sliders_Gnd, OUTPUT);
    digitalWrite(pin_Sliders_5V, HIGH);
    digitalWrite(pin_Sliders_Gnd, LOW);
  }
}

//---------------------------------------------------------------
//read analog pin and set slider
//TODO here put presets value 
#define POT(a_pin, default_value) (SLIDERS_ENABLED?(analogRead(a_pin)):default_value)
//---------------------------------------------------------------
void sliders_loop() {
  //Tone
  Pot_Tone = 3 * float(POT(pot_Tone, 512) - 512) / 512.0; //-3..3

  //Sample rate
  Pot_Sample_Rate = map(POT(pot_Sample_Rate, 512), 0, 1023, audio_sample_rate1, audio_sample_rate0);
  set_audio_sample_rate(Pot_Sample_Rate);

  //Diffusion
  Pot_Diffusion = map(POT(pot_Diffusion, 512), 0,1023,0,diff_keep_max);
  set_diff_keep(Pot_Diffusion);

/*  
  //set diff step and Diffusion propagation
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
  }*/
}


//---------------------------------------------------------------
