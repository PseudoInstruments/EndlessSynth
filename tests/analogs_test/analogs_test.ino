//analogs_test - Read A0..A7 on Mega/Due.

//Tutorial on multiplexer:
//http://adam-meyer.com/arduino/CD74HC4067

//Connections:
//Using 250000 Baud for Serial, because Due can't work properly with 500000 which I would prefer...
//https://forum.arduino.cc/t/arduino-due-baudrate-higher-than-115200/539463/4

//--------------------------------------------
//Types
typedef unsigned char uint8;
typedef char int8;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef int int32;

const int max_u16 = 65535;
const long int u16_n = 65536;

typedef long long int int64;
typedef unsigned long long int uint64;


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void setup() {
  Serial.begin(250000);
  prln();
  prln("----------------------------------------------------------------");
  prln("analogs_test, prints A0..A7");
  prln("----------------------------------------------------------------");

}

//-----------------------------------------------------------------------
void loop() {
  pr("A0 "); prln(analogRead(A0));
  pr("A1 "); prln(analogRead(A1));
  pr("A2 "); prln(analogRead(A2));
  pr("A3 "); prln(analogRead(A3));
  pr("A4 "); prln(analogRead(A4));
  pr("A5 "); prln(analogRead(A5));
  pr("A6 "); prln(analogRead(A6));
  pr("A7 "); prln(analogRead(A7));
  
  prln("-----------------------------");  
  
  delay(300);
}

//-----------------------------------------------------------------------
