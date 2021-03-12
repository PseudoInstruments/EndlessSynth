//ADSR envelope 
//It works at control rate
//The ADSR parameters are computed at key pressing and not changing until all keys are released.
//ASDR processes all keys using single envelope, which starts on first key is pressed and ends an all keys are released.

//affects audio_volume value

byte adsr_pressed = 0;             //is key pressed             
unsigned long adsr_attack = 0;  //time of pressing key

//---------------------------------------------------------------
//value from xsound - volume of sound
extern int audio_volume;
extern const int audio_volume_max;

void ADSR_setup() {
  audio_volume = 0;
}


//---------------------------------------------------------------
//event: key pressed or released
void ADSR_key_event(unsigned int time, byte pressed) {
  if (pressed != adsr_pressed) {
    pr("ADSR event "); prln(pressed);
    adsr_pressed = pressed;
  }  
    //compute timing
   // adsr_time_
    
}

//---------------------------------------------------------------
//update event
void ADSR_loop(unsigned int time) {
  //pr(time);
  audio_volume = int(clampi(audio_volume_max * (sin(time*0.001)*0.5 + 0.5),0,127));
  //prln(audio_volume);
}

//---------------------------------------------------------------
