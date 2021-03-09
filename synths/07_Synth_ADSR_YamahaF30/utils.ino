//Utilities

//---------------------------------------------------------------
//abs function, Arduino's abs works not correct for me in some cases
int absi(int i) {
  return  (i >= 0)?i:-i;
}

int clampi(int i, int a, int b) {
  if (i < a) return a;
  if (i > b) return b;
  return i;
}

int mapi_clamp(int i, int a, int b, int A, int B) {
  return clampi(map(i, a, b, A, B), A, B);
}

//---------------------------------------------------------------
//print routines - use their for shortening code
void pr(const char *str) {
  Serial.print(str);
}
void prln(const char *str) {
  Serial.println(str);
}
void pr(int i) {
  Serial.print(i);
}
void prln(int i) {
  Serial.println(i);
}
void prln() {
  Serial.println();
}

//---------------------------------------------------------------
