//multiplexer_test - single multiplexer on Arduino Uno/Mega/Due.

//Tutorial on multiplexer:
//http://adam-meyer.com/arduino/CD74HC4067

//--------------------------------------------
//Notes:
//--------------------------------------------
//This sketch connects 16-channel multiplexer with 16 connected pots and gets it output to A1.

//--------------------------------------------
//Controller: Arduino Uno/Mega/Due

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

const byte MULTI_CH = 16;       //number of multiplexer channels
const byte MULTI_ADDRESSES = 4; //number of multiplexer addreses

//byte PIN_MULTI_ENABLE = 31; //not used, connect to Gnd
byte PIN_ADDR[MULTI_ADDRESSES] = {2, 3, 4, 5};  //Address pins
byte PIN_ANALOG = A0;   //signal input from multiplexer for pots
byte PIN_BUTTON = 7;   //signal input from multiplexer for buttons (Arduino will pullup it at start)

//Test values sent to multiplexer - connect to multiplexer channels
const byte test_pins=3;
byte test_pin[test_pins] = {8, 9, 10};

//-----------------------------------------------------------------------
void setup() {
  Serial.begin(250000);
  prln();
  prln("----------------------------------------------------------------");
  prln("Multiplexer_test, A0 - check pots, D7 - check buttons");
  prln("----------------------------------------------------------------");

  //Multiplexors pins setup
  for (byte i=0; i<MULTI_ADDRESSES; i++) {
    pinModePower(PIN_ADDR[i], 0);
  }

  //enable mux - by sending "LOW" to its EN pin
  //pinModePower(PIN_MULTI_ENABLE, 0); 

  pinModePower(test_pin[0],1);
  pinModePower(test_pin[1],0);
  pinModePower(test_pin[2],1);

  //button pullup
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  //Test
  set_addr(1);

}

//-----------------------------------------------------------------------
void dig_write(int pin, int v) {
  byte val = v?HIGH:LOW;
  //pr("dig write "); pr(pin); pr(" "); prln(val);
  digitalWrite(pin, val);
  
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
      dig_write(PIN_ADDR[k], v);
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
int read_button() {
    //delay(1); //not required, but for test
    int input = 1-digitalRead(PIN_BUTTON);
    //pr("button "); pr(input);
    //prln();
    return input;
}


//-----------------------------------------------------------------------
void read_mux() {
  prln("Read mux:   "); 
  
  for (byte i=0; i<MULTI_CH; i++) {
    pr(" "); pr(i+1);
    //set address
    set_addr(i);
    //Read values, both for pots and buttons
    
    int button = read_button();
    int analog = read_analog();
    pr("\t"); pr(button); pr("  "); pr(analog);
    prln();
  }
  prln();

}

//-----------------------------------------------------------------------
void loop() {
  //read_val();
  //set_addr(1);
  
  read_mux();
  //prln("-----------------------------");  
  
  delay(300);
}

//-----------------------------------------------------------------------
