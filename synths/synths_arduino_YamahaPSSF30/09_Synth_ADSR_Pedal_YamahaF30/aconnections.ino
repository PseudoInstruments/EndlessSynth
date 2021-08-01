//Connections
//Here described all used pins

//Note: If you have no sliders, set SLIDERS_ENABLED=0 in the main sketch file.
//Note: PORTS mapping list in Arduino Mega see the end of this file.


//---------------------------------------------------------------
//Audio output
const byte pin_audio_output_signal = 2;  //It's set to pins indirectly by PORTE in "xsound"
const byte pin_audio_gnd = 3;

//---------------------------------------------------------------
//Keyboard
const byte pins_Keyboard_blockN = 7;   //number of keyboard block blocks
const byte pins_Keyboard_signalN = 6;  //number of keyboard read pins

byte pins_Keyboard_block[pins_Keyboard_blockN] = {35, 33, 31, 29, 27, 25, 23};
byte pins_Keyboard_signal[pins_Keyboard_signalN] = {32, 30, 28, 26, 24, 22};

//---------------------------------------------------------------
//Power for sliders
const byte pin_Sliders_5V = 11;   //+5V for sliders
const byte pin_Sliders_Gnd = 12;  //Gnd for sliders

const byte pin_Pedal_5V = 7;   //+5V for pedal
const byte pin_Pedal_Gnd = 6;  //Gnd for pedal


//---------------------------------------------
const int Diffusion_shift = 7;  /*127 -> power*/ \

//---------------------------------------------
//Description of all pots, switches, keys: (I using linear pots 10 kOhm)
//It's used in "asliders" file, FUN - function to apply

//POTS:
//const byte pot_Pedal = A3;
//const int Pedal_Min = 0;
//const int Pedal_Max = 127;
//int Pot_Pedal;
//KEYS:
//byte Pin_Synth_Mode = 0;
//byte Pin_Synth_Mode_Changed = 0;


#define INPUTS_APPLY(FUN, PRINTID) \
  /*  Block 1 - main settings: */\
  \
  FUN##_POTF(1, Tone, Ton, A2, -3, 3, PRINTID); \
  FUN##_POTI(1, Pedal_Sens, Ped_Sens, A3, -127, 127, PRINTID); \
  FUN##_POTI(1, Diffusion, Dif, A4, 8, 256, PRINTID); \
  /*decaying diffusion 0..256, 0 - no diffusion, 256 - keep all diffusion*/ \
  \
  FUN##_PIN(1, Timbre1, Timb1, 21, PRINTID); \
  FUN##_PIN(1, Timbre2, 2, 20, PRINTID); \
  FUN##_PIN(1, Timbre3, 3, 19, PRINTID); \
  FUN##_PIN(1, Timbre4, 4, 18, PRINTID); \
  FUN##_PIN(1, Timbre5, 5, 17, PRINTID); \
  FUN##_PIN(1, Timbre6, 6, 16, PRINTID); \
  \
  FUN##_PIN(1, Arpegiator_Mode, Arp_Mode, 8, PRINTID); \
  FUN##_PIN(1, Synth_Mode, Synth_Mod, 9, PRINTID); \
  FUN##_PIN(1, Latch, Latch, 10, PRINTID); \
  \
  /*  Block 2 - synth */    \
  FUN##_POTI(2, Pedal_Inp, PED, A1, 0, 1023, PRINTID); /*Note: my pedal outputs 0..19 because has range 1kOhm..20 kOhm, see Pedal_Input_Max*/ \    
  \
  FUN##_POTI(2, Attack, Att, A12, 0, 1023, PRINTID); \
  FUN##_POTI(2, Decay, Dec, A10, 0, 1023, PRINTID); \
  FUN##_POTI(2, Sustain, Sust, A11, 0, 1023, PRINTID); \
  FUN##_POTI(2, Release, Rels, A13, 0, 1023, PRINTID); \
  FUN##_POTI(2, LFO_Range, LFORange, A14, 0, 127, PRINTID); \
  FUN##_POTI(2, LFO_Rate, Rate, A8, 1, 10, PRINTID); /*Notes -> frequencies*/\
  \
  FUN##_POTI(2, Digital_Volume, DigiVol, A9, 0, 127, PRINTID);   /*(sound effect 1)/* \
  /*const int Digital_Volume_shift = 7; // << 7 instead "/audio_volume_max"*/ \
  \
  FUN##_POTI(2, Filter, Flt, A7, 0, 1023, PRINTID);           /*(sound effect 2)*/    \
  FUN##_POTI(2, Sample_Rate, SRate, A6, 0, 8000, PRINTID);      /*Sample rate*/  \
  \
  FUN##_PIN(2, ADSR, ADSR, 36, PRINTID); \
  \
  FUN##_PIN(2, LFO_Shape1, Shp1, 44, PRINTID); \
  FUN##_PIN(2, LFO_Shape2, Shp2, 52, PRINTID); \
  \
  FUN##_PIN(2, Enable_Pedal1, Ped1, 38, PRINTID); \
  FUN##_PIN(2, Enable_Pedal2, Ped2, 40, PRINTID); \
  FUN##_PIN(2, Enable_Pedal3, Ped3, 42, PRINTID); \
  \
  FUN##_PIN(2, Enable_LFO1, LFO1, 46, PRINTID); \
  FUN##_PIN(2, Enable_LFO2, 2, 48, PRINTID); \
  FUN##_PIN(2, Enable_LFO3, 3, 50, PRINTID);


