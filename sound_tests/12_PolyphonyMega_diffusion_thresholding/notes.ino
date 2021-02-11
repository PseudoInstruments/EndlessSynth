//This file contains utility functions for get note frequency
//http://newt.phys.unsw.edu.au/jw/notes.html

float m_to_f(float midi_note) {
  return pow(2,(midi_note-69)/12)*440; //Hz
  //note: for midi_note>100 sometimes occurs overflow
}

void test_notes() {
  Serial.println("Test notes:");
  Serial.print("A "); Serial.println(m_to_f(69));
  Serial.print("B "); Serial.println(m_to_f(70));
}
//---------------------------------------------------------------
