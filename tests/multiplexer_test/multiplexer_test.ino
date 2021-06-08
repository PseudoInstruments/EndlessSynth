//multiplexer_test - single multiplexer on Arduino Due.

//Tutorial on multiplexer:
//http://adam-meyer.com/arduino/CD74HC4067

//--------------------------------------------
//Notes:
//--------------------------------------------
//This sketch connects 16-channel multiplexer with 16 connected pots and gets it output to A1.

//--------------------------------------------
//Controller: Arduino Due / Mega

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
const int u16_n = 65536;

typedef long long int int64;
typedef unsigned long long int uint64;


//-----------------------------------------------------------------------
//gnd, 3.3v, пропуск, 4 синих - адресные, 23,25,27,29, желтый - A1, зеленый - 49.
//Правый шлейф включения мультиплексоров - 31,33,35,37,39,41,43,45,47.

const byte MULTI_CH = 16;       //number of multiplexer channels
const byte MULTI_ADDRESSES = 4; //number of multiplexer addreses

byte PIN_MULTI_ENABLE = 31;
byte PIN_ADDR[MULTI_ADDRESSES] = {23, 25, 27, 29};
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

  //enable mux - by sending "LOW" to its EN pin
  pinModePower(PIN_MULTI_ENABLE, 0); 
  
  

}
//-----------------------------------------------------------------------
void read_mux() {
  pr("Read mux:   "); 
  
  for (byte i=0; i<MULTI_CH; i++) {
    //decode address
    byte addr = i;
    for (byte k=0; k<MULTI_ADDRESSES; k++) {
      byte v = addr%2;
      digitalWrite(PIN_ADDR[k], v?HIGH:LOW);
      addr /= 2;
    }
    int input = analogRead(PIN_ANALOG);
    pr(" "); pr(input);
  }
  prln();

}

//-----------------------------------------------------------------------
void loop() {
  read_mux();
  prln("-----------------------------");  
  
  delay(1000);
}

//-----------------------------------------------------------------------
