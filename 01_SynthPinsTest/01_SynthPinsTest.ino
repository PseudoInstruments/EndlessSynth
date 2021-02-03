//EndlessSynth, Toy Piano Keyboard Pins Test

//Device: Arduino Mega
//Connection:
//- 12 pins from Toy Piano to Arduino Mega: 22,24,26,...,44
//Monitor port:
//- baud must be set to 500000!

/*
pins: 0 1 2 3 4 5 6 7 8 9 10 11
keys: 0..31
 
pin 0 and 4,5,6,7,8,9,10,11 - keys 0..7 
pin 1 and 4,5,6,7,8,9,10,11 - keys 8..15 
pin 2 and 4,5,6,7,8,9,10,11 - keys 16..23 
pin 3 and 4,5,6,7,8,9,10,11 - keys 24..31 
(key numbers are reverted)

NOTE: there is some issue with switching control pins 0,1,2,3:
if for example key 0 and 8 is pressed:
- then if pin 0 and pin 1 will be in opposite state - the result is flowing + to -, short circuit.
  To resolve the problem, we must use diodes, or just disable current from control pins 0,1,2,3 when not used
  But we will deal with it in the next sketch, here we just testing pins communation
*/
//Result: keyboard is polyphonic, can be read by blocks of 8 keys


const int N = 12;
int keyb[N] = {22,24,26,28,30,32,34,36,38,40,42,44};

int ground = 0; //индекс пина для земли - пытаемся обнаружить логику пинов, перебирая ее
             //1;
             //2;
             //3;

void setup() {
  //Печать приветствия
  Serial.begin(500000);
  Serial.println("EndlessSynth Keyboard Pins tester 1.1");
  Serial.print("  ");
  for (int i=0; i<N; i++) {
    Serial.print(keyb[i]); Serial.print(" ");
  }
  Serial.println();

  //Установка пинов
  for (int i=0; i<N; i++) {
    if (i == ground) {   //выдаем на этот пин GND, а остальные включаем на вход
      pinMode(keyb[i], OUTPUT);
      digitalWrite(keyb[i], LOW);
    }
    else {
      pinMode(keyb[i], INPUT_PULLUP);
    }
  }
}

void loop() {
  //Проверка пинов и выдача на печать
  for (int i=0; i<N; i++) {
    if (i == ground) Serial.print("*");
    else {
      int v = digitalRead(keyb[i]);
      if (v == LOW) Serial.print("X");
      else Serial.print("-");
    }  
  }
  Serial.println();
  delay(100);


}
