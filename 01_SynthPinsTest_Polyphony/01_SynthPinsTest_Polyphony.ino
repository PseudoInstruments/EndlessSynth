//EndlessSynth, Toy Piano Keyboard Pins Test

//Toy Piano sensor was prepared: I made small holes and solder wires to each key.
//So we have 32 out pins and pin for GND

//Device: Arduino Mega
//Connection:
//- 32 keys and Gnd from Toy Piano to Arduino Mega:
//22,...,53
//and Gnd
//(key numbers are reverted)

//Monitor port:
//- baud must be set to 500000!

const int N = 32;
int keyb[N] = {22, 23, 24, 25, 26, 27, 28, 29, 
               30, 31, 32, 33, 34, 35, 36, 37,
               38, 39, 40, 41, 42, 43, 44, 45,
               46, 47, 48, 49, 50, 51, 52, 53};

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
    int v = digitalRead(//keyb[i]);  //reverted order, for soldering testing
      keyb[N-1-i]); //natural order
    if (v == LOW) Serial.print("X");
    else Serial.print("-");
    //int v = analogRead(keyb[i]);
    //Serial.print(v); Serial.print("\t");
  }
  Serial.println();
  delay(200);


}
