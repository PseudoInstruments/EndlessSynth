//(ADSR envelope - not now)
//Filter LFO of "audio_step"
//It works at control rate
//The ADSR parameters are computed at key pressing and not changing until all keys are released.
//ASDR processes all keys using single envelope, which starts on first key is pressed and ends an all keys are released.

//affects audio_volume value

byte adsr_pressed = 0;             //is key pressed
unsigned long adsr_t0 = 0;  //time of pressing key
unsigned long adsr_t1 = 0;  //time of max volume
unsigned long adsr_t2 = 0;  //time of decay end
unsigned long adsr_max_vol = 256;   //max volume
unsigned long adsr_sustain_vol = 256;   //sustain volume
byte adsr_state = 0;    //nothing, attack, decay, release

//---------------------------------------------------------------
//value from xsound - volume of sound
extern int audio_volume;
extern const int audio_volume_max;
extern long int sound_value;    //we change it to 0 when sound off to prevent from 1-bit "beep"

extern int audio_step;  //- Filter value, we will control it here

//---------------------------------------------------------------
void synth_setup() {
  audio_volume = 0;
}


//---------------------------------------------------------------
//LFO
const int lfo_max = 127;

//get timbres for lfo shape
extern const unsigned int wave_n;
extern const int timbre_range;

extern char wave_sine[];
extern char wave_tri[];
extern char wave_saw[];
extern char wave_noised[];

char *LFO_wave = wave_sine;

const float lfo_speed_min = 0;
const float lfo_speed_max = wave_n * 10 * .001f; //10*1000;  //Hz*1000


//LFO current state
int LFO_Range = 0;
float LFO_Rate = 0;
int LFO_Shape = 0; //sine, sawtooth

float lfo_phase_ = 0;
int LFO_Value_ = 0;   //current LFO value

//Pedal 
const int Pedal_Input_Max = 19;   //Note: my pedal outputs 0..19 because has range 1kOhm..20 kOhm
//const int Pedal_Max = 127;    - commented, because hardcoded in Pot_Pedal_Sens
int Pedal_ = 0;      //Pedal value 

//---------------------------------------------------------------
//event: key pressed or released
void ADSR_key_event(unsigned int time, byte pressed) {
  if (pressed != adsr_pressed) {
    //pr("ADSR event "); prln(pressed);
    adsr_pressed = pressed;

    if (pressed) {
      adsr_t0 = time;
      //adsr_t1 = adsr_t0 +

    }
    else {
      sound_value = 0;  //prevent "beep"
    }
    //compute timing
    // adsr_time_
  }
}

//---------------------------------------------------------------
inline void LFO_loop(unsigned long time) {
  if (Pin_LFO_Shape1_Changed || Pin_LFO_Shape2_Changed) {
    byte shape = Pin_LFO_Shape1*2 + Pin_LFO_Shape2;
    pr("LFO Shape "); prln(shape);
    if (shape == 0)  LFO_wave = wave_sine;
    if (shape == 1)  LFO_wave = wave_tri;
    if (shape == 2)  LFO_wave = wave_saw;
    if (shape == 3)  LFO_wave = wave_noised;
  }
  
  LFO_Range = map(Pot_LFO_Range, 0, 1023, 0, lfo_max);

  LFO_Rate = mapf(Pot_LFO_Rate, 0, 1023, lfo_speed_min, lfo_speed_max);
  static unsigned long time_prev = 0;
  unsigned long delta_ms = time - time_prev;
  lfo_phase_ += delta_ms * LFO_Rate;
  time_prev = time;

  long int phase = int(lfo_phase_) % wave_n;
  
  LFO_Value_ = map(LFO_wave[phase], -timbre_range, timbre_range, -LFO_Range, LFO_Range);

  
//  audio_step = map(wave_sine[phase], -timbre_range, timbre_range, Filter_1, Filter_2);

  //Debug print
  if (sliders_debug == DEBUG_LFO && print_now) {
    pr("LFO "); pr(LFO_Range); pr("  spd "); pr(LFO_Rate); pr(" value "); prln(LFO_Value_);
    //pr(time);
    //audio_volume = int(clampi(audio_volume_max * (sin(time*0.001)*0.5 + 0.5),0,127));
    //prln(audio_volume);

    //audio_volume = Sustain_vol;
    //audio_step = Filter_1; //audio_volume_max - Sustain_vol;// * 5;

    //if (print_now) {
    //pr("phase "); prln(phase);
    //pr("  audio_step "); prln(audio_step);
    //pr("  delta "); prln(delta_ms);
    //}
  }
}

//---------------------------------------------------------------
void sample_rate_loop() {
  
}


//---------------------------------------------------------------
inline void pedal_loop() {
  Pedal_ = mapi_clamp(Pot_Pedal_Inp, 0, Pedal_Input_Max, 0, Pot_Pedal_Sens);  

  //Debug print
  if (sliders_debug == DEBUG_LFO && print_now) {
    pr("Pedal "); prln(Pedal_); 
  }
}

//---------------------------------------------------------------
//update event
void synth_loop(unsigned long time) {
  //Enable/Disable synth controls
  //if (Pin_Synth_Mode_Changed) {
  SLIDERS_ENABLED_2 = Pin_Synth_Mode;
  //  pr("Synth: "); prln(SLIDERS_ENABLED_2);
  //}

  //Pedal
  pedal_loop();

  //LFO
  LFO_loop(time);

  //Sample Rate
  //TODO make nonlinear
  int sample_rate = Pot_Sample_Rate;

  //TODO now disabled LFO and Pedal control for sample rate, because works strange
  /*if (Pin_Enable_Pedal3 == 1) {
    int add = ((long long)(Pedal_)*Sample_Rate_Max / Pedal_Sens_Max);
    sample_rate += add;
    //prln(add); 
  }
  if (Pin_Enable_LFO3 == 1) {
    int add = ((long long)(LFO_Value_)*Sample_Rate_Max / LFO_Range);
    sample_rate += add;
    //prln(add); 
  }*/

  sample_rate = clampi(sample_rate, 0, Sample_Rate_Max);
  set_audio_sample_rate(sample_rate);

  //prln(sample_rate);

  if (sliders_debug == DEBUG_LFO && print_now) {
    pr("Sample rate "); prln(sample_rate);
  }
  
}

//---------------------------------------------------------------
