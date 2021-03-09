//Sliders

//8 potentiometers 10 kOhm:
//1 Tone
//2 Attack       (ADSR settings)
//3 Decay
//4 Sustain
//5 Release
//6 Sample Rate  (Timbre settings)
//7 Diffusion
//8 [Reserved]
//Note: If you have no sliders, set SLIDERS_ENABLED=0 in the main sketch file.

//Note: nonlinearity - slider slows start TODO linearize sliders values


//Connection:
//("apin" means "analog pin")
const byte apin_Tone = A2;
const byte apin_Attack = A3;
const byte apin_Decay = A4;
const byte apin_Sustain = A5;
const byte apin_Release = A6;

const byte apin_Sample_rate = A8;
const byte apin_Diffusion = A9;
const byte apin_Reserved = A10;      //not used

const byte slider_Gnd_pin = 4;  //Gnd for sliders
const byte slider_5V_pin = 5;   //+5V for sliders

//---------------------------------------------------------------
void sliders_setup() {
  if (SLIDERS_ENABLED)  {
    prln("Sliders Tone: A2, ADSR: A3456, sample rate: A8, Diffusion: A9, [Reserved] A10, sliders power: pin 4 as Gnd, pin 5 as 5V");
  }
  else prln("[Sliders disabled]");

  if (SLIDERS_ENABLED) {
    //set up pins
    pinMode(slider_5V_pin, OUTPUT);
    pinMode(slider_Gnd_pin, OUTPUT);
    digitalWrite(slider_5V_pin, HIGH);
    digitalWrite(slider_Gnd_pin, LOW);
  }
}

//---------------------------------------------------------------
void sliders_loop() {
  //set diff step and Diffusion propagation
  int Tone = (SLIDERS_ENABLED) ? analogRead(apin_Tone) : 512; //0..1023
  int Attack = (SLIDERS_ENABLED) ? analogRead(apin_Attack) : 0;
  int Decay = (SLIDERS_ENABLED) ? analogRead(apin_Decay) : 1023;
  int Sustain = (SLIDERS_ENABLED) ? analogRead(apin_Sustain) : 0;
  int Release = (SLIDERS_ENABLED) ? analogRead(apin_Release) : 0;

  int Sample_rate = (SLIDERS_ENABLED) ? analogRead(apin_Sample_rate) : 0;
  int Diffusion = (SLIDERS_ENABLED) ? analogRead(apin_Diffusion) : 512;
  int Reserved = (SLIDERS_ENABLED) ? analogRead(apin_Reserved) : 512;


  sound_set_sliders(Tone, Sample_rate);

  //Debug print
  if (sliders_debug) {
    static byte t = 0;
    t = (t + 1) % 10;
    if (t == 0) {
      pr("Tone="); pr(Tone);
      pr(", Attack="); pr(Attack);
      pr(", Decay="); pr(Decay);
      pr(", Sustain="); pr(Sustain);
      pr(", Release="); pr(Release);

      pr(", Sample_rate="); pr(Sample_rate);
      pr(", Diffusion="); pr(Diffusion);
      pr(", Reserved="); pr(Reserved);

      prln("");
    }
  }
}


//---------------------------------------------------------------
