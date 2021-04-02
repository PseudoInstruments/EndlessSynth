//Sound generation

//--------------------------------------------------------------
//Sample rate

int audio_step_mcs_ = 30;    //estimation of audio loop - performed at start in sound_measure_performance()

//We will control sample rate in note scale
const float sample_rate_note0 = 60;   //60 -> 261.63 Hz
const float sample_rate_note1 = 114;  //114 -> 5919.91 Hz
//Table of sample rates   //TODO make as const
const int sample_rates_n = 256;
int sample_rates[sample_rates_n];

int sample_rate_ = 500;

int audio_delay_mcs = 10;   //delay in sound loop, controls sample rate

const int loops0 = 10;
const int loops1 = 500;
int loops_ = 300;   //number of steps before control step

int skip_n_ = 5;    //fast / slow loops relation


//--------------------------------------------------------------
//Drum sample
const int wave_n = 512;
byte wave[wave_n];   //TODO store in bits

//controllable
const int wave_duration_min = 10;
int wave_duration = wave_n; //0..wave_n

//sample pos
int pos_ = wave_n;



//--------------------------------------------------------------
//Measure performance of audio_loop
//Note: do this performance measure before sliders setup
void sound_measure_performance() {
  unsigned long time0 = micros();

  //fake sound start
  sound_play(0);
  int n = 100;
  for (int i=0; i<n; i++) {
    sound_audio_step();
  }

  pos_ = wave_n;  //stop playing

  //Result
  audio_step_mcs_ = (micros() - time0) / n;
  
  Serial.print("[Performance measurement: audio step works "); 
  Serial.print(audio_step_mcs_);
  Serial.println(" mcs]");

}

//--------------------------------------------------------------
void init_wave() {
  for (int i = 0; i < wave_n; i++) {
    wave[i] = (random(100) < 50) ? 0 : 1;
    //Serial.print(wave[i]);
    //Serial.print(" ");
  }
}

//--------------------------------------------------------------
//make table of sample rates
void init_sample_rates() {
  for (int i = 0; i < sample_rates_n; i++) {
    float note = mapf(i, 0, sample_rates_n, sample_rate_note0, sample_rate_note1);
    sample_rates[i] = m_to_f_float(note);
  }

  Serial.print("Sample rates: "); Serial.print(sample_rates[0]); 
  Serial.print(" - "); Serial.println(sample_rates[sample_rates_n-1]);
  
  //for (int i=0; i<sample_rates_n; i++) {
  //  Serial.print(i); Serial.print(" "); Serial.println(sample_rates[i]);
  //}
}

//--------------------------------------------------------------
void sound_setup() {
  init_sample_rates();
  init_wave();
  
}


//--------------------------------------------------------------
inline void sound_control_step() {
  //Sample rate and number of loops
  if (&Pot_Sample_Rate_Changed) {
    float sample_note = mapf(*Pot_Sample_Rate, pot_min, pot_max, sample_rate_note0, sample_rate_note1);
    sample_rate_ = sample_rates[mapi_clamp(*Pot_Sample_Rate, pot_min, pot_max, 0, sample_rates_n-1)];

    const int FPS_slow = 5;
    const int FPS_fast = 100;

    loops_ = constrain(sample_rate_ / FPS_slow, 1, 10000);

    int skip_loops = constrain(sample_rate_ / FPS_fast, 1, 10000);
    skip_n_ = constrain(loops_ / skip_loops, 1, 10000);


    audio_delay_mcs = constrain(1000000 / sample_rate_ - audio_step_mcs_, 1, 1000000);
    //audio_delay_mcs = mapi_clamp(*Pot_Sample_Rate, pot_min, pot_max, audio_delay_mcs0, audio_delay_mcs1);
  }

  //Sound duration
  wave_duration = mapi_clamp(*Pot_Drum1_Duration, pot_min, pot_max, wave_duration_min, wave_n);
  
}

//--------------------------------------------------------------
inline void sound_play(int id) {
  pos_ = 0;
}

//--------------------------------------------------------------
//one sound step
inline void sound_audio_step() {
  //audio output
  if (pos_ < wave_duration) {
    if (wave[pos_]) {
      digitalWrite(audio_pin, HIGH);    //buzzer ON
    }
    else {
      digitalWrite(audio_pin, LOW);    //buzzer OFF
    }

    pos_++;
  }
  else {
    pos_ = wave_n;                  //prevent continuation of sound play
    digitalWrite(audio_pin, LOW);    //buzzer OFF
  }
  //delay
  delayMicroseconds(audio_delay_mcs);

}

//--------------------------------------------------------------
inline void sound_audio_loop() {
  static int skip_pos_ = 0;  //for fast control step

  for (int i = 0; i < loops_; i++) {
    skip_pos_++;
    skip_pos_ %= skip_n_;
    if (skip_pos_ == 0) {
      sliders_fast_control_step();      //read pedal and key presses
    }
    sound_audio_step();
  }
}

//--------------------------------------------------------------
void sound_debug_print() {
  
  Serial.print("  sample_rate="); Serial.print(sample_rate_);
  Serial.print("  audio_delay_mcs="); Serial.print(audio_delay_mcs);
  Serial.print(" loops="); Serial.print(loops_);
  Serial.print("  wave_duration="); Serial.print(wave_duration);



}
//--------------------------------------------------------------
