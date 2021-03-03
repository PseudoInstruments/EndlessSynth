/* Microphone
  Most of work done inside sound module, but here we process mic button.  
 
  ----------------------------------------
  Programming details
  ----------------------------------------
  By default Arduino ADC works on 8Khz - ok for us.
  But can speedup:
  https://www.instructables.com/Make-Your-Own-Spy-Bug-Arduino-Voice-Recorder/
  Recorder:
  https://github.com/TMRh20/TMRpcm
  http://microsin.net/programming/avr/real-time-digital-audio-processing-using-arduino.html

*/

//mic button pins
const byte mic_button_pin1=8;
const byte mic_button_pin2=9;

//mic button state
byte mic_button=0;

//--------------------------------------------------------------
void mic_setup() {
 /* Serial.print("Program gets sound from microphone to A0, mic button pins: ");
  Serial.print(mic_button_pin1); Serial.print(","); Serial.println(mic_button_pin2);
  Serial.println("Note: please use trimmer resistor 10KOhm to move silence mic level to 2.5V (or, equally, 512 on A0)");


  //setup mic button pins
  pinMode(mic_button_pin1, OUTPUT); 
  pinMode(mic_button_pin2, INPUT_PULLUP); 
  digitalWrite(mic_button_pin1, LOW); 
*/
}

//--------------------------------------------------------------
void mic_loop() {
 /* mic_button = (digitalRead(mic_button_pin2) == LOW)?1:0;
  static byte mic_last = 0;
  if (mic_last != mic_button) {
    Serial.print("mic button: ");
    if (mic_button) {
      Serial.println("ON");
    }
    else {
      Serial.println("OFF");
    }
    mic_last = mic_button;
  }*/
}

//--------------------------------------------------------------
