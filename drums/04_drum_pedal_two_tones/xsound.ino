//Sound generation




//--------------------------------------------------------------
void sound_setup() {
  init_wave();
}

//--------------------------------------------------------------
//Sample rate
const int audio_delay_mcs0 = 4000;
const int audio_delay_mcs1 = 0;//400;
int audio_delay_mcs = 10;   //delay in sound loop, controls sample rate

const int loops0 = 10;
const int loops1 = 500;
int loops_ = 300;   //number of steps before control step

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
void init_wave() {
  for (int i = 0; i < wave_n; i++) {
    wave[i] = (random(100) < 50) ? 0 : 1;
    //Serial.print(wave[i]);
    //Serial.print(" ");
  }

}

//--------------------------------------------------------------
inline void sound_control_step() {
  audio_delay_mcs = mapi_clamp(*Pot_Sample_Rate, pot_min, pot_max, audio_delay_mcs0, audio_delay_mcs1);
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
  //loops_ = map(audio_delay_mcs, audio_delay_mcs0, audio_delay_mcs1, loops0, loops1); 
  //NOTE - this simple formula gives good sound but in the middle part of slider 2 it freezes a control rate a bit

  long int freq = (1000000 / (audio_delay_mcs + 100));
  //Serial.println(freq);

  const int FPS_slow = 5;
  const int FPS_fast = 100;

  loops_ = constrain(freq / FPS_slow, 1, 10000);
  //loops_ = constrain(loops_, 30, 1000);

  int skip_loops = constrain(freq / FPS_fast, 1, 10000);
  int skip_n = constrain(loops_ / skip_loops, 1, 10000);
  static int skip_pos_ = 0;  //for fast control step

  //unsigned long time0 = micros();

  for (int i = 0; i < loops_; i++) {
    skip_pos_++;
    skip_pos_ %= skip_n;
    if (skip_pos_ == 0) {
      sliders_fast_control_step();      //read pedal and key presses
    }
    sound_audio_step();
  }

  //estimate sample rate, runs once
  //static byte was_measured = 0;
  //if (!was_measured) {
  //  was_measured = 1;
  //  unsigned long delta = micros() - time0;
  //  long int audio_sample_rate = 1000000.0 * loops_ / delta;
  //  Serial.print("Computed audio sample rate: "); Serial.println(audio_sample_rate);
  //}

}

//--------------------------------------------------------------
void sound_debug_print() {
  Serial.print("  audio_delay_mcs="); Serial.print(audio_delay_mcs);
  Serial.print("  wave_duration="); Serial.print(wave_duration);

  Serial.print(" loops="); Serial.print(loops_);

}
//--------------------------------------------------------------
