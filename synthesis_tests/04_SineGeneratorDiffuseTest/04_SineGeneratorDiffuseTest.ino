//EndlessSynth, 1-bit sine generator with diffusion
//I implemented 1 bit sound synthesis without timer.
//It's appear that sound too harch :)
//also, it's inefficient to compute time for each phase sample.
//So, in the next sketches I will use timer interrupts timer to obtain precise sample rate.
//as a final result, see 09_SineWaveInterruptsMega

//Device: Arduino Uno or Mega
//Commutation:
const byte pin_buz = 2;
//Note: Monitor port baud must be set to 500000!


//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Sine 1.1");

  Serial.print("Buzzer pin: "); Serial.println(pin_buz);

  pinMode(pin_buz, OUTPUT);
  make_wavetable();
}

//---------------------------------------------------------------
//Sine wavetable
const int wave_n = 256;
char wave_table[wave_n];

void make_wavetable() {
  int volume = 100; //0..127
  for (int i = 0; i < wave_n; i++) {
    wave_table[i] = int(sin(i * 2 * PI / wave_n) * volume); //127); //-127..127
  }

  Serial.println("Wavetable");
  for (int i = 0; i < wave_n; i++) {
    Serial.print(int(wave_table[i])); Serial.print(" ");
    if (i > 0 && i % 20 == 0) Serial.println();
  }
  Serial.println();
}

//---------------------------------------------------------------
//Sound output parameters

int sample_rate = 16000; //desired sample rate, can vary
int pause_mcs = (1000000 / sample_rate); //Delay between sound outputs. Note: real sample reate is less because of other things to process.
const unsigned int sound_chunk_mcs = 100; //duration of continuous sound playing - between them all other commands proessed

//Tone
float freq = 440; //sound frequency
unsigned long period_mcs = (unsigned long)(1000000.0 / freq); //period for wavetable

const int diff_step = 127; //step of diffusion subtraction - 1..127, kind of threshold for sound

//Generation variables
unsigned long time_start_mcs = 0; //time of sound start
int sound_value = 0;              //sound diffusion value

//---------------------------------------------------------------
inline void sound_output() {
  unsigned long time_now0 = micros();
  unsigned long time_now = time_now0;

  //unsigned long time_end = time_now + sound_chunk_mcs; //time of exit this loop

  //for time measure
  //int count = 1000;
  //for (int i = 0; i < count; i++) {

  //sound generation loop
  //while (time_now < time_end) {
  //unsigned long delta = time_now - time_start_mcs;
  //int phase = (delta % period_mcs) * wave_n / period_mcs;
  //Serial.print(delta); Serial.print(" "); Serial.print(period_mcs); Serial.print(" "); Serial.println(phase);

  int repeats = 8;
  for (int phase = 0; phase < wave_n; phase++) {
    char sample = wave_table[phase];  //TODO interpolation
    for (int i = 0; i < repeats; i++) {

      sound_value += sample;
      //Serial.print(int(sample)); Serial.print(" "); Serial.println(sound_value);

      //output audio sample to buzzer
      if (sound_value >= 0) {
        digitalWrite(pin_buz, HIGH);    //buzzer ON
        sound_value -= diff_step;       //diffusion propagation
      }
      else {
        digitalWrite(pin_buz, LOW);     //buzzer OFF
        sound_value += diff_step;       //diffusion propagation
      }
      //wait before play next sound
      //delayMicroseconds(pause_mcs);
      //delayMicroseconds(1);

      //time_now = micros(); //update current time
    }
  }

  //unsigned long time_end = micros();
  //Serial.print("cycle lenght ");
  //Serial.println((time_end - time_now0) / (repeats * wave_n));
  //Serial.println((time_end - time_now0) / (count));

  //Cycle duration:
  //7 microseconds without time measure and phase computation -> 142 kHz
  //87 microseconds with time measure and phase computation -> 12kHz

}



//---------------------------------------------------------------
void loop() {
  sound_output();
  //Serial.println(period_mcs);

  //unsigned long time_now = micros();
  //Serial.println(time_now);
}

//---------------------------------------------------------------
