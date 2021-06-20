//pot_indicator_02_fade - module consisting of potentiometer and 10-led indicator.
//Features:
//Fading betweed leds when showing pot position, using Timer1 for Nano and custom-implemented PWM.

//See photos at pot_indicator_making

//Controller: Nano (5V)

//Libraries required:
//- TimerOne (should be installed using "Manage Libraries")

//Connections:
//Using 250000 Baud for Serial

#include <TimerOne.h>

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
const byte led_n = 10;    //number of leds in indicator


const byte pwm_max = 80;        //max pwm value
const int timer_period = 25;    //microseconds

const int values_range = 1024;  //range of pot input
const byte led_max_brightness = pwm_max / 16; //max brightness  - CAN TUNE THIS

const byte pin_led0 = 2; //D2 - started pin, next D3, D4,...

//-----------------------------------------------------------------------
void setup() {
  Serial.begin(250000);
  prln();
  prln("----------------------------------------------------------------");
  prln("pot_indicator_02_fade, pot connected to A0, 10-led indicator to D2,D3,...,D11");
  prln("----------------------------------------------------------------");

  //LED outputs
  for (byte i=0; i<led_n; i++) {
    pinModePower(pin_led0 + i, 0);
  }

  //Timer setup
  Timer1.initialize(timer_period); 
  Timer1.attachInterrupt(timer_proc); 
}

//-----------------------------------------------------------------------


volatile byte timer_counter = 0;
volatile byte led_id1=0;     // use volatile for shared variables
volatile byte led_id2=0;
volatile byte led_value1=0;
volatile byte led_value2=0;

void timer_proc(void)
{
  digitalWrite(pin_led0 + led_id1, timer_counter < led_value1);  
  digitalWrite(pin_led0 + led_id2, timer_counter < led_value2);  
  
  timer_counter++;
  //if (timer_counter>=led_max) timer_counter=0;
 timer_counter %= pwm_max;
}

//-----------------------------------------------------------------------
byte prev_k1 = 0;
byte prev_k2 = 0;

void set_led_value(int v) {
  //v in 0..values_range-1

  float t = float(v)/(values_range-1) * led_n;  //[0..led_n]
  byte k1 = min(int(t), led_n-1);             //first led
  byte k2 = min(k1+1, led_n-1); //second led - edge case for last led

  //crop t
  if (t <= k1+0.3) t = k1;
  if (t >= k1+0.7) t = k2;

  float br2 = (k1 == k2)?1:(t-k1);   //0..1
  float br1 = (k1 == k2)?1:(1-br2); 

  //non-linear transform
  //br1 *= br1;
  //br2 *= br2;
  
  byte bright1 = int(br1 * led_max_brightness); 
  byte bright2 = int(br2 * led_max_brightness);; 


  //set to timer's proc values
  noInterrupts();
  //disable leds if changed k1,k2
  if (k1 != prev_k1 || k2 != prev_k2) {
    digitalWrite(pin_led0 + prev_k1,0);
    digitalWrite(pin_led0 + prev_k2,0);

    prev_k1 = k1;
    prev_k2 = k2;
  }

  led_id1 = k1;
  led_value1 = bright1;
  led_id2 = k2;
  led_value2 = bright2;
  
  interrupts();

}

//-----------------------------------------------------------------------
void loop() {
  int v = analogRead(A0);
  pr("A0 "); prln(v);
  set_led_value(v);
  prln("-----------------------------");  
  
  delay(50);
}

//-----------------------------------------------------------------------
