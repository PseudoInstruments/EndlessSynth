//EndlessSynth, Toy Piano Keyboard Reader Test

//Device: Arduino Mega
//Connection:
//- 12 pins from Toy Piano to Arduino Mega: 22,24,26,...,44
//Monitor port:
//- baud must be set to 500000!

//pins 0,1,2,3 - choosing keys block, 
//pins 4,5,6,7,8,9,10,11 - keys 0..7, 8..15, 16..23, 24..31 
//(keys numbers is reverted)


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
      pinMode(keyGnd[i], OUTPUT);
      digitalWrite(keyGnd[i], HIGH);  //выключено по-умолчанию
  }
  for (int i=0; i<keyReadN; i++) {
      pinMode(keyRead[i], INPUT_PULLUP);
  }
}

//---------------------------------------------------------------
void keys_update() {
  //Обновление состояния клавиатуры
  for (byte k=0; k<keyGndN; k++) {
    //выбираем область клавиатуры для считывания  
    for (byte j=0; j<keyGndN; j++) {
      digitalWrite(keyGnd[j], (j==k)?LOW:HIGH);      
    }
    //небольшая пауза, чтобы пины настроились, говорят, что достаточно 2 мкс, но делаем больше
    delayMicroseconds(10);
    //считываем значения клавиатуры
    for (byte i=0; i<keyReadN; i++) {
      byte key = i+keyReadN*k; 
      byte v = (digitalRead(keyRead[i]) == LOW)?1:0;      
      byte &state =key_state[key];
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
