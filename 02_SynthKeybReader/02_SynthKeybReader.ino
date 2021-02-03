//EndlessSynth, Toy Piano Keyboard Reader Test

//Device: Arduino Mega
//Connection:
//- 12 pins from Toy Piano to Arduino Mega: 22,24,26,...,44
//Monitor port:
//- baud must be set to 500000!

//pins 0,1,2,3 - choosing keys block, 
//pins 4,5,6,7,8,9,10,11 - keys 0..7, 8..15, 16..23, 24..31 
//(keys numbers is reverted)
/*NOTE: there is some issue with switching control pins 0,1,2,3:
if for example key 0 and 8 is pressed:
- then if pin 0 and pin 1 will be in opposite state - the result is flowing + to -, short circuit.
  To resolve the problem, we disable current from control pins 0,1,2,3 when not used

The next problem: if we press several keys in one block 
and then one key in another block - it will give response of the all keys in the second block!
To resolve it, we need to solder keys separately... or put many diodes.
*/

const byte keyGndN = 4;
const byte keyReadN = 8;
byte keyGnd[keyGndN] = {22,24,26,28};
byte keyRead[keyReadN] = {30,32,34,36,38,40,42,44};

const byte keys = 32;  //number of keys
byte key_state[keys]; //key state - pressed or released 
//TODO can store in a bit mask

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Keyboard Reader 1.1");
  Serial.print("  keyb pins:");
  for (int i=0; i<keyGndN; i++) {
    Serial.print(keyGnd[i]); Serial.print(" ");
  }
  for (int i=0; i<keyReadN; i++) {
    Serial.print(keyRead[i]); Serial.print(" ");
  }
  Serial.println();
  Serial.println("Now press the keys!");

  //Установка пинов
  for (int i=0; i<keyGndN; i++) {
      pinMode(keyGnd[i], INPUT);  //disable current from control pin, see NOTE above
  }
  for (int i=0; i<keyReadN; i++) {
      pinMode(keyRead[i], INPUT_PULLUP);
  }
}

//---------------------------------------------------------------
void keys_update() {
  //Обновление состояния клавиатуры
  for (byte k=0; k<keyGndN; k++) {
    //set control pin 
    for (byte j=0; j<keyGndN; j++) {
      if (j == k) {
        pinMode(keyGnd[j], OUTPUT);  //put current
        digitalWrite(keyGnd[j], LOW);              
      }
      else {
        pinMode(keyGnd[j], INPUT);  //disable current, see NOTE above
      }
    }
    //небольшая пауза, чтобы пины настроились, говорят, что достаточно 2 мкс, но делаем больше
    delayMicroseconds(10);
    //считываем значения клавиатуры

    //TODO can use ports for reading faster, see 05_ArduinoMegaPortTest
    for (byte i=0; i<keyReadN; i++) {
      byte key = keys - 1 - (i+keyReadN*k); //because of reverted 
      byte v = (digitalRead(keyRead[i]) == LOW)?1:0;     
      byte &state = key_state[key];
      //клавиша была отпущена или нажата
      if (state != v) {
        Serial.print("key "); Serial.print(key); Serial.print(": ");
        if (v) Serial.println("on");
        else Serial.println("off");
      }
      state = v;
    }
    
  }

  //тестовая выдача на печать
  //for (int i=0; i<keys; i++) {
  //  Serial.print(key_state[i]);
  //}
  //Serial.println();
}

//---------------------------------------------------------------
void loop() {
  //обновление состояния клавиш
  keys_update();
  
  delay(10);

}

//---------------------------------------------------------------
