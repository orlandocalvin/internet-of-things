#include <Arduino.h>

const int LED_BUILTIN = 2;

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello from ESP32!");

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}