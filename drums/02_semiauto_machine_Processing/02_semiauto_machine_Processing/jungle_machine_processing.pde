/**
Semi-automatic drum machine.
Press 
1,q,a,z - kick
2,w,s,x - snare
3,e,d,c - snare2
4,r,f,v - hat

In a given tempo, for a given drum, 
pressing first key in a row gives repeated drum in 4 beats, 
second in 3 beats, then 2 and 1.
In real MIDI controller, we can hit with different velocity and so affect number of repeats,
but here it's fixed.

 */

import processing.sound.*;

int delay1_ms = 1000/16;  //Tempo

int n = 4;
SoundFile soundfile[];

//Repeater
int hit_count[];   //how much hits required
int hit_time[];  //time of next hit
int hit_beats[];  //delay between beats

//--------------------------------------------------------------------
void setup() {
  size(640, 360);
  background(255);

  // Repeater
  hit_count = new int[n];
  hit_time = new int[n];
  hit_beats = new int[n];

  // Load a soundfiles
  soundfile = new SoundFile[n];
  soundfile[0] = new SoundFile(this, "38898__sandyrb__hmg-kick-001.wav");
  soundfile[1] = new SoundFile(this, "36249__sandyrb__h134-snare-01.wav");
  soundfile[2] = new SoundFile(this, "89500__menegass__sd06.wav");
  soundfile[3] = new SoundFile(this, "801__bdu__closehat.aiff");

  // These methods return useful infos about the file
  //println("SFSampleRate= " + soundfile.sampleRate() + " Hz");
  //println("SFSamples= " + soundfile.frames() + " samples");
  //println("SFDuration= " + soundfile.duration() + " seconds");

  // Play the file in a loop
  //soundfile[0].loop();
}      

//--------------------------------------------------------------------
//we need 'time' passed to have more accurate repeating timing 
void play_repeat(int i, int repeats, int beats, int time) { 
  //println(i);

  // Play now
  soundfile[i].play();
  
  hit_beats[i] = beats;
  
  // Plan next repeats
  repeats--;
  hit_count[i] = repeats;
  if (repeats > 0) {
      hit_time[i] = time + delay1_ms * beats;
  }
  
  //kick, snare, snare2 mutes each other
  for (int j=0; j<3; j++) {
    if (j != i) hit_count[j] = 0;
  }  
}

//--------------------------------------------------------------------
void play(int i, int repeats, int beats) { 
   play_repeat(i, repeats, beats, millis()); 
}

//--------------------------------------------------------------------
void keyPressed() {
  if (key == '1') play(0, 1, 4);
  if (key == 'q') play(0, 4, 4);
  if (key == 'a') play(0, 2, 2);
  if (key == 'z') play(0, 4, 1);

  if (key == '2') play(1, 1, 4);
  if (key == 'w') play(1, 4, 4);
  if (key == 's') play(1, 4, 2);
  if (key == 'x') play(1, 4, 1);
  
  if (key == '3') play(2, 1, 4);
  if (key == 'e') play(2, 4, 4);
  if (key == 'd') play(2, 4, 2);
  if (key == 'c') play(2, 4, 1);
  
  if (key == '4') play(3, 1, 4);
  if (key == 'r') play(3, 8, 4);
  if (key == 'f') play(3, 8, 2);
  if (key == 'v') play(3, 8, 1);
}

//--------------------------------------------------------------------
void draw() {
  background(0);
  textSize(32);
  text("Keys:\n   1,q,a,z - kick\n   2,w,s,x - snare\n   3,e,d,c - snare2\n   4,r,f,v - hat", 10, 30); 
  

  //repeats
  int time = millis();
  for (int i=0; i<n; i++) {
    if (hit_count[i] > 0 && time >= hit_time[i]) {
       play_repeat(i, hit_count[i], hit_beats[i], hit_time[i]);  
       // It's important that we pass hit_time instead millis to improve repeat accuracy. 
    }
  }
}



//--------------------------------------------------------------------
