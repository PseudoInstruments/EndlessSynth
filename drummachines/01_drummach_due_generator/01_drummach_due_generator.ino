//01_drummach_due_generator - drummachine sound test for Arduino Due.

//--------------------------------------------
//Features:
//--------------------------------------------
// This is test of sound:
// - setup timers for control and audio generation

//--------------------------------------------
//Controller: Arduino Due
//Additional Libs to install: Due Timer

//Connections:
//Using 250000 Baud for Serial, because Due can't work properly with 500000 which I would prefer...
//https://forum.arduino.cc/t/arduino-due-baudrate-higher-than-115200/539463/4

//Connect Jack output to Due:
//- Due Gnd -> Jack Gnd
//- Due DAC0 -> Resistor on 500 Ohm -> Jack Signal (if mono, if stereo - to both signals).
//--------------------------------------------

//Links on sound generating with Due:
//http://groovuino.blogspot.com/
//http://rcarduino.blogspot.com/2012/12/arduino-due-dds-part-1-sinewaves-and.html
//https://github.com/Gaetino/Groovuino

//-----------------------------------------------------------------------
void setup() {
  Serial.begin(250000);
  prln();
  prln("----------------------------------------------------------------");
  prln("Endless Drummachine 01_drummach_due_generator - drummachine sound test for Arduino Due, v. 1.1");
  prln("----------------------------------------------------------------");

}

//-----------------------------------------------------------------------
void loop() {

}

//-----------------------------------------------------------------------
