//---------------------------------------------------------------
//Potentiometers
//Use five 10 KOhm linear pots
//- Connect Arduino's Gnd and 5V to pots inputs, 
//and Arduino's pins A1,A2,A3,A4,A5 to pot's outputs.
//This pots control Pitch, Attack, Release, PWM, Octave.
//---------------------------------------------------------------

const byte pots = 5;
const byte pin_pots[pots] = {A1, A2, A3, A4, A5};

int pots_values[pots] = {0,0,0,0,0}; //0..1023


//---------------------------------------------------------------
//Functions for receiving pots values
//---------------------------------------------------------------

//Pitch, -100..100, in percents
int get_pitch() {
  //we get -150..150 and then collapse in to -100..100 to obtain zero-range at pot's center
  int p = map(pots_values[0],0,1023,-150,150); 
  if (abs(p)<50) return 0;
  if (p > 0) return p-50;
  return p+50;
}

//---------------------------------------------------------------
//Attack time, 0..2000 milliseconds
int get_attack() {
  return map(pots_values[1],0,1023,0,2000); 
}

//---------------------------------------------------------------
//Release time, 0..2000 milliseconds
int get_release() {
  return map(pots_values[2],0,1023,0,2000); 
}

//---------------------------------------------------------------
//PWM, 0..255 - range is specific of sound generation implementation, see file "sound"
int get_pwm() {
  return map(pots_values[3],0,1023,0,255); 
}

//---------------------------------------------------------------
//Octave, -2..2
int get_octave() {
  return map(pots_values[4],0,1023,-2,2); 
}

//---------------------------------------------------------------
// Pots setup
//---------------------------------------------------------------
void pots_setup() {
  //Nothing here
}

//---------------------------------------------------------------
// Pots update
//---------------------------------------------------------------
void pots_update() {
  for (byte i=0; i<pots; i++) {
    pots_values[i] = analogRead(pin_pots[i]);
  }

  //Debug print - uncomment it for printing pots values
  Serial.print("Pots:\t"); 
  for (byte i=0; i<pots; i++) {
    Serial.print(pots_values[i]);
    Serial.print("\t");
  }
  Serial.print("Pitch: "); Serial.print(get_pitch());
  Serial.print("\tAttack: "); Serial.print(get_attack());
  Serial.print("\tRelease: "); Serial.print(get_release());
  Serial.print("\tPWM: "); Serial.print(get_pwm());
  Serial.print("\tOctave: "); Serial.print(get_octave());
  Serial.println();
}

//---------------------------------------------------------------
