# EndlessSynth synthesizers codes

This is the main codes of EndlessSynthe project, dedicated to creating 1-bit synthesizer 
using Arduino and toy keyboard.

## Synths

### 01_SineSynthArpegiatorMega_Harsh

Synthesizer for Arduino Mega using toy keyboard. Output goes through buzzer or "Troika" audio amplifier to minijack.

Features:
* sine wave synth, without attack and release
* harsh 1-bit dithering settings
* arpeggiator ("note" and "string" keys)
* harsh sound
* switch octaves



### 02_SineSynthArpegiatorMega_Smoother

Synthesizer for Arduino Mega using toy keyboard with smoother sound
generated from sine wave. Output goes through buzzer or "Troika" audio amplifier to minijack.

Features:
* sine wave synth, without attack and release
* smoother 1-bit dithering settings
* arpeggiator ("note" and "string" keys)
* harsh sound
* switch octaves


### 03_SineSynthArpegiatorMega_Sliders

Synthesizer for Arduino Mega and toy keyboard 
with two sliders controlling 1-bit dithering algorithm. Output goes through buzzer or "Troika" audio amplifier to minijack.

* sine wave synth, without attack and release
* two sliders connected to A4 and A5 controls 1-bit algorithm.
  - slider 1: "step" - kind of "level of sound generation", the higher - the more dithering
  - slider 2: "duration" - duration of dithering, the lower - the more harshness
* arpeggiator ("note" and "string" keys)
* harsh sound
* switch octaves


### 04_SineSynthArpegiatorMega_Sliders_Boombox

Synthesizer for Arduino Mega and toy keyboard 
with two sliders controlling 1-bit dithering algorithm. Output goes through buzzer or "Troika" audio amplifier to minijack.
Also to A0 connected toy microphone coupled with potentiometer to output to A0 512 in silence.
Also connected button from microphone. When button is pressed is sends signal instead synthesizing.
So synthesizer works as boombox 1-bit engine too.

The second slider is kind of mic sensitivity.
Hint: setting both 1-bit sliders to zero and just presing button generates glitch sound!
But to normal mic sound - increase both 1-bit sliders.

* sine wave synth, without attack and release
* two sliders connected to A4 and A5 controls 1-bit algorithm.
  - slider 1: "step" - kind of "level of sound generation", the higher - the more dithering
  - slider 2: "duration" - duration of dithering, the lower - the more harshness
* arpeggiator ("note" and "string" keys)
* harsh sound
* switch octaves
* switch sample rate
* microphone with button for boombox mode

### Roadmap

* Mic: constrol mic volume (can digitally using potentioneter).
* Sampler: sample sound and play by keys
* Keyboard advance: use pairs of octave for more octaves or for switch samples/synth
* Sound Engine: control sample rate - enable during mic enabled
* Synthesizing module: triangle, sawtooth, square waves, pulse synthesis, envelope: attack, decay, release.
* Sound output: remove offset to -5V because it's hard to use mixer.
