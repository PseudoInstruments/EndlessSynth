/* Endless Drums Pedal generates two drum sounds by pressing physical pedal or keys.

  ----------------------------------------
  Features:
  - Two drum tones, controlled from pedal and keys
  - Note-scale sample rate
  
  ----------------------------------------
  Controller: Arduino Mega
  ----------------------------------------
  Connection
  ----------------------------------------
  1) Audio output: D2 (Sig) and D3 (Gnd)

  2) Pedal Sustain - D11 (Sig) and D12 (Gnd)

  3) Sliders (potentiometers 10kOhm) for controlling sound parameters:
  - Sample rate pot (10 kOhm, and others pots too)
  - Duration 1 pot
  - Duration 2 pot
  - Route Pedal to tone 1 switch
  - Route Pedal to 2 switch
  - Key for manual playing tone 1
  - Key for manual playing tone 2
  
  
  And for pots connect Gnd, 5V

  //It's better to use linear pots.
  ----------------------------------------
*/

//--------------------------------------------------------------

int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("Endless Drum Pedal Two Tones, v. 1.1 for Arduino Mega");
  Serial.println("Send '1' to enable debug print to console");

  //Note: do this performance measure before sliders setup
  sound_measure_performance();  
 
  sliders_setup();  

  sound_setup();  //need to be after sliders to set up drum sounds

}

//--------------------------------------------------------------
inline void control_step() {
  //Serial
  if (Serial.available() > 0) { //we expect only "1" rare to on/off debugging
    int key = Serial.read();
    if (key == '1') {
      debug = 1 - debug;
      Serial.print("Debug "); Serial.println(debug);
    }
  }

  //Sliders
  sliders_slow_control_step();

  //Sound
  sound_control_step();

  //Debug print
  if (debug) {
    sliders_debug_print();
    sound_debug_print();

    Serial.println();
  }
}

//--------------------------------------------------------------
void loop() {

  //run control step
  control_step();

  //run audio loop
  sound_audio_loop();
  

}
//--------------------------------------------------------------
