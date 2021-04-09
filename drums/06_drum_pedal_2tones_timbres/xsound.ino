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
//Drum samples
const int wave_N = 3000;
byte wave1[wave_N];   //TODO store in bits
byte wave2[wave_N];
int wave1_n = wave_N;
int wave2_n = wave_N;

byte *wave = wave1;
int *wave_n = &wave1_n;


//sample pos
int pos_ = wave_n;

//timbre
const float square_note0 = 34;   //34 -> 58 Hz
const float square_note1 = 84;  //84 -> 1046,  72 -> 523 Hz
const float square_delta = square_note1 - square_note0;

//--------------------------------------------------------------
//Measure performance of audio_loop
//Note: do this performance measure before sliders setup
void sound_measure_performance() {
  unsigned long time0 = micros();

  //fake sound start
  sound_play(0);
  int n = 100;
  for (int i = 0; i < n; i++) {
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
//make table of sample rates
void init_sample_rates() {
  for (int i = 0; i < sample_rates_n; i++) {
    float note = mapf(i, 0, sample_rates_n-1, sample_rate_note0, sample_rate_note1);
    sample_rates[i] = m_to_f_float(note);
  }

  Serial.print("Sample rates: "); Serial.print(sample_rates[0]);
  Serial.print(" - "); Serial.println(sample_rates[sample_rates_n - 1]);

  //for (int i=0; i<sample_rates_n; i++) {
  //  Serial.print(i); Serial.print(" "); Serial.println(sample_rates[i]);
  //}
}

//--------------------------------------------------------------
void sound_setup() {
  init_sample_rates();

  Serial.print("Tone frequencies range: "); Serial.print(m_to_f_int(square_note0));
  Serial.print(" - "); Serial.println(m_to_f_int(square_note1));

  init_drum1();
  init_drum2();
}

//--------------------------------------------------------------
void init_wave(byte *wave, int &wave_n, int duration_slider, int timbre_slider, int tone1_slider, int tone2_slider) {
  int duration_ms = mapi_clamp(duration_slider, pot_min, pot_max, 10, 500);

  float note0 = mapf_clamp(tone1_slider, pot_min, pot_max, square_note0, square_note1); 
  float note1 = clampf(note0 + mapf(tone2_slider, pot_min, pot_max, -square_delta, +square_delta), square_note0, square_note1); 
   
  int n_max = (long long)(sample_rate_) * duration_ms / 1000;
  wave_n = min(n_max, wave_N);

  //float phase = 0;
  //float phase_adder;    //period 1 - <0.5->0, >=0.5->1
  int phase = 0;
  int phase_adder;    //period sample_rate_: 0..sample_rate/2 -> 0, else -> 1
  int sample_rate2 = sample_rate_ / 2;

  //timbre, 0 - tone, 127 - noise
  int timbre_noise = mapi_clamp(timbre_slider, pot_min, pot_max, 0, 127); 
  int timbre_tone = 127 - timbre_noise; 
  
  for (int i = 0; i < wave_n; i++) {
    float ton = mapf(i, 0, wave_n-1, note0, note1);
    int freq = m_to_f_int(ton);
    //phase_adder = float(freq) / sample_rate_;
    phase_adder = freq;

    int sound = timbre_tone*((phase < sample_rate2) ? -1 : 1) + random(-timbre_noise,timbre_noise);
    wave[i] =  (sound>0) ? 1 : 0;
    phase += phase_adder;
    phase %= sample_rate_; 
  }

  //Debug print
  //Serial.println("Drum update");
  //Serial.print("duration_ms "); Serial.println(duration_ms);
  //Serial.print("wave_n "); Serial.println(wave_n);
  //Serial.print("Freq "); Serial.print(m_to_f_int(note0)); Serial.print(" -- "); Serial.println(m_to_f_int(note1));
  //Serial.print("Timbre noise "); Serial.println(timbre_noise);

  /*for (int i=0; i<wave_n; i++) {
    Serial.print(wave[i]); Serial.print(" ");
    if (i % 40 == 0 && i > 0) Serial.println(); 
  }
  Serial.println(); */

  //Serial.println("MIDI to freq:");
  //for (int i=0; i<127; i++) {
  //  Serial.print(i); Serial.print(" ");Serial.println(m_to_f_int(i)); 
  //}
}

//--------------------------------------------------------------
void init_drum1() {
  init_wave(wave1, wave1_n, *Pot_Drum1_Duration, *Pot_Drum1_Timbre, *Pot_Drum1_Tone1, *Pot_Drum1_Tone2);
}

void init_drum2() {
  init_wave(wave2, wave2_n, *Pot_Drum2_Duration, *Pot_Drum2_Timbre, *Pot_Drum2_Tone1, *Pot_Drum2_Tone2);
}

//--------------------------------------------------------------
inline void sound_control_step() {
  //Sample rate and number of loops
  if (&Pot_Sample_Rate_Changed) {
    float sample_note = mapf(*Pot_Sample_Rate, pot_min, pot_max, sample_rate_note0, sample_rate_note1);
    sample_rate_ = sample_rates[mapi_clamp(*Pot_Sample_Rate, pot_min, pot_max, 0, sample_rates_n - 1)];

    const int FPS_slow = 10;
    const int FPS_fast = 100;

    loops_ = constrain(sample_rate_ / FPS_slow, 1, 10000);

    int skip_loops = constrain(sample_rate_ / FPS_fast, 1, 10000);
    skip_n_ = constrain(loops_ / skip_loops, 1, 10000);


    audio_delay_mcs = constrain(1000000 / sample_rate_ - audio_step_mcs_, 1, 1000000);
    //audio_delay_mcs = mapi_clamp(*Pot_Sample_Rate, pot_min, pot_max, audio_delay_mcs0, audio_delay_mcs1);
  }

  //Drum 1
  if (*Pot_Sample_Rate_Changed
      || *Pot_Drum1_Duration_Changed || *Pot_Drum1_Timbre_Changed
      || *Pot_Drum1_Tone1_Changed || *Pot_Drum1_Tone2_Changed) {
    init_drum1();
  }

  //Drum 2
  if (*Pot_Sample_Rate_Changed
      || *Pot_Drum2_Duration_Changed || *Pot_Drum2_Timbre_Changed
      || *Pot_Drum2_Tone1_Changed || *Pot_Drum2_Tone2_Changed) {
    init_drum2();
  }
}

//--------------------------------------------------------------
inline void sound_play(int id) {
  if (id == 0) {
    wave = wave1;
    wave_n = &wave1_n;
  }
  else {
    wave = wave2;
    wave_n = &wave2_n;    
  }
  
  pos_ = 0;
}

//--------------------------------------------------------------
//one sound step
inline void sound_audio_step() {
  //audio output
  if (pos_ < *wave_n) {
    if (wave[pos_]) {
      digitalWrite(audio_pin, HIGH);    //buzzer ON
    }
    else {
      digitalWrite(audio_pin, LOW);    //buzzer OFF
    }

    pos_++;
  }
  else {
    pos_ = wave_N;                  //prevent continuation of sound play
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

}
//--------------------------------------------------------------
