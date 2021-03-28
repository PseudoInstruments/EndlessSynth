//Constants

//---------------------------------------------------------------
//Sound Engine
//---------------------------------------------------------------
//Audio sample rate - main parameter of sound engine.
//It's the frequency of sound pulses.
//Use lower values if not enough computing power; currently we use power of 2 to faster divide

const long audio_sample_rate0 = 2000; 
const long audio_sample_rate1 = 8000;    //More frame rate gives overflow and three notes freeze synth
const long audio_sample_rate_step = 20; //"big" difference when change frame rate

//----------------------------------------------------------
//Sound generation
//----------------------------------------------------------
//1-bit sound diffusion parameters

//Audio volume - changed by ADSR
const int audio_volume_min = 0;    
const int audio_volume_max = 127;
const int audio_volume_shift = 7; // << 7 instead "/audio_volume_max"

  
//decaying diffusion 0..256, 0 - no diffusion, 256 - keep all diffusion
const int diff_keep_max = 256;
const int diff_keep_shift = 8;




//---------------------------------------------------------------
