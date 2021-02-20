//This file contains utility functions for get note frequency
//http://newt.phys.unsw.edu.au/jw/notes.html

float tone_adjust_ = 0; //-1..1 - adjustment of tone

//Convert midi note to frequency
int m_to_f_int(int midi_note) {
  return m_to_f_float(midi_note);
}

//float version
float m_to_f_float(float midi_note) {
  return pow(2,(tone_adjust_ + midi_note-69)/12)*440; //Hz
  //note: for midi_note>100 sometimes occurs overflow
}

void test_notes() {
  Serial.println("Test notes:");
  Serial.print("A "); Serial.println(m_to_f_int(69));
  Serial.print("B "); Serial.println(m_to_f_int(70));
}
//---------------------------------------------------------------
