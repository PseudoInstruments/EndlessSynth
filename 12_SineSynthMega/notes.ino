//This file contains utility functions for get note frequency
//http://newt.phys.unsw.edu.au/jw/notes.html

//Convert midi note to frequency
//if midi_note == -1 result is 0;
int m_to_f_int(int midi_note) {
  if (midi_note == -1) return 0;
  return m_to_f_float(midi_note);
}

//float version
float m_to_f_float(float midi_note) {
  if (midi_note == -1) return 0;
  return pow(2,(midi_note-69)/12)*440; //Hz
  //note: for midi_note>100 sometimes occurs overflow
}

void test_notes() {
  Serial.println("Test notes:");
  Serial.print("A "); Serial.println(m_to_f_int(69));
  Serial.print("B "); Serial.println(m_to_f_int(70));
}
//---------------------------------------------------------------
