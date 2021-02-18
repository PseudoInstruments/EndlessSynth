# EndlessSynth
1-bit synthesizers and other instruments using Arduino,
with toy keyboard and microphone and self-made "guitar".

 ![Perf image](https://raw.githubusercontent.com/EndlessBits/EndlessSynth/main/doc/perf-2021-02-18-12h20m.png)
  
Made by Endless Attractions Museum, https://eamuseum.com/Endless-Instruments

## Instruments

### Endless Boombox 

Endless boombox is a instrument made from toy microphone and Arduino Uno. 
It processes input sound to 1 bit.

Performance video: 

[![How it works](https://img.youtube.com/vi/3pW5L71EReI/0.jpg)](https://www.youtube.com/watch?v=3pW5L71EReI)

Code and implementation details: boomboxes/01_boombox_simple


### Endless Guitar

Endless Guitar is a 1-string 1-bit guitar with controllable volume, sample rate and sensitivity.

How it works: 

[![How it works](https://img.youtube.com/vi/se3GxTyyy_U/0.jpg)](https://www.youtube.com/watch?v=se3GxTyyy_U)

Code and implementation details: guitars/02_guitar_sliders

### Endless Synth

Endless Synth is a 1-bit synthesizer which generates sine wave.
It has sliders to control volume, diffuse step and history duration.
Also from keys user can switch octaves and sample rate.

The style of playing is like guitar - you choose "notes" by the left hand and hit "strings" by the right hand.
This style fits well for live performance of arpeggios found in 8-bit games music.

How it works: 

[![How it works](https://img.youtube.com/vi/sMg1RHxQQ2w/0.jpg)](https://www.youtube.com/watch?v=sMg1RHxQQ2w)

Code and assembly insructions: synths folder.


## Performances

[![Performance part 2](https://img.youtube.com/vi/nv2vj5iOq3Y/0.jpg)](https://www.youtube.com/watch?v=nv2vj5iOq3Y)

[![Performance part 3](https://img.youtube.com/vi/5nkdpzOd9-w/0.jpg)](https://www.youtube.com/watch?v=5nkdpzOd9-w)
 

## Contents

* **boomboxes** - Endless Boombox codes and instructions.
* **doc** - Some images.
* **guitars** - Endless Guitar codes and instructions.
* **keyboard_tests** - tests of connecting toy keyboards.
* **mic_tests** - tests of using microphone with Arduino.
* **synthesis_tests** - tests of synthesizing sound in Arduino.
* **synths** - Endless Wynth codes and instructions.

