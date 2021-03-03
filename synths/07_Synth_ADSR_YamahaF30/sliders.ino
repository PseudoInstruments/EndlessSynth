//This file contains working with sliders
//http://newt.phys.unsw.edu.au/jw/notes.html

const byte slider_1_pin = A4;
const byte slider_2_pin = A4;
const byte slider_5V_pin = 4;
const byte slider_Gnd_pin = 5;

//---------------------------------------------------------------
void sliders_setup() {
  if (SLIDERS_ENABLED)  Serial.println("   sliders pins: A4,A5, sliders power: pin 4 as 5V, pin 5 as Gnd");
  else Serial.println("[Sliders disabled]");
 
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
  //set diff step and diffusion propagation
  int slider2 = (SLIDERS_ENABLED) ? analogRead(A4) : 0; //0..1023 - sample rate
  int slider3 = (SLIDERS_ENABLED) ? analogRead(A5) : 512; //0..1023 - tone adjustment

  sound_set_1bitparams(slider2, slider3);

  //Debug print
  if (SLIDERS_ENABLED) {
    if (debug_now) {
      Serial.print("slider2 "); Serial.print(slider2);
      Serial.print("  slider3 "); Serial.print(slider3);
      Serial.println();
    }
  }
}


//---------------------------------------------------------------
