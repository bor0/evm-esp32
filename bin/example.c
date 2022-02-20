#include <analogWrite.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  analogWrite(LED_BUILTIN, 255);
  delay(1000);
  analogWrite(LED_BUILTIN, 0);
  delay(1000);
}
