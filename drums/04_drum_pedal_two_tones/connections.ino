//Connections and working with pots, switches, keys and pedal

//--------------------------------------------------------------
const byte audio_pin = 2;     //Audio output Sig
const byte audio_gnd_pin = 3; //Audio output Gnd

const byte pedal_pin = 11;  //Pedal pin Sig
const byte pedal_gnd_pin = 12;  //Pedal pin Gnd

const byte led_pin = 13;  //Built-in led pin

//--------------------------------------------------------------
const byte Pots = 9;    //number of pots
//Pots values
int Pot[Pots] = { -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000};
byte Pot_changed[Pots]; //is pot changed

int *Pot_Drum1_Duration = &Pot[0];
int *Pot_Drum1_Timbre = &Pot[1];
int *Pot_Drum1_Tone1 = &Pot[2];
int *Pot_Drum1_Tone2 = &Pot[3];

int *Pot_Sample_Rate = &Pot[4];

int *Pot_Drum2_Duration = &Pot[5];
int *Pot_Drum2_Timbre = &Pot[6];
int *Pot_Drum2_Tone1 = &Pot[7];
int *Pot_Drum2_Tone2 = &Pot[8];

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

  pinMode(audio_pin, OUTPUT); //activate audio output
  pin_power(audio_gnd_pin, 0);

  pinMode(pedal_pin, INPUT_PULLUP); //pedal
  pin_power(pedal_gnd_pin, 0);

  pinMode(led_pin, OUTPUT); //activate led



  //Read values, will be used for sound generator
  sliders_control_step();
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
inline void sliders_control_step() {
  //Reading pedal is inside sound loop
  //pedal_ = digitalRead(pedal_pin)?0:1;

  //Led on pedal
  if (pedal_) digitalWrite(led_pin, HIGH);
  else digitalWrite(led_pin, LOW);

  //Pots
  read_pots();



}

//--------------------------------------------------------------
inline void sliders_audio_step() {
  //pedal
  static byte last_pedal = 0;

  pedal_ = digitalRead(pedal_pin) ? 0 : 1;
  if (pedal_ && !last_pedal) {
    sound_on_pedal();     //play
  }
  last_pedal = pedal_;

}

//--------------------------------------------------------------
void sliders_debug_print() {
  Serial.print("pedal="); Serial.print(pedal_);
  Serial.print(" Pots: ");
  for (byte i = 0; i < Pots; i++) {
    Serial.print(Pot[i]);
    if (Pot_changed[i]) Serial.print("+");
    else Serial.print(" ");
    Serial.print(" ");
  }
  Serial.print(" Pot_Sample_Rate="); Serial.print(*Pot_Sample_Rate);
}

//--------------------------------------------------------------
