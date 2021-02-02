//EndlessSynth Keyboard Pins tester
//Проверка пинов клавиатуры детского синтезатора "Агния Барто"
//Подключил 12 пинов с клавиатуры к Arduino Mega,
//22,24,26,...,44
/*
пины: 0 1 2 3 4 5 6 7 8 9 10 11
клавиши: 0..31
 
пин 0 и 4,5,6,7,8,9,10,11 - клавиши 0..7 
пин 1 и 4,5,6,7,8,9,10,11 - клавиши 8..15 
пин 2 и 4,5,6,7,8,9,10,11 - клавиши 16..23 
пин 3 и 4,5,6,7,8,9,10,11 - клавиши 24..31 
*/
//Вывод: клавиатуру можно опрашивать параллельно, то есть клавиши будут работать независимо.

const int N = 12;
int keyb[N] = {22,24,26,28,30,32,34,36,38,40,42,44};

int ground = 0; //индекс пина для земли - пытаемся обнаружить логику пинов, перебирая ее
             //1;
             //2;
             //3;

void setup() {
  //Печать приветствия
  Serial.begin(9600);
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
