//YAMAHA PSS F30 Keyboard Reader
//This is truly polyphonic keyboard with 37 keys.

//Connection:
//7 - control pins - choosing keys block,
//11 - signal pins, but it's uses only six - 1,3,5,7,9,11 - we connect only them

const byte keyGndN = 7;
const byte keyReadN = 6;

//Pins for connecting:
byte keyGnd[keyGndN] = {52, 50, 48, 46, 44, 42, 40};
byte keyRead[keyReadN] = {32, 30, 28, 26, 24, 22};


const int keys = 37;

//keys for playing "strings" by right hand
const byte string_keys_indices[POLYPHONY] = {keys-1, keys-2, keys-4}; //"string" keys, note reversed order

//keys for octave switch
const byte key_octave1 = keys - 7;
const byte key_octave2 = keys - 5;
const byte key_octave3 = keys - 3;

//keys for sample rate switch
const byte key_timbre1 = keys - 6;
const byte key_timbre2 = keys - 8;

//keys used for notes
const byte note_keys_count = keys - 8;

//octave base notes
const byte note_octave1 = 60 - 3 * 12;
const byte note_octave2 = 60 - 12;
const byte note_octave3 = 60; //C


//Current base note - changed with octave switch
byte base_note = note_octave2;

//---------------------------------------------------------------
void keyboard_setup() {
  pr("Keyboard Yamaha PSS F30 control pins: ");
  for (int i = 0; i < keyGndN; i++) {
    pr(keyGnd[i]); pr(" ");
  }
  pr(",  signal pins: ");
  for (int i = 0; i < keyReadN; i++) {
    pr(keyRead[i]); pr(" ");
  }
  prln();

  //Set pins
  for (int i = 0; i < keyGndN; i++) {
    pinMode(keyGnd[i], OUTPUT);
    digitalWrite(keyGnd[i], HIGH);
  }
  for (int i = 0; i < keyReadN; i++) {
    pinMode(keyRead[i], INPUT_PULLUP);
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
//octave switch
void set_base_note(char note) {
  if (base_note != note) {
    base_note = note;
    pr("Base note: ");
    prln(int(note));
  }
}

//---------------------------------------------------------------
//process pressing key
void key_pressed(byte key) {
  //this is note key
  if (key < note_keys_count) {
    if (note_keys_n < POLYPHONY) { //add to array
      //pr("note_key "); pr(note_keys_n); pr(" = "); prln(key);
      note_keys[note_keys_n++] = key;
      //because we scan keys ordered, note_keys will be ordered too
    }
    return;
  }

  //this is string key
  //TODO can map all keys
  for (byte i = 0; i < POLYPHONY; i++) {
    if (key == string_keys_indices[i]) {
      //pr("string_key "); prln(i);
      string_keys[i] = 1;
      return;
    }
  }

  //octave switch
  if (key == key_octave1) {
    set_base_note(note_octave1);
    return;
  }
  if (key == key_octave2) {
    set_base_note(note_octave2);
    return;
  }
  if (key == key_octave3) {
    set_base_note(note_octave3);
    return;
  }

  //timbre switch
  if (key == key_timbre1) {
    set_timbre(0);
    return;
  }
  if (key == key_timbre2) {
    set_timbre(1);
    return;
  }

  //sample rate switch
  //if (key == key_sample_rate1) { set_audio_sample_rate_index(0); return; }
  //if (key == key_sample_rate2) { set_audio_sample_rate_index(1); return; }
}

//---------------------------------------------------------------
//unsigned long int keyb_time_print_ = 0; //time for printing keyboard state

void keyboard_loop() {
  //clear notes and string keys
  note_keys_n = 0;
  for (byte i = 0; i < POLYPHONY; i++) {
    note_keys[i] = -1;
    string_keys[i] = 0;
  }

  bool was_changed = false;  //was keyboard state changed

  //Scan keys
  for (byte k = 0; k < keyGndN; k++) {
    //enable control pin
    digitalWrite(keyGnd[k], LOW);
    //delayMicroseconds(10);  //need wait 2 mcs, but let's wait for more...

    //Scan pins
    //TODO can use ports for reading faster, see 05_ArduinoMegaPortTest

    for (byte i = 0; i < keyReadN; i++) {
      byte key = keys - 1 - (i + keyReadN * k); //because of reverted
      byte v = (digitalRead(keyRead[i]) == LOW) ? 1 : 0;

      byte &state = key_state[key];
      //key was pressed or released
      if (state != v) {
        was_changed = true;
        if (debug) {
          pr("key "); pr(key); pr(": ");
          if (v) prln("on");
          else prln("off");
        }
      }
      state = v;

      //process pressing events
      if (v) {
        key_pressed(key);
      }

    }
    //disable control pin
    digitalWrite(keyGnd[k], HIGH);
  }

  //play
  if (was_changed) {
    //set keys with using base_note, which is depends on choosen octave
    set_notes(string_keys[0] ? note_keys[0] : -1,
              string_keys[1] ? note_keys[1] : -1,
              string_keys[2] ? note_keys[2] : -1,
              string_keys[3] ? note_keys[3] : -1,
              base_note);
  }

  //demo play
  if (demo_play) {
    static int t = 0;
    t = (t+1)%(FPS/2);
    if (t == 0) {
      //random notes     
      set_notes(random(24), -1, -1, -1, base_note);
    }
  }
  
  //test print of keyboard state - use for check all is connected
  //for (int i=0; i<keys; i++) {
  //  pr(key_state[i]);
  //}
  //prln();

}

//---------------------------------------------------------------
