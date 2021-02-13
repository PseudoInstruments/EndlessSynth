# EndlessSynth synthesizers codes

This is the main codes of EndlessSynthe project, dedicated to creating 1-bit synthesizer 
using Arduino and toy keyboard.

## Synths

### 01_SineSynthArpegiatorMega_Harsh

Synthesizer for Arduino Mega using toy keyboard with harsh sound
generated from sine wave. Output goes through buzzer or "Troika" audio amplifier to minijack.


### 02_SineSynthArpegiatorMega_Smoother

Synthesizer for Arduino Mega using toy keyboard with smoother sound
generated from sine wave. Output goes through buzzer or "Troika" audio amplifier to minijack.

### 03_SineSynthArpegiatorMega_Sliders

Synthesizer for Arduino Mega and toy keyboard 
with two sliders controlling 1-bit dithering algorithm. Output goes through buzzer or "Troika" audio amplifier to minijack.

### 04_SineSynthArpegiatorMega_Sliders_Boombox

Synthesizer for Arduino Mega and toy keyboard 
with two sliders controlling 1-bit dithering algorithm. Output goes through buzzer or "Troika" audio amplifier to minijack.
Also to A0 connected toy microphone coupled with potentiometer to output to A0 512 in silence.
Also connected button from microphone. When button is pressed is sends signal instead synthesizing.
So synthesizer works as boombox 1-bit engine too.

Hint: setting both 1-bit sliders to zero and just presing button generates glitch sound!
But to normal mic sound - increase both 1-bit sliders.

### Roadmap

* Sampler: sample sound and play by keys
* Keyboard advance: use pairs of octave for more octaves or for switch samples/synth
* Sound Engine: control sample rate, attact, decay, release
* Synthesizing module: triangle, sawtooth, square waves, pulse synthesys.

