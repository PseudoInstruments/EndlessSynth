//Toy Piano Keyboard Reader

//Connection:
//Toy Piano sensor was prepared: I made small holes and solder wires to each key.
//So we have 32 out pins and pin for GND

//Device: Arduino Mega
//Connection:
//- 32 keys and Gnd from Toy Piano to Arduino Mega:
//22,24,26,...,36,
//23,25,27,...,37,
//38,40,42,...,52,
//39,41,43,...,53
//and Gnd
//(key numbers are reverted)

const int keys = 32;
int keyReadPin[keys] = {22, 24, 26, 28, 30, 32, 34, 36,
                        23, 25, 27, 29, 31, 33, 35, 37,
                        38, 40, 42, 44, 46, 48, 50, 52,
                        39, 41, 43, 45, 47, 49, 51, 53
                       };

const byte note_keys_count = keys - 8; //notes used for notes
const byte string_keys_indices[POLYPHONY] = {26, 28, 30, 31}; //"string" keys

//Starting midi note
byte midi_note_0 = 65; //F

//---------------------------------------------------------------
void keyboard_setup() {
  //Set
  for (int i = 0; i < keys; i++) {
    pinMode(keyReadPin[i], INPUT_PULLUP);
  }
  //Print
  Serial.print("Keys pins: ");
  for (int i = 0; i < keys; i++) {
    Serial.print(keyReadPin[i]); Serial.print(" ");
  }
  Serial.println();
  Serial.println("Now press the keys!");

}

//---------------------------------------------------------------
byte key_state[keys]; //key state - pressed or released
//TODO can store in a bit mask

//pressed "note keys"
byte note_keys[POLYPHONY];
byte note_keys_n = 0;
//mask of pressed "string keys"
byte string_keys[POLYPHONY];

//---------------------------------------------------------------
//process pressing key
void key_pressed(byte key) {
  //this is note key
  if (key < note_keys_count) {
    if (note_keys_n < POLYPHONY) { //add to array
      //Serial.print("note_key "); Serial.print(note_keys_n); Serial.print(" = "); Serial.println(key);
      note_keys[note_keys_n++] = key;
      //because we scan keys ordered, note_keys will be ordered too
    }
    return;
  }

  //this is string key
  //TODO can map all keys
  for (byte i = 0; i < POLYPHONY; i++) {
    if (key == string_keys_indices[i]) {
      //Serial.print("string_key "); Serial.println(i);
      string_keys[i] = 1;
    }
  }

  //TODO octave switch
}

//---------------------------------------------------------------
void keyboard_loop() {
  //clear notes and string keys
  note_keys_n = 0;
  for (byte i = 0; i < POLYPHONY; i++) {
    note_keys[i] = -1;
    string_keys[i] = 0;
  }

  bool was_changed = false;  //was keyboard state changed

  //Scan keys
  for (int k = keys; k >= 0; k--) {
    byte key = keys-1-k;
    byte v = (digitalRead(keyReadPin[k]) == LOW) ? 1 : 0;
    byte &state = key_state[key];
    //key was pressed or released
    if (state != v) {
      was_changed = true;
      Serial.print("key "); Serial.print(key); Serial.print(": ");
      if (v) Serial.println("on");
      else Serial.println("off");
    }
    state = v;

    //process pressing events
    if (v) {
      key_pressed(key);
    }
  }


  //play
  if (was_changed) {
    set_notes(string_keys[0] ? note_keys[0] : -1,
              string_keys[1] ? note_keys[1] : -1,
              string_keys[2] ? note_keys[2] : -1,
              string_keys[3] ? note_keys[3] : -1);
  }
  //test print of keyboard state - use for check all is connected
  //for (int i=0; i<keys; i++) {
  //  Serial.print(key_state[i]);
  //}
  //Serial.println();

}

//---------------------------------------------------------------
