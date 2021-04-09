//This file contains utility functions for get note frequency
//http://newt.phys.unsw.edu.au/jw/notes.html


//Convert midi note to frequency
int m_to_f_int(int midi_note) {
  return m_to_f_float(midi_note);
}

//float version
float m_to_f_float(float midi_note) {
  return pow(2,(Pot_Tone + midi_note-69)/12)*440; //Hz    //Uses slider Tone
  //note: for midi_note>100 sometimes occurs overflow
}

void test_notes() {
  prln("Test notes:");
  pr("A "); prln(m_to_f_int(69));
  pr("B "); prln(m_to_f_int(70));
}
//---------------------------------------------------------------
