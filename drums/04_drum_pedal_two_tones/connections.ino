//Connections and working with pots, switches, keys and pedal

//--------------------------------------------------------------
const byte audio_pin = 2;     //Audio output Sig
const byte audio_gnd_pin = 3; //Audio output Gnd

const byte led_pin = 13;  //Built-in led pin

//--------------------------------------------------------------
const byte pedal_pin = 11;  //Pedal pin Sig
const byte pedal_gnd_pin = 12;  //Pedal pin Gnd

const byte key1_pin = 20;       //keys for play a first or second drum
const byte key2_pin = 18;
const byte switch1_pin = 21;    //switches for routing pedal to drums
const byte switch2_pin = 19;

const byte N = 2;     //number of drums
byte Key1;
byte Key2;
byte Switch[N];


//--------------------------------------------------------------
const byte Pots = 9;    //number of pots
//Pots values
int Pot[Pots] = { -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000};
byte Pot_changed[Pots]; //is pot changed

int *Pot_Drum1_Duration = &Pot[0];    byte *Pot_Drum1_Duration_Changed = &Pot_changed[0];
int *Pot_Drum1_Timbre = &Pot[1];      byte *Pot_Drum1_Timbre_Changed = &Pot_changed[1];
int *Pot_Drum1_Tone1 = &Pot[2];       byte *Pot_Drum1_Tone1_Changed = &Pot_changed[2];
int *Pot_Drum1_Tone2 = &Pot[3];       byte *Pot_Drum1_Tone2_Changed = &Pot_changed[3];

int *Pot_Sample_Rate = &Pot[4];       byte *Pot_Sample_Rate_Changed = &Pot_changed[4];

int *Pot_Drum2_Duration = &Pot[5];    byte *Pot_Drum2_Duration_Changed = &Pot_changed[5];
int *Pot_Drum2_Timbre = &Pot[6];      byte *Pot_Drum2_Timbre_Changed = &Pot_changed[6];
int *Pot_Drum2_Tone1 = &Pot[7];       byte *Pot_Drum2_Tone1_Changed = &Pot_changed[7];
int *Pot_Drum2_Tone2 = &Pot[8];       byte *Pot_Drum2_Tone2_Changed = &Pot_changed[8];

const byte pot_apin[Pots] = {A8, A9, A10, A11, A7, A6, A5, A4, A3};

const int pot_min = 0;     //for adjusting pots boundaries
const int pot_max = 1023;
const int pot_change_thresh = 10;  //value for set that pot is changed


//--------------------------------------------------------------
//Pedal
byte pedal_ = 0;


//--------------------------------------------------------------
//set 5V or Gnd to pin
void pin_power(int pin, int value) {
  pinMode(pin, OUTPUT);
  if (value) digitalWrite(pin, HIGH);
  else digitalWrite(pin, LOW);
}

//--------------------------------------------------------------
void sliders_setup() {
  Serial.println("Audio output: D2 (Sig), D3 (Gnd), pedal: D11, D12");
  Serial.println("TODO Slider 'Volume': D2, audio output, Gnd, Slider 'Sample' Rate: 5V, A4, Gnd, Slider 'Duration': 5V, A5, Gnd");

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  //Audio output
  pinMode(audio_pin, OUTPUT);
  pin_power(audio_gnd_pin, 0);

  //Led
  pinMode(led_pin, OUTPUT);

  //Pedal
  pinMode(pedal_pin, INPUT_PULLUP);
  pin_power(pedal_gnd_pin, 0);

  //Keys and switches
  pinMode(key1_pin, INPUT_PULLUP);
  pinMode(key2_pin, INPUT_PULLUP);
  pinMode(switch1_pin, INPUT_PULLUP);
  pinMode(switch2_pin, INPUT_PULLUP);

  //Read values, will be used for sound generator
  sliders_slow_control_step();
}


//--------------------------------------------------------------
void read_pots() {
  int value;
  for (byte i = 0; i < Pots; i++) {
    value = analogRead(pot_apin[i]);
    //Serial.print(i); Serial.print(" "); Serial.println(value);
    Pot_changed[i] = (abs(value - Pot[i]) > pot_change_thresh);
    Pot[i] = value;
  }
}

//--------------------------------------------------------------
//Read pots
inline void sliders_slow_control_step() {
  //Pots
  read_pots();

  //Switches
  Switch[0] = digitalRead(switch1_pin) ? 0 : 1;
  Switch[1] = digitalRead(switch2_pin) ? 0 : 1;

  //Reading pedal and keys is in fast_control_step()

  //Flash led on pedal
  if (pedal_) digitalWrite(led_pin, HIGH);
  else digitalWrite(led_pin, LOW);

}

//--------------------------------------------------------------
//Read pedal and keys
//Called from "xsound"
inline void sliders_fast_control_step() {
  //Pedal
  static byte last_pedal = 0;

  pedal_ = digitalRead(pedal_pin) ? 0 : 1;
  if (pedal_ != last_pedal) {
    if (pedal_) {
      on_pedal();
    }
    last_pedal = pedal_;
  }

  //Keys
  static byte last_key1 = 0;
  static byte last_key2 = 0;
  Key1 = digitalRead(key1_pin) ? 0 : 1;
  Key2 = digitalRead(key2_pin) ? 0 : 1;

  if (Key1 != last_key1) {
    if (Key1) {
      sound_play(0);
    }
    last_key1 = Key1;
  }
  if (Key2 != last_key2) {
    if (Key2) {
      sound_play(1);
    }
    last_key2 = Key2;
  }
}

//--------------------------------------------------------------
//Pedal event
inline void on_pedal() {
  //decide which sound to start, depending on routing

  static byte id = 0;

  if (Switch[0] && Switch[1]) {
    sound_play(id);
    id = 1 - id;    //Toggle drum
  }
  else {
    if (Switch[0]) {
      sound_play(0);
    }
    if (Switch[1]) {
      sound_play(1);
    }
  }
}


//--------------------------------------------------------------
void sliders_debug_print() {
  Serial.print("pedal="); Serial.print(pedal_);
  Serial.print(" Keys: "); Serial.print(Key1); Serial.print(" "); Serial.print(Key2); ;
  Serial.print(" Switches: "); Serial.print(Switch[0]); Serial.print(" "); Serial.print(Switch[1]);
  Serial.print(" Pots: ");
  for (byte i = 0; i < Pots; i++) {
    Serial.print(Pot[i]);
    if (Pot_changed[i]) Serial.print("+");
    else Serial.print(" ");
    Serial.print(" ");
  }
  //Serial.print(" Pot_Sample_Rate="); Serial.print(*Pot_Sample_Rate);
}

//--------------------------------------------------------------
