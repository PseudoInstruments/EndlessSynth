//EndlessSynth, Yamaha PSS F30 Read Keyboard
//This sketch shows that keyboard is really polyphonic, but it doesn't use half of read pins

//Scheme: //http://sandsoftwaresound.net/wp-content/uploads/2019/12/Reface_key_schematic.jpg
//(But I have only 11 signal outputs, not 12)
//This is truly polyphonic keyboard, because contains diodes which stops conflict from several pressed keys.



//Device: Arduino Mega
//Monitor port:
//- baud must be set to 500000!

//Connection:
//7 - control pins - choosing keys block, 
//11 - signal pins, but it's uses only six - 1,3,5,7,9,11 - we connect only them


const byte keyGndN = 7;
const byte keyReadN = 6; 
byte keyGnd[keyGndN] = {52, 50, 48, 46, 44, 42, 40};
byte keyRead[keyReadN] = {32, 30, 28, 26, 24, 22};


const byte keys = 37;  //number of keys
byte key_state0[keys]; //old key state
byte key_state[keys]; //key state - pressed or released 
//TODO can store in a bit mask

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Yamaha PSS F30 Keyboard Reader 1.1");
  Serial.print("  control pins: ");
  for (int i=0; i<keyGndN; i++) {
    Serial.print(keyGnd[i]); Serial.print(" ");
  }
  Serial.print(",  signal pins: ");
  for (int i=0; i<keyReadN; i++) {
    Serial.print(keyRead[i]); Serial.print(" ");
  }
  Serial.println();
  Serial.println("Now press the keys!");

  //Set pins
  for (int i=0; i<keyGndN; i++) {
      pinMode(keyGnd[i], OUTPUT);
      digitalWrite(keyGnd[i], HIGH);  
  }
  for (int i=0; i<keyReadN; i++) {
      pinMode(keyRead[i], INPUT_PULLUP);
  }
}

//---------------------------------------------------------------
void loop() {

  for (byte k=0; k<keyGndN; k++) {
    //enable control pin
    digitalWrite(keyGnd[k], LOW);              
    delayMicroseconds(10);  //need wait 2 mcs, but let's wait for more...

    //Scan pins
    //TODO can use ports for reading faster, see 05_ArduinoMegaPortTest
    
    Serial.print(" ");
    for (byte i=0; i<keyReadN; i++) {
      byte v = (digitalRead(keyRead[i]) == LOW)?1:0;     
      if (v) Serial.print("X");
      else Serial.print(".");

      byte key = keys - 1 - (i+keyReadN*k); //because of reverted 
      key_state[key] = v;
    }

    //disable control pin
    digitalWrite(keyGnd[k], HIGH);              

  }
  //Serial.println();
  
  Serial.print("   ");
  //print keys events in the same line
  for (int i=0; i<keys; i++) {
      byte &state0 = key_state0[i];
      byte &state = key_state[i];
      if (state0 != state) {
        Serial.print("key_"); Serial.print(i); Serial.print("=");
        if (state) Serial.print("1 ");
        else Serial.print("0 ");
      }
      state0 = state;

  }  
  Serial.println();
 
  delay(100);
}
