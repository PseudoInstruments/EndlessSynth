//EndlessSynth, Toy Piano Keyboard Pins Test

//Toy Piano sensor was prepared: I made small holes and solder wires to each key.
//So we have 32 out pins and pin for GND

//Device: Arduino Mega
//Connection:
//- 32 keys and Gnd from Toy Piano to Arduino Mega:
//22,24,26,...,36,
//23,25,27,...,37,
//38,40,42,...,52,
//39,41,43,...,53
//and Gnd
//(key numbers are reverted)

//Monitor port:
//- baud must be set to 500000!

const int N = 32;
int keyb[N] = {22, 24, 26, 28, 30, 32, 34, 36, 
               23, 25, 27, 29, 31, 33, 35, 37,
               38, 40, 42, 44, 46, 48, 50, 52,
               39, 41, 43, 45, 47, 49, 51, 53};

void setup() {
  //Печать приветствия
  Serial.begin(500000);
  Serial.println("EndlessSynth Keyboard Pins Polyphonic tester 1.1");
  Serial.print("  ");
  for (int i = 0; i < N; i++) {
    Serial.print(keyb[i]); Serial.print(" ");
  }
  Serial.println();

  //Setup pins
  for (int i = 0; i < N; i++) {
    pinMode(keyb[i], INPUT_PULLUP);
  }
}

void loop() {
  //Check pins and print
  for (int i = 0; i < N; i++) {
    if (i > 0 && i % 8 == 0) Serial.print(" ");
    int v = digitalRead(keyb[i]);
    if (v == LOW) Serial.print("X");
    else Serial.print("-");
    //int v = analogRead(keyb[i]);
    //Serial.print(v); Serial.print("\t");
  }
  Serial.println();
  delay(200);


}
