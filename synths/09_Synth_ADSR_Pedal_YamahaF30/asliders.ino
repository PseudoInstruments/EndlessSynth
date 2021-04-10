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
#define READ_POTI(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) { Pot##_##NAME = map(SLIDERS_ENABLED_##ID?(analogRead(A_PIN)):512, 0, 1023, VMIN, VMAX); }
#define READ_POTF(ID, NAME, SHORT_NAME, A_PIN, VMIN, VMAX, PRINTID) { Pot##_##NAME = mapf(SLIDERS_ENABLED_##ID?(analogRead(A_PIN)):512, 0, 1023, VMIN, VMAX); }
#define READ_PIN(ID, NAME, SHORT_NAME, DIGITAL_PIN, PRINTID) { Pin##_##NAME = SLIDERS_ENABLED_##ID?(digitalRead(DIGITAL_PIN)?0:1):0; }

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
void pinModePower(byte pin, byte value) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, (value)?HIGH:LOW);  
}

//---------------------------------------------------------------
void sliders_setup() {
  if (!SLIDERS_ENABLED_1)  {
    prln("[Sliders 1 disabled]");
  }
  if (!SLIDERS_ENABLED_2)  {
    prln("[Sliders 2 disabled]");
  }
  if (!SLIDERS_ENABLED_3)  {
    prln("[Sliders 3 disabled]");
  }  
  pr("   Power pin sliders: 5V -> D"); pr(pin_Sliders_5V); pr(", Gnd -> D"); prln(pin_Sliders_Gnd);
  pr("   Power pin pedal: 5V -> D"); pr(pin_Pedal_5V); pr(", Gnd -> D"); prln(pin_Pedal_Gnd);
  //set up power pins
  pinModePower(pin_Sliders_5V, 1);
  pinModePower(pin_Sliders_Gnd, 0);
  pinModePower(pin_Pedal_5V, 1);
  pinModePower(pin_Pedal_Gnd, 0);

  //Set up pins for switches and keys
  INPUTS_APPLY(SETUP, 0)
}

//---------------------------------------------------------------
void sliders_loop() {
  //Read all controls
  INPUTS_APPLY(READ, 0)


  //PRINT
  if (sliders_debug > 0 && print_now) {
    //1,2,3 - print blocks
    if (sliders_debug == 1 || sliders_debug == 2 || sliders_debug == 3) {
      INPUTS_APPLY(PRINT, sliders_debug);
      prln();
    }
    if (sliders_debug == 4) { //print digital pins
      test_print_pins(1, 0);
    }
    if (sliders_debug == 5) { //print analog pins
      test_print_pins(0, 1);
    }
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
