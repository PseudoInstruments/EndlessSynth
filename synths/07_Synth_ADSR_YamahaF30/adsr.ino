//ADSR envelope 
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

void ADSR_setup() {
  audio_volume = 0;
}


//---------------------------------------------------------------
//Sliders event
int Attack_ms = 0;
int Decay_ms = 0;
int Sustain_vol = 0;
int Release_ms = 0;

void set_adsr_sliders(int Attack_slider, int Decay_slider, int Sustain_slider, int Release_slider) {
  
  Attack_ms = map(Attack_slider, 0, 1023, 0, 500);
  Decay_ms = map(Decay_slider, 0, 1023, 0, 500);

  const int min_vol = 64;   //NOTE: LOWER - will beep
  Sustain_vol = map(Sustain_slider, 0, 1023, min_vol, audio_volume_max);
  Release_ms = map(Release_slider, 0, 1023, 0, 500);

  pr("ADSR "); pr(Attack_ms); pr(" "); pr(Decay_ms); pr(" "); pr(Sustain_vol); pr(" "); pr(Release_ms);
  prln();
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
void ADSR_loop(unsigned int time) {
  //pr(time);
  //audio_volume = int(clampi(audio_volume_max * (sin(time*0.001)*0.5 + 0.5),0,127));
  //prln(audio_volume);
  audio_volume = Sustain_vol;
  
}

//---------------------------------------------------------------
