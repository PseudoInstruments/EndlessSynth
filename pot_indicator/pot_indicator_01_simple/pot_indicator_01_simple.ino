//pot_indicator_01_simple - module consisting of potentiometer and 10-led indicator.
//Indicator shows pot position strictly, without fading.

//Connections:
//Using 250000 Baud for Serial

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

byte pin_led0 = 2; //D2 - started pin, next D3, D4,...
byte led_n = 10;    //10 leds 

//-----------------------------------------------------------------------
void setup() {
  Serial.begin(250000);
  prln();
  prln("----------------------------------------------------------------");
  prln("pot_indicator_01_simple, pot connected to A0, 10-led indicator to D2,D3,...,D11");
  prln("----------------------------------------------------------------");


  for (byte i=0; i<led_n; i++) {
    pinModePower(pin_led0 + i, 0);
  }

}

//-----------------------------------------------------------------------
byte prev_led = 0;
void set_led_value(int i) {
  if (i >= 0 && i < led_n) {
    digitalWrite(pin_led0 + prev_led, 0);
    digitalWrite(pin_led0 + i, 1);
    prev_led = i;
  }
}

//-----------------------------------------------------------------------
void loop() {
  int v = analogRead(A0);
  pr("A0 "); prln(v);

  set_led_value(map(v,0,1024,0,led_n));
  /*
  //running wave
  static int k = 0;
  static int i0 = 0;
  byte i = k/5;
  digitalWrite(pin_led0 + i0, 0);
  digitalWrite(pin_led0 + i, 1);

  i0 = i;
  k++;
  if (k>=led_n*5) k = 0;*/
  
  prln("-----------------------------");  
  
  delay(50);
}

//-----------------------------------------------------------------------
