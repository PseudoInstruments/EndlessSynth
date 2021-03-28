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

//const byte pot_Pedal = A3;
byte Pin_Timbre1;
byte Pin_Timbre2;
byte Pin_Timbre3;
byte Pin_Timbre4;
byte Pin_Timbre5;

byte Pin_Arp;
byte Pin_Latch_Left;
byte Pin_Latch_Right;


//  Block 2 - synth
byte Pin_ADSR;
//const byte pot_Decay = A6;
byte Pin_Sustain;
//const byte pot_Attack = A7;
//const byte pot_Release = A8;

//const byte pot_LFO_Range = A9;
//const byte pot_LFO_Rate = A10;
byte Pin_LFO_Shape1;
byte Pin_LFO_Shape2;
//const byte pot_Digital_Volume = A11;  //(sound effect 1)
//const byte pot_Filter = A12;          //(sound effect 2)

byte Pin_Enable_Pedal1;
byte Pin_Enable_Pedal2;
byte Pin_Enable_Pedal3;
byte Pin_Enable_LFO1;
byte Pin_Enable_LFO2;
byte Pin_Enable_LFO3;

//Block 3 - presets
byte Pin_Presets;
byte Pin_Preset1;
byte Pin_Preset2;
byte Pin_Preset3;
byte Pin_Preset4;
byte Pin_Preset5;
byte Pin_Preset6;
byte Pin_Preset7;
byte Pin_Preset8;


//---------------------------------------------------------------
//function for setup pin - used in pins_iterate
void set_pin_in(byte pin, byte &value) {
  pinMode(pin, INPUT_PULLUP);
}

//---------------------------------------------------------------
//function for read pin - used in pins_iterate
void read_pin_in(byte pin, byte &value) {
  value = digitalRead(pin)?0:1;
}

//---------------------------------------------------------------
//perform action on all pins - set up or read
void pins_iterate(void (*fun)(byte pin, byte &value)) {
  // Block 1
  fun(pin_Timbre1, Pin_Timbre1);
  fun(pin_Timbre2, Pin_Timbre2);
  fun(pin_Timbre3, Pin_Timbre3);
  fun(pin_Timbre4, Pin_Timbre4);
  fun(pin_Timbre5, Pin_Timbre5);

  fun(pin_Arp, Pin_Arp);
  fun(pin_Latch_Left, Pin_Latch_Left);
  fun(pin_Latch_Right, Pin_Latch_Right);


  //  Block 2 - synth
  fun(pin_ADSR, Pin_ADSR);
  fun(pin_Sustain, Pin_Sustain);

  fun(pin_LFO_Shape1, Pin_LFO_Shape1);
  fun(pin_LFO_Shape2, Pin_LFO_Shape2);

  fun(pin_Enable_Pedal1, Pin_Enable_Pedal1);
  fun(pin_Enable_Pedal2, Pin_Enable_Pedal2);
  fun(pin_Enable_Pedal3, Pin_Enable_Pedal3);
  fun(pin_Enable_LFO1, Pin_Enable_LFO1);
  fun(pin_Enable_LFO2, Pin_Enable_LFO2);
  fun(pin_Enable_LFO3, Pin_Enable_LFO3);

  //  Block 3 - presets
  fun(pin_Presets, Pin_Presets);
  fun(pin_Preset1, Pin_Preset1);
  fun(pin_Preset2, Pin_Preset2);
  fun(pin_Preset3, Pin_Preset3);
  fun(pin_Preset4, Pin_Preset4);
  fun(pin_Preset5, Pin_Preset5);
  fun(pin_Preset6, Pin_Preset6);
  fun(pin_Preset7, Pin_Preset7);
  fun(pin_Preset8, Pin_Preset8);
}

//---------------------------------------------------------------
void sliders_setup() {
  if (SLIDERS_ENABLED)  {
    prln("[Sliders enabled]");
  }
  else prln("[Sliders disabled]");

  if (SLIDERS_ENABLED) {
    //set up power pins
    pinMode(pin_Sliders_5V, OUTPUT);
    pinMode(pin_Sliders_Gnd, OUTPUT);
    digitalWrite(pin_Sliders_5V, HIGH);
    digitalWrite(pin_Sliders_Gnd, LOW);

    //set up pins for switches and keys
    pins_iterate(set_pin_in);

  }
}

//---------------------------------------------------------------
//read analog pin and set slider
inline void read_poti(byte a_pin, int &value, int out0, int out1, int default_value = 512) {
  value = SLIDERS_ENABLED?(analogRead(a_pin)):default_value;
  value = map(value, 0, 1023, out0, out1);
}

inline void read_potf(byte a_pin, float &value, int out0, int out1, int default_value = 512) {
  value = mapf(SLIDERS_ENABLED?(analogRead(a_pin)):default_value, 0, 1023, out0, out1);
}

//---------------------------------------------------------------
void sliders_loop() {
  //Read digital pins 
  pins_iterate(read_pin_in);

  //Read pots
  read_potf(pot_Tone, Pot_Tone, tone_min, tone_max); 
  read_poti(pot_Sample_Rate, Pot_Sample_Rate, audio_sample_rate1, audio_sample_rate0);
  read_poti(pot_Diffusion, Pot_Diffusion, 0, diff_keep_max);

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
