//02_drummach_multiplex_test - testing drummachine pots and keys with multiplexers on Arduino Due.

//Tutorial on multiplexer:
//http://adam-meyer.com/arduino/CD74HC4067

//--------------------------------------------
//Notes:
//--------------------------------------------
//This sketch connects to 9 16-channel multiplexors and gets it output to A1.
//For reading keys, we enable additional 10kOhm resistor.

//--------------------------------------------
//Controller: Arduino Due

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
//gnd, 3.3v, пропуск, 4 синих - адресные, 23,25,27,29, желтый - A1, зеленый - 49.
//Правый шлейф включения мультиплексоров - 31,33,35,37,39,41,43,45,47.

const byte MULTI_N = 9; //number of multiplexers
const byte MULTI_CH = 16;       //number of multiplexer channels
const byte MULTI_ADDRESSES = 4; //number of multiplexer addreses

byte PIN_MULTI_ENABLE[MULTI_N] = {31, 33, 35, 37, 39, 41, 43, 47};
byte PIN_ADDR[MULTI_ADDRESSES] = {29, 27, 25, 23}; //{23, 25, 27, 29};
byte PIN_ANALOG = A1;
byte PIN_RESISTOR = 49;

//-----------------------------------------------------------------------
void setup() {
  Serial.begin(250000);
  prln();
  prln("----------------------------------------------------------------");
  prln("Endless Drummachine 02_drummach_multiplex_test - test pots, keys, multiplexors, v. 1.1");
  prln("----------------------------------------------------------------");

  //Multiplexors pins setup
  for (byte i=0; i<MULTI_ADDRESSES; i++) {
    pinModePower(PIN_ADDR[i], 0);
  }

  for (byte i=0; i<MULTI_N; i++) {
    pinModePower(PIN_MULTI_ENABLE[i], 1); //1 - disables mux
  }
  //Resistor... 

}


//-----------------------------------------------------------------------
void set_addr(int i) {
    //set address
    //pr("set addr ");
    //pr(i); //pr("   ");
    //prln();
    
    byte addr = i;
    for (byte k=0; k<MULTI_ADDRESSES; k++) {
      byte v = addr%2;
      //pr(v);
      digitalWrite(PIN_ADDR[k], v);
      addr /= 2;
    }
  
}

//-----------------------------------------------------------------------
int read_analog() {
    //delay(1); //not required, but for test
    int input = analogRead(PIN_ANALOG);
    //pr("analog "); pr(input);
    //prln();
    return input;
}

//-----------------------------------------------------------------------
void read_mux(int q) {
  pr("mux "); pr(q+1); pr(":   ");
  //enable mux - by sending "LOW" to its EN pin
  digitalWrite(PIN_MULTI_ENABLE[q], LOW);
  
  for (byte i=0; i<MULTI_CH; i++) {
    //pr(" "); pr(i+1);
    //set address
    set_addr(i);
    //Read values, both for pots and buttons
    
    int analog = read_analog();
    if (i % 4 == 0) pr(" | ");
    else pr(" "); pr(analog);
    //prln();
  }
  prln();
  
  //disable MUX
  digitalWrite(PIN_MULTI_ENABLE[q], HIGH);

}

//-----------------------------------------------------------------------
//Main loop - used for audio
void loop() {
  prln("------------");  //Separator - used in processing
  //read_mux(0);
  for (byte i=0; i<MULTI_N; i++) {
    read_mux(i);
  }
  
  delay(300);
}

//-----------------------------------------------------------------------
