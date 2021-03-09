//ADSR envelope 
//It works at control rate
//The ADSR parameters are computed at key pressing and not changing until all keys are released.
//ASDR processes all keys using single envelope, which starts on first key is pressed and ends an all keys are released.


byte adsr_key = 0;             //is key pressed             
unsigned long adsr_attack = 0;  //time of pressing key

//---------------------------------------------------------------
void ADSR_setup() {
  
}

//---------------------------------------------------------------
void ADSR_loop(unsigned int time) {

}

//---------------------------------------------------------------
//event: key pressed
void ADSR_key_pressed(unsigned int time) {
  adsr_key = 1;
    //compute timing
   // adsr_time_
    
}

//---------------------------------------------------------------
//event: key released
void ADSR_key_released() {
  adsr_key = 0;
}

//---------------------------------------------------------------
