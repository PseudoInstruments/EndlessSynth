# EndlessSynth synthesizers codes

This is the main codes of EndlessSynthe project, dedicated to creating 1-bit synthesizer 
using Arduino and toy keyboard.

## Contents

* **01_SineSynthArpegiatorMega_Harsh** - synthesizer for Arduino Mega using toy keyboard with harsh sound
generated from sine wave. Output goes through buzzer or "Troika" audio amplifier to minijack.


* **02_SineSynthArpegiatorMega_Smoother** - synthesizer for Arduino Mega using toy keyboard with smoother sound
generated from sine wave. Output goes through buzzer or "Troika" audio amplifier to minijack.

* **03_SineSynthArpegiatorMega_Sliders** - synthesizer for Arduino Mega and toy keyboard 
with two sliders controlling 1-bit dithering algorithm. Output goes through buzzer or "Troika" audio amplifier to minijack.

* **04_SineSynthArpegiatorMega_Sliders_Boombox** - - synthesizer for Arduino Mega and toy keyboard 
with two sliders controlling 1-bit dithering algorithm. Output goes through buzzer or "Troika" audio amplifier to minijack.
Also to A0 connected toy microphone coupled with potentiometer to output to A0 512 in silence.
Also connected button from microphone. When button is pressed is sends signal instead synthesizing.
So synthesizer works as boombox 1-bit engine too.
