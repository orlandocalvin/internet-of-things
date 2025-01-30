#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6fCBfC8Hf"
#define BLYNK_TEMPLATE_NAME "LED BUILTIN"
#define BLYNK_AUTH_TOKEN "UTuMEbfv1wLbbHPwiD_gRPuQElwvF36-"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char* ssid = "acer";
const char* password = "12345678";

void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  Blynk.run();
}