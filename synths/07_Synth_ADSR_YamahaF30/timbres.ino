//Timbres
//sine, triangle, sawtooth, sine+noise


byte adsr_key = 0;             //is key pressed             
unsigned long adsr_attack = 0;  //time of pressing key

//---------------------------------------------------------------
//TODO put to const memory - print to console and after copy here
const unsigned int wave_n = 256;
char wave_sine[wave_n];
char wave_tri[wave_n];
char wave_saw[wave_n];
char wave_harsh[wave_n];    //sine wave distorted by white noise //TODO may add noise by separate slider

char *wave_table = wave_sine; //ORIGINAL TIMBRE, can be switched

//---------------------------------------------------------------
void timbres_setup() {
  int volume = 100; //0..127
  for (int i = 0; i < wave_n; i++) {
    wave_sine[i] = int(sin(i * 2 * PI / wave_n) * volume);  //-volume..volume
    wave_saw[i] = map(i, 0, wave_n - 1, -volume, volume);
    wave_tri[i] = map(absi(i - wave_n/2), 0, wave_n/2, volume, -volume); //using absi because "abs" works not correct here
    wave_harsh[i] = clampi(wave_sine[i] + (random(volume/4) - volume/8), -volume, volume);
  }

  prln("Wavetable");
    for (int i = 0; i < wave_n; i++) {
    pr(int(wave_table[i])); pr(" ");
    if (i > 0 && i % 20 == 0) prln();
    }
    prln();
  
}

//---------------------------------------------------------------
byte old_timbre = 255;
void set_timbre(byte index) {
  if (index != old_timbre) {
    if (index == 0) wave_table = wave_sine;
    if (index == 1) wave_table = wave_tri;
    if (index == 2) wave_table = wave_saw;
    if (index == 3) wave_table = wave_harsh;
    old_timbre = index;
    if (debug) {
      pr("timbre "); prln(index);
   }
  }
}

//---------------------------------------------------------------
void set_timbre_slider(int slider) {
  
  
}

//---------------------------------------------------------------
