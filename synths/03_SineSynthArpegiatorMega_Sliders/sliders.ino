//This file contains working with sliders

const byte slider_1_pin = A4;
const byte slider_2_pin = A4;
const byte slider_5V_pin = 4;
const byte slider_Gnd_pin = 5;

//---------------------------------------------------------------
void sliders_setup() {
  Serial.println("   sliders pins: A4,A5, sliders power: pin 4 as 5V, pin 5 as Gnd");

  //set up pins
  pinMode(slider_5V_pin, OUTPUT);
  pinMode(slider_Gnd_pin, OUTPUT);
  digitalWrite(slider_5V_pin, HIGH);
  digitalWrite(slider_Gnd_pin, LOW);
}

//---------------------------------------------------------------
void sliders_loop() {
  //set diff step and diffusion propagation
  int alg_step10bit = analogRead(A4); //0..1023 - kind of "level of sound generation", the higher - the more dithering
  int alg_diff10bit = 1023-analogRead(A5); //0..1023 - duration of dithering, the lower - the more harshness, simple <0,>0 alg
  
  sound_set_1bitparams(alg_step10bit, alg_diff10bit);

  //Debug print - uncomment to print sliders values
  //Serial.print("slider1 "); Serial.print(alg_step10bit);
  //Serial.print("  slider2 "); Serial.print(alg_diff10bit);
  //Serial.println();

}

//---------------------------------------------------------------
