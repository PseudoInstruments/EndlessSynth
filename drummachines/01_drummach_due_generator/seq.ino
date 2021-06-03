//Sequencer

float seq_tempo_ = 120;

uint32 seq_tick_ = 0;
uint32 seq_time_ = 0; 

uint32 seq_step_ = 0;

const int seq_n = 16; //
DrumSound

//---------------------------------------------------------------
void seq_setup() {
  seq_time_ = millis();
}

//---------------------------------------------------------------
void seq_update() {
  uint32 time = millis();
  uint32 tick_step = 60000 / (seq_tempo_*4);

  if (seq_time_ + tick_step <= time) {
    pr("seq step ");
    prln(seq_step_);

    //play new drum
    
    
    seq_step_++;
    seq_step_ %= 16;
    int is_tick = 0;
    while (seq_time_ + tick_step <= time) {
      seq_time_ += tick_step; 
    }
  }
  
}

//---------------------------------------------------------------
