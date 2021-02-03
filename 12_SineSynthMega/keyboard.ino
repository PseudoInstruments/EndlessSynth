//Toy Piano Keyboard Reader

//Connection:
//- 12 pins from Toy Piano to Arduino Mega: 22,24,26,...,44
//pins 0,1,2,3 - choosing keys block,
//pins 4,5,6,7,8,9,10,11 - keys 0..7, 8..15, 16..23, 24..31
//(keys numbers is reverted)

const byte keyGndN = 4;
const byte keyReadN = 8;
byte keyGndPin[keyGndN] = {22, 24, 26, 28};
byte keyReadPin[keyReadN] = {30, 32, 34, 36, 38, 40, 42, 44};

const byte keys = 32;  //number of keys
const byte note_keys_count = keys - 8; //notes used for notes
const byte string_keys_indices[POLYPHONY] = {26,28,30,31}; //"string" keys

//Starting midi note
byte midi_note_0 = 65; //F

//---------------------------------------------------------------
void keyboard_setup() {
  for (int i = 0; i < keyGndN; i++) {
    Serial.print(keyGndPin[i]); Serial.print(" ");
  }
  for (int i = 0; i < keyReadN; i++) {
    Serial.print(keyReadPin[i]); Serial.print(" ");
  }
  Serial.println();
  Serial.println("Now press the keys!");

  //Set
  for (int i = 0; i < keyGndN; i++) {
    pinMode(keyGndPin[i], OUTPUT);
    digitalWrite(keyGndPin[i], HIGH);  //выключено по-умолчанию
  }
  for (int i = 0; i < keyReadN; i++) {
    pinMode(keyReadPin[i], INPUT_PULLUP);
  }
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
    for (byte i=0; i<POLYPHONY; i++) {
      if (key == string_keys_indices[i]) {
      //Serial.print("string_key "); Serial.println(i);
      string_keys[i] = 1;
    }
  }

  //TODO octave switch
}

//---------------------------------------------------------------
void keyboard_loop() {
  note_keys_n = 0;  //clear pressing notes
  for (byte i = 0; i < POLYPHONY; i++) { //clear string notes
    string_keys[i] = 0;
  }

  bool was_changed = false;  //was keyboard state changed

  //Scan keys
  for (char k = keyGndN-1; k>=0; k--) {   //because "for" decreases, we need to use char for check ">=0" properly
    for (char j = 0; j < keyGndN; j++) { //choose keyboard block
      digitalWrite(keyGndPin[j], (j == k) ? LOW : HIGH);
    }
    delayMicroseconds(1000); //pause to pin affected circuit

    //read keys pins //TODO can use ports for reading faster, see 05_ArduinoMegaPortTest
    for (char i = keyReadN-1; i >= 0; i--) {  //because "for" decreases, we need to use char for check ">=0" properly
      byte key = keys - 1 - (i + keyReadN * k); //because of reverted
      byte v = (digitalRead(keyReadPin[i]) == LOW) ? 1 : 0;
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
  }
    
  //play
  if (was_changed) {
    set_notes(string_keys[0] ? note_keys[0] : -1,
              string_keys[1] ? note_keys[1] : -1,
              string_keys[2] ? note_keys[2] : -1,
              string_keys[3] ? note_keys[3] : -1);
  }
  //test print
  //for (int i=0; i<keys; i++) {
  //  Serial.print(key_state[i]);
  //}
  //Serial.println();

}

//---------------------------------------------------------------
