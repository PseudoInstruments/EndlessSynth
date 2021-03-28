//Sliders
//See description of controls in the main file
//See connections in "aconnections"
//Note: If you have no sliders, set SLIDERS_ENABLED=0 in the main sketch file.

//---------------------------------------------
//Macroses
//---------------------------------------------

//Macros for defining integer pot:
//const byte pot_Pedal = A3;
//const int Pedal_Min = 0;
//const int Pedal_Max = 1023;
//int Pot_Pedal;

#define DEF_POTI(NAME, A_PIN, VMIN, VMAX) \
  const byte pot##_##NAME = A_PIN; \
  const int NAME##_Min = VMIN; \
  const int NAME##_Max = VMAX; \
  int Pot##_##NAME;

//Macros for defining float pot:
#define DEF_POTF(NAME, A_PIN, VMIN, VMAX) \
  const byte pot##_##NAME = A_PIN; \
  const int NAME##_Min = VMIN; \
  const int NAME##_Max = VMAX; \
  float Pot##_##NAME;

//Macros for defining input pin:
#define DEF_PIN(NAME, DIGITAL_PIN) \
  const byte pin##_##NAME = DIGITAL_PIN; \
  byte Pin##_##NAME;


//Setup
#define SETUP_POTI(NAME, A_PIN, VMIN, VMAX) {}
#define SETUP_POTF(NAME, A_PIN, VMIN, VMAX) {}
#define SETUP_PIN(NAME, DIGITAL_PIN) { pinMode(DIGITAL_PIN, INPUT_PULLUP); }

//Read
#define READ_POTI(NAME, A_PIN, VMIN, VMAX) {}
#define READ_POTF(NAME, A_PIN, VMIN, VMAX) {}
#define READ_PIN(NAME, DIGITAL_PIN) {}

//---------------------------------------------------------------
//Define all controls
INPUTS_APPLY(DEF)

//---------------------------------------------------------------
//function for read pin - used in pins_iterate
void read_pin_in(byte pin, byte &value) {
  value = digitalRead(pin)?0:1;
}

//---------------------------------------------------------------
void sliders_setup() {
  if (SLIDERS_ENABLED)  {
    prln("[Sliders enabled]");
  }
  else prln("[Sliders disabled]");

  if (SLIDERS_ENABLED) {
    //set up power pins
    pinMode(pin_Sliders_5V, OUTPUT);
    pinMode(pin_Sliders_Gnd, OUTPUT);
    digitalWrite(pin_Sliders_5V, HIGH);
    digitalWrite(pin_Sliders_Gnd, LOW);

    //Set up pins for switches and keys
    INPUTS_APPLY(SETUP)
  }
}

//---------------------------------------------------------------
//read analog pin and set slider
inline void read_poti(byte a_pin, int &value, int out0, int out1, int default_value = 512) {
  value = SLIDERS_ENABLED?(analogRead(a_pin)):default_value;
  value = map(value, 0, 1023, out0, out1);
}

inline void read_potf(byte a_pin, float &value, int out0, int out1, int default_value = 512) {
  value = mapf(SLIDERS_ENABLED?(analogRead(a_pin)):default_value, 0, 1023, out0, out1);
}

//---------------------------------------------------------------
void sliders_loop() {
  //Read all controls
  INPUTS_APPLY(READ)
  
  /*
    //Debug print
    if (sliders_debug) {
      static byte t = 0;
      t = (t + 1) % 10;
      if (t == 0) {
        pr("SLIDERS Tone="); pr(Tone);
        pr(", Attack="); pr(Attack);
        pr(", Decay="); pr(Decay);
        pr(", Sustain="); pr(Sustain);
        pr(", Release="); pr(Release);

        pr(", Sample_rate="); pr(Sample_rate);
        pr(", Diffusion="); pr(Diffusion);
        pr(", Timbre="); pr(Timbre);

        prln("");
      }
    }*/
}


//---------------------------------------------------------------
