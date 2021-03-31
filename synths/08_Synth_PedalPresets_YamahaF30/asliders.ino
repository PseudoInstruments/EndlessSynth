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

#define DEF_POTI(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) \
  const byte pot##_##NAME = A_PIN; \
  const int NAME##_Min = VMIN; \
  const int NAME##_Max = VMAX; \
  int Pot##_##NAME;

//Macros for defining float pot:
#define DEF_POTF(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) \
  const byte pot##_##NAME = A_PIN; \
  const int NAME##_Min = VMIN; \
  const int NAME##_Max = VMAX; \
  float Pot##_##NAME;

//Macros for defining input pin:
#define DEF_PIN(ID, NAME, SHORT_NAME, DIGITAL_PIN, PRINTID) \
  const byte pin##_##NAME = DIGITAL_PIN; \
  byte Pin##_##NAME;


//Setup
#define SETUP_POTI(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) {  pr("   "); pr(ID); pr(" pot "); pr(#NAME); pr(" -> (A)"); prln(A_PIN); }
#define SETUP_POTF(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) { pr("   "); pr(ID); pr(" pot "); pr(#NAME); pr(" -> (A)"); prln(A_PIN); }
#define SETUP_PIN(ID, NAME, SHORT_NAME, DIGITAL_PIN, PRINTID) { pinMode(DIGITAL_PIN, INPUT_PULLUP); pr("   "); pr(ID); pr("     "); pr(#NAME); pr(" -> D"); prln(DIGITAL_PIN); }

//Read
#define READ_POTI(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) { Pot##_##NAME = map(SLIDERS_ENABLED?(analogRead(A_PIN)):512, 0, 1023, VMIN, VMAX); }
#define READ_POTF(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) { Pot##_##NAME = mapf(SLIDERS_ENABLED?(analogRead(A_PIN)):512, 0, 1023, VMIN, VMAX); }
#define READ_PIN(ID, NAME, SHORT_NAME, DIGITAL_PIN, PRINTID) { Pin##_##NAME = SLIDERS_ENABLED?(digitalRead(DIGITAL_PIN)?0:1):0; }

//Print
#define PRINT_POTI(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) { if (ID == PRINTID) {pr(" "); pr(#SHORT_NAME); pr("="); pr(Pot##_##NAME); }}
#define PRINT_POTF(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) { if (ID == PRINTID) {pr(" "); pr(#SHORT_NAME); pr("="); pr(Pot##_##NAME); }}
#define PRINT_PIN(ID, NAME, SHORT_NAME, DIGITAL_PIN, PRINTID) { if (ID == PRINTID) {pr(" "); pr(#SHORT_NAME); pr("="); pr(Pin##_##NAME); }}

//---------------------------------------------------------------
//Define all controls
INPUTS_APPLY(DEF, 0)

//---------------------------------------------------------------
//function for read pin - used in pins_iterate
void read_pin_in(byte pin, byte &value) {
  value = digitalRead(pin) ? 0 : 1;
}

//---------------------------------------------------------------
void sliders_setup() {
  if (!SLIDERS_ENABLED)  {
    prln("[Sliders disabled]");
    return;
  }
  
  prln("[Sliders enabled]");
  pr("   Power pin sliders: 5V -> D"); pr(pin_Sliders_5V); pr(", Gnd -> D"); prln(pin_Sliders_Gnd);
  //set up power pins
  pinMode(pin_Sliders_5V, OUTPUT);
  pinMode(pin_Sliders_Gnd, OUTPUT);
  digitalWrite(pin_Sliders_5V, HIGH);
  digitalWrite(pin_Sliders_Gnd, LOW);

  //Set up pins for switches and keys
  INPUTS_APPLY(SETUP, 0)
}

//---------------------------------------------------------------
void sliders_loop() {
  //Read all controls
  INPUTS_APPLY(READ, 0)


  //PRINT
  if (sliders_debug > 0 && debug_now) {
    INPUTS_APPLY(PRINT, sliders_debug);
    prln();
  }
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
