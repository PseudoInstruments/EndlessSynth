//---------------------------------------------------------------
//Keyboard
//---------------------------------------------------------------

//- Connect Arduino's pins 5,4,3,2 to keyboard's ribbon cable with 4 contacts - for choosing keys blocks
//- Сonnect Arduino's pins 6,7,8,9,10,11,12,13 to to keyboard's ribbon cable with 8 contacts - for choosing key inside block
const byte blocks = 4;
const byte pin_blocks[blocks] = {5,4,3,2};
const byte keys_in_block = 8;
const byte pin_keys[keys_in_block] = {6,7,8,9,10,11,12,13};

const byte keys = 32; //number of keys

//Base note - it's used together with octave to decide which key to play
const byte base_note = 53-12; //F 

byte keyboard_note = 0; //currently pressed note, 0 - not pressed


//---------------------------------------------------------------
// Keyboard setup
void keyboard_setup() {
  for (byte i = 0; i < blocks; i++) {
    pinMode(pin_blocks[i], INPUT);  //really it will OUTPUT when required
  }
  for (byte  i = 0; i < keys_in_block; i++) {
    pinMode(pin_keys[i], INPUT_PULLUP);  //we do pullup because will send "GND" for checking pressing
  }
}

//---------------------------------------------------------------
// Keyboard update
// If several keys are pressed, it will use rightmost key.
void keyboard_update() {
  byte key = 255;   //255 - no key, 0..keys-1 - some key is pressed
  //Scan all blocks
  for (byte b = 0; b < blocks; b++) {

    //put current to the block
    pinMode(pin_blocks[b], OUTPUT);
    digitalWrite(pin_blocks[b], LOW);

    //small pause for setting current - looks it's not required
    delayMicroseconds(2);

    for (byte k=0; k<keys_in_block; k++) {
      byte v = (digitalRead(pin_keys[k]) == LOW) ? 1 : 0;
      if (v) {
        key = b*keys_in_block + k;
        // Debug print - uncomment it for printing keyboard response
        //Serial.print("raw key "); Serial.println(key);        
      }
    }

    //disable current from the block, see NOTE above
    pinMode(pin_blocks[b], INPUT);
  }

  // If some key is pressed - move to proper octave and check if it was changed
  if (key != 255) {
    byte note = key + base_note + get_octave()*12;
    // Case key is pressed
    if (!keyboard_note) {
      key_pressed(note); //key newly pressed - start sounding with attack
    }
    else {
      if (keyboard_note != note) {
        key_changed(note); //key changed - just change tone, without attack
      }
    }
    keyboard_note = note;
  }
  else {
    //no key pressed, check if it was released
    if (keyboard_note) {
      keyboard_note = 0;
      key_released();   //key released - start sound to fade
    }
  }
  
}



//---------------------------------------------------------------
