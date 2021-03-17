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

//get timbres for lfo shape
extern const unsigned int wave_n;
extern const int timbre_range;

extern char wave_sine[];
extern char wave_tri[];
extern char wave_saw[];
extern char wave_noised[];

//---------------------------------------------------------------
void ADSR_setup() {
  audio_volume = 0;
}


//---------------------------------------------------------------
//Sliders event
//int Attack_ms = 0;
//int Decay_ms = 0;
//int Sustain_vol = 0;
//int Release_ms = 0;

//Filter params
const int filter_min = 1;
const int filter_max = timbre_range*127/100;

const float lfo_speed_min = 0;
const float lfo_speed_max = wave_n * 5 * .001f; //10*1000;  //Hz*1000


//Filter current state
int Filter_1 = 127;
int Filter_2 = 127;
float Filter_LFO_Speed = 0;
int Filter_LFO_Shape = 0; //sine, sawtooth

float lfo_phase_ = 0;



//-------------------------------------------------------------------------
void set_adsr_sliders(int Attack_slider, int Decay_slider, int Sustain_slider, int Release_slider) {


  Filter_1 = map(Attack_slider, 0, 1023, filter_max, filter_min);
  Filter_2 = map(Decay_slider, 0, 1023, filter_max, filter_min);

  Filter_LFO_Speed = mapf(Sustain_slider, 0, 1023, lfo_speed_min, lfo_speed_max);
  /*
    const int min_vol = 1; //64;   //NOTE: LOWER - will beep
    Sustain_vol = map(Sustain_slider, 0, 1023, min_vol, audio_volume_max);
    Release_ms = map(Release_slider, 0, 1023, 0, 500);
  */

  if (debug_now) {
    pr("Filter_LFO_Speed "); prln(Filter_LFO_Speed);
    //pr("ADSR "); pr(Attack_ms); pr(" "); pr(Decay_ms); pr(" "); pr(Sustain_vol); pr(" "); pr(Release_ms);
    //prln();
  }
}


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
//update event
void ADSR_loop(unsigned long time) {
  unsigned long time_prev = 0;
  //LFO
  lfo_phase_ += (time - time_prev) * Filter_LFO_Speed;
  time_prev = time;

  long int phase = int(lfo_phase_) % wave_n;

  audio_step = map(wave_sine[phase],-timbre_range,timbre_range,Filter_1,Filter_2);
  
  //pr(time);
  //audio_volume = int(clampi(audio_volume_max * (sin(time*0.001)*0.5 + 0.5),0,127));
  //prln(audio_volume);

  //audio_volume = Sustain_vol;
  //audio_step = Filter_1; //audio_volume_max - Sustain_vol;// * 5;

  if (debug_now) {
      pr("phase "); pr(phase);  
      pr("  audio_step "); prln(audio_step); 
      pr("  delta "); prln(time - time_prev);
  }

}

//---------------------------------------------------------------
