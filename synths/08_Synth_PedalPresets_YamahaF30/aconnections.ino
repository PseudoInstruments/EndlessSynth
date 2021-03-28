//Connections
//Here described all used pins

//Note: If you have no sliders, set SLIDERS_ENABLED=0 in the main sketch file.
//Note: PORTS mapping list in Arduino Mega see the end of this file.

//---------------------------------------------------------------
//Audio output
const byte pin_audio_output = 2;

//---------------------------------------------------------------
//Keyboard
byte pins_Keyboard_read[keyGndN] = {52, 50, 48, 46, 44, 42, 40};
byte pins_Keyboard_block[keyReadN] = {32, 30, 28, 26, 24, 22};
//---------------------------------------------------------------

//Control blocks
const byte pin_Sliders_5V = 6;   //+5V for sliders  
const byte pin_Sliders_Gnd = 5;  //Gnd for sliders - it's PORTE 3

//---------------------------------------------
//Pots, switches, keys: (I using linear pots 10 kOhm)
//---------------------------------------------
//  Block 1 - main settings:
const byte pot_Tone = A2;
const byte pot_Pedal = A3;
const byte pin_Timbre1 = 7;
const byte pin_Timbre2 = 8;
const byte pin_Timbre3 = 9;
const byte pin_Timbre4 = 10;
const byte pin_Timbre5 = 11;

const byte pin_Arp = 12;
const byte pin_Latch_Left = 13;
const byte pin_Latch Right = 14;
const byte pot_Diffusion = A5;

//  Block 2 - synth
const byte pin_ADSR = 15;
const byte pot_Decay = A6;
const byte pin_Sustain = 16;
const byte pot_Attack = A7;
const byte pot_Release = A8;
const byte pot_LFO_Range = A9;
const byte pot_LFO_Rate = A10;
const byte pin_LFO_Shape1 = 17;
const byte pin_LFO_Shape2 = 18;
const byte pot_Digital_Volume = A11;  //(sound effect 1)
const byte pot_Filter = A12;          //(sound effect 2)
const byte pot_Sample_Rate = A13;     //(sound effect 3)
const byte pin_Enable_Pedal1 = 19;
const byte pin_Enable_Pedal2 = 21;
const byte pin_Enable_Pedal3 = 23;
const byte pin_Enable_LFO1 = 25;
const byte pin_Enable_LFO2 = 27;
const byte pin_Enable_LFO3 = 29;

//Block 3 - presets
const byte pin_Presets = 31;
const byte pin_Preset1 = 33;
const byte pin_Preset2 = 35;
const byte pin_Preset3 = 37;
const byte pin_Preset4 = 39;
const byte pin_Preset5 = 41;
const byte pin_Preset6 = 43;
const byte pin_Preset7 = 45;
const byte pin_Preset8 = 47;

//---------------------------------------------------------------
//

//---------------------------------------------------------------
//Arduino Mega PORTS mapping   https://forum.arduino.cc/index.php?topic=52534.0
/*
 we use PORTE for audio output, so disable all others, be careful
  
0   PORTE 0    PORTE!
1   PORTE 1     PORTE!  
2   PORTE 4     PORTE!  audio output   
3   PORTE 5     PORTE!
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
