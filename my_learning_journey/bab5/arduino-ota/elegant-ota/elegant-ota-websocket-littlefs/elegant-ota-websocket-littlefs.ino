#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ElegantOTA.h>

// Replace with your network credentials
const char* ssid = "SUPER-ORCA";
const char* password = "zxcvbnmv";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Set LED GPIO
const int ledPin = 4;

// Stores LED state
String ledState;

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else {
    Serial.println("LittleFS mounted successfully");
  }
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Replaces placeholder with LED state value
String processor(const String& var) {
  if(var == "STATE") {
    if(digitalRead(ledPin)) {
      ledState = "ON";
    }
    else {
      ledState = "OFF";
    }
    return ledState;
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  initLittleFS();

  // Set GPIO4 as an OUTPUT
  pinMode(ledPin, OUTPUT);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html", false, processor);
  });

  server.serveStatic("/", LittleFS, "/");

  // Route to set GPIO state to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledPin, HIGH);
    request->send(LittleFS, "/index.html", "text/html", false, processor);
  });

  // Route to set GPIO state to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledPin, LOW);
    request->send(LittleFS, "/index.html", "text/html", false, processor);
  });

  // Start server
  server.begin();

  ElegantOTA.begin(&server);    // Start ElegantOTA

}

void loop() {
  ElegantOTA.loop();
}