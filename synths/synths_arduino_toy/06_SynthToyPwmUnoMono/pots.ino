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
// Pots setup
void pots_setup() {
  //Nothing here
}

//---------------------------------------------------------------
// Pots update
void pots_update() {
  for (byte i=0; i<pots; i++) {
    pots_values[i] = analogRead(pin_pots[i]);
  }

  //Debug print   
  Serial.print("Pots:\t"); 
  for (byte i=0; i<pots; i++) {
    Serial.print(pots_values[i]);
    Serial.print("\t");
  }
  Serial.println();
}

//---------------------------------------------------------------