/*  Block 3 - presets  (NOTE: CURRENTLY NOT USED)    \
  FUN##_PIN(3, Presets, Presets, 37, PRINTID); \
  FUN##_PIN(3, Preset1, P1, 39, PRINTID); \
  FUN##_PIN(3, Preset2, P2, 41, PRINTID); \
  FUN##_PIN(3, Preset3, P3, 43, PRINTID); \
  FUN##_PIN(3, Preset4, P4, 45, PRINTID); \
  FUN##_PIN(3, Preset5, P5, 47, PRINTID); \
  FUN##_PIN(3, Preset6, P6, 49, PRINTID); \
  FUN##_PIN(3, Preset7, P7, 51, PRINTID); \
  FUN##_PIN(3, Preset8, P8, 53, PRINTID);
*/
//---------------------------------------------------------------
//Print all analog and digital pins
void test_print_pins(byte digital, byte analog) {
  if (digital) {
    pr("Pins Digital:");
    for (int i = 0; i < 53; i++) {
      if (i % 10 == 0) {
        pr("[");
        pr(i);
        pr("] ");
      }
      pr(digitalRead(i)?".":"X"); pr(" ");
    }
    prln();
  }
  if (analog) {
    pr("Analog:  ");
    for (int i = 0; i < 16; i++) {
      if (i % 8 == 0 && i > 0) {
        pr("   ");
      }
      int k = A0;
      if (i == 0) k = A0;
      if (i == 1) k = A1;
      if (i == 2) k = A2;
      if (i == 3) k = A3;
      if (i == 4) k = A4;
      if (i == 5) k = A5;
      if (i == 6) k = A6;
      if (i == 7) k = A7;
      if (i == 8) k = A8;
      if (i == 9) k = A9;
      if (i == 10) k = A10;
      if (i == 11) k = A11;
      if (i == 12) k = A12;
      if (i == 13) k = A13;
      if (i == 14) k = A14;
      if (i == 15) k = A15;
      pr(analogRead(k));
      pr(" ");
    }
    prln();
  }
}


//---------------------------------------------------------------
//Arduino Mega PORTS mapping   https://forum.arduino.cc/index.php?topic=52534.0
/*
  we use PORTE for audio output, so disable all others, be careful

  0   PORTE 0    PORTE!
  1   PORTE 1     PORTE!
  2   PORTE 4     PORTE!  audio output Signal
  3   PORTE 5     PORTE!  audio output Gnd
  4   PORTG 5             
  5   PORTE 3     PORTE!  
  6   PORTH 3             pedal Gnd
  7   PORTH 4             pedal 5V

  8   PORTH 5
  9   PORTH 6
  10   PORTB 4
  11   PORTB 5
  12   PORTB 6
  13   PORTB 7

  14   PORTJ 1
  15   PORTJ 0
  16   PORTH 1
  17   PORTH 0
  18   PORTD 3
  19   PORTD 2
  20   PORTD 1
  21   PORTD 0

  22   PORTA 0
  23   PORTA 1
  24   PORTA 2
  25   PORTA 3
  26   PORTA 4
  27   PORTA 5
  28   PORTA 6
  29   PORTA 7

  30   PORTB 7
  31   PORTB 6
  32   PORTB 5
  33   PORTB 4
  34   PORTB 3
  35   PORTB 2
  36   PORTB 1
  37   PORTB 0

  38   PORTD 7
  39   PORTG 2
  40   PORTG 1
  41   PORTG 0
  42   PORTL 7
  43   PORTL 6
  44   PORTL 5
  45   PORTL 4

  46   PORTL 3
  47   PORTL 2
  48   PORTL 1
  49   PORTL 0
  50   PORTB 3
  51   PORTB 2
  52   PORTB 1
  53   PORTB 0
*/

//---------------------------------------------------------------
