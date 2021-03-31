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
const byte pins_Keyboard_readN = 7;   //number of pins
const byte pins_Keyboard_blockN = 6;  //number of pins

byte pins_Keyboard_read[pins_Keyboard_readN] = {52, 50, 48, 46, 44, 42, 40};
byte pins_Keyboard_block[pins_Keyboard_blockN] = {32, 30, 28, 26, 24, 22};

//---------------------------------------------------------------
//Power for sliders
const byte pin_Sliders_5V = 6;   //+5V for sliders
const byte pin_Sliders_Gnd = 5;  //Gnd for sliders - it's PORTE 3

//---------------------------------------------
//Description of all pots, switches, keys: (I using linear pots 10 kOhm)
//It's used in "asliders" file, FUN - function to apply
#define INPUTS_APPLY(FUN) \
  /*  Block 1 - main settings: */\
  FUN##_POTF(Tone, A2, -3, 3); \
  FUN##_POTI(Pedal, A3, 0, 1023); \
  FUN##_POTI(Diffusion, A4, 8, 256); \  
  /*decaying diffusion 0..256, 0 - no diffusion, 256 - keep all diffusion*/ \
  const int Diffusion_shift = 7;  /*127 -> power*/ \
                                                \
  FUN##_PIN(Timbre1, 7); \
  FUN##_PIN(Timbre2, 8); \
  FUN##_PIN(Timbre3, 9); \
  FUN##_PIN(Timbre4, 10); \
  FUN##_PIN(Timbre5, 11); \
                                                \
  FUN##_PIN(Arp, 12); \
  FUN##_PIN(Latch_Left, 13); \
  FUN##_PIN(Latch_Right, 14); \
                                                \  
  /*  Block 2 - synth */    \
  FUN##_POTI(Attack, A5, 0, 1023); \
  FUN##_POTI(Decay, A6, 0, 1023); \
  FUN##_POTI(Sustain, A7, 0, 1023); \
  FUN##_POTI(Release, A8, 0, 1023); \
                                                \  
  FUN##_POTI(LFO_Range, A9, 0, 1023); \
  FUN##_POTI(LFO_Rate, A10, 0, 1023); \
                                                \
  FUN##_POTI(Digital_Volume, A11, 0, 127);   /*(sound effect 1)/* \
  /*const int Digital_Volume_shift = 7; // << 7 instead "/audio_volume_max"*/ \
                                                \
  FUN##_POTI(Filter, A12, 0, 1023);           /*(sound effect 2)*/    \
  FUN##_POTI(Sample_Rate, A13, 2000, 8000);      /*(sound effect 3)*/  \
                                                \
  FUN##_PIN(ADSR, 15); \
                                                \  
  FUN##_PIN(LFO_Shape1, 17); \
  FUN##_PIN(LFO_Shape2, 18); \
                                                  \
  FUN##_PIN(Enable_Pedal1, 19); \
  FUN##_PIN(Enable_Pedal2, 21); \
  FUN##_PIN(Enable_Pedal3, 23); \
                                                  \
  FUN##_PIN(Enable_LFO1, 25); \
  FUN##_PIN(Enable_LFO2, 27); \
  FUN##_PIN(Enable_LFO3, 29); \
                                                  \
  /*  Block 3 - presets*/      \
  FUN##_PIN(Presets, 31); \
  FUN##_PIN(Preset1, 33); \
  FUN##_PIN(Preset2, 35); \
  FUN##_PIN(Preset3, 37); \
  FUN##_PIN(Preset4, 39); \
  FUN##_PIN(Preset5, 41); \
  FUN##_PIN(Preset6, 43); \
  FUN##_PIN(Preset7, 45); \
  FUN##_PIN(Preset8, 47); 

//---------------------------------------------------------------
//

//---------------------------------------------------------------
//Arduino Mega PORTS mapping   https://forum.arduino.cc/index.php?topic=52534.0
/*
  we use PORTE for audio output, so disable all others, be careful

  0   PORTE 0    PORTE!
  1   PORTE 1     PORTE!
  2   PORTE 4     PORTE!  audio output Signal
  3   PORTE 5     PORTE!  audio output Gnd
  4   PORTG 5             sliders 5V
  5   PORTE 3     PORTE!  sliders Gnd
  6   PORTH 3
  7   PORTH 4

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
