//Timbres
//triangle, sine, sawtooth, noised


//---------------------------------------------------------------
//TODO put to const memory - print to console and after copy here
const unsigned int wave_n = 256;
char wave_sine[wave_n];
char wave_tri[wave_n];
char wave_saw[wave_n];
char wave_noised[wave_n];    //TODO may add noise by separate slider

char *wave_table = wave_noised; //ORIGINAL TIMBRE, can be switched

//---------------------------------------------------------------
void timbres_setup() {
  int volume = 100; //0..127
  for (int i = 0; i < wave_n; i++) {
    wave_sine[i] = int(sin(i * 2 * PI / wave_n) * volume);  //-volume..volume
    wave_tri[i] = map(absi(i - wave_n / 2), 0, wave_n / 2, volume, -volume); //using absi because "abs" works not correct here
    wave_saw[i] = map(i, 0, wave_n - 1, -volume, volume);
    wave_noised[i] = map(abs(i - wave_n / 2), 0, wave_n / 2, volume, -volume);  //here we use strange "abs" behavior, see ths printing below in comments
    //clampi(wave_saw[i] + wave_saw[(i*3)%wave_n]/2, -volume, volume);
    //(random(volume / 4) - volume / 8)
  }

  /*prln("Wavetable");
  for (int i = 0; i < wave_n; i++) {
    pr(int(wave_table[i])); pr(" ");
    if (i > 0 && i % 40 == 0) prln();
  }
  prln();*/

}

/* Noised wavetable (to understand what's this):
44 43 41 40 38 37 35 34 32 30 29 27 26 24 23 21 19 18 16 15 13 12 10 9 7 5 4 2 1 -1 -2 -4 -6 -7 -9 -10 -12 -13 -15 -16 -18 
-20 -21 -23 -24 -26 -27 -29 -31 -32 -34 -35 -37 -38 -40 -41 -43 -45 -46 -48 -49 -51 -52 -54 -56 -57 -59 -60 -62 -63 -65 -66 -68 -70 -71 -73 -74 -76 -77 -79 -81 
-82 -84 -85 -87 -88 -90 -91 -93 -95 -96 -98 -99 -101 -102 -104 -106 -107 -109 -110 -112 -113 -115 -116 -118 -120 -121 -123 -124 -126 -127 127 125 124 122 121 119 118 116 115 113 
111 110 108 107 105 104 102 100 99 97 96 94 93 91 90 88 86 85 83 82 80 79 77 75 74 72 71 69 68 66 65 63 61 60 58 57 55 54 52 50 
49 47 46 44 43 41 40 38 36 35 33 32 30 29 27 25 24 22 21 19 18 16 15 13 11 10 8 7 5 4 2 0 -1 -3 -4 -6 -7 -9 -10 -12 
-14 -15 -17 -18 -20 -21 -23 -25 -26 -28 -29 -31 -32 -34 -35 -37 -39 -40 -42 -43 -45 -46 -48 -50 -51 -53 -54 -56 -57 -59 -60 -62 -64 -65 -67 -68 -70 -71 -73 -75 
-76 -78 -79 -81 -82 -84 -85 -87 -89 -90 -92 -93 -95 -96 -98 
 */

//---------------------------------------------------------------
byte old_timbre = 255;
void set_timbre(byte index) {
  if (index != old_timbre) {
    if (index == 0) wave_table = wave_tri;
    if (index == 1) wave_table = wave_sine;
    if (index == 2) wave_table = wave_saw;
    if (index == 3) wave_table = wave_noised;
    old_timbre = index;
    if (debug) {
      pr("timbre "); prln(index);
    }
  }
}

//---------------------------------------------------------------
void set_timbre_slider(int slider) {
  //values from slider:
  //5 433 926 1017
  if (slider < 210) {
    set_timbre(0);
    return;
  }
  if (slider < 700) {
    set_timbre(1);
    return;
  }
  if (slider < 980) {
    set_timbre(2);
    return;
  }
  set_timbre(3);
}

//---------------------------------------------------------------
