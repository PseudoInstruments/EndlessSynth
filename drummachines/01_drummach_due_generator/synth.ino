//Sound



//current playing drum
DrumSound drum_;

//---------------------------------------------------------------
//start sound
//void synth_sound_play(DrumSound drum) {

//}

//---------------------------------------------------------------
//playing sound
AudioSample synth_audio_out() {
  return drum_.audio_out();
}

//---------------------------------------------------------------
