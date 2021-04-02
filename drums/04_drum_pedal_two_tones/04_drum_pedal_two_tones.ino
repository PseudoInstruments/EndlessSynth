/* Endless Drums Pedal generates two drum sounds by pressing physical pedal or keys.

  Controller: Arduino Mega
  ----------------------------------------
  Connection
  ----------------------------------------
  1) Audio output: D2 (Sig) and D3 (Gnd)

  2) Pedal Sustain - D11 (Sig) and D12 (Gnd)

  3) Sliders - this sketch uses two sliders (I mean potentiometers 10kOhm) for controlling sound parameters.
  A4 - sample rate
  A5 - duration
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

  sliders_setup();
  
  sound_setup();


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
