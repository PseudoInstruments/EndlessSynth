# EndlessSynth
1-bit synthesizers and other instruments using Arduino,
with toy keyboard and microphone and self-made "guitar".

 ![Perf image](https://raw.githubusercontent.com/EndlessBits/EndlessSynth/main/doc/perf-2021-02-18-12h20m.png)
  
## Instruments

### Endless Boombox 

Endless boombox is a instrument made from toy microphone and Arduino Uno. 
It processes input sound to 1 bit.

Performance video: https://youtu.be/3pW5L71EReI

Code and implementation details: boomboxes/01_boombox_simple


### Endless Guitar

Endless Guitar is a 1-string 1-bit guitar with controllable volume, sample rate and sensitivity.

How it works: 
<iframe width="560" height="315" src="https://www.youtube.com/embed/se3GxTyyy_U" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


Code and implementation details: guitars/02_guitar_sliders

### Endless Synth

Endless Synth is a 1-bit synthesizer which generates sine wave.
It has sliders to control volume, diffuse step and history duration.
Also from keys user can switch octaves and sample rate.

The style of playing is like guitar - you choose "notes" by the left hand and hit "strings" by the right hand.
This style fits well for live performance of arpeggios found in 8-bit games music.

Performance video: https://youtu.be/sMg1RHxQQ2w

Code and assembly insructions: synths folder.


## Performances

 https://youtu.be/nv2vj5iOq3Y 
 
 https://youtu.be/5nkdpzOd9-w 

 

## Contents

* **boomboxes** - Endless Boombox codes and instructions.
* **doc** - Some images.
* **guitars** - Endless Guitar codes and instructions.
* **keyboard_tests** - tests of connecting toy keyboards.
* **mic_tests** - tests of using microphone with Arduino.
* **synthesis_tests** - tests of synthesizing sound in Arduino.
* **synths** - Endless Wynth codes and instructions.

