#include <WiFi.h>
#include <AsyncMqttClient.h>

#define LED_PIN 4
#define WIFI_SSID "SUPER-ORCA"
#define WIFI_PASSWORD "zxcvbnmv"

// MQTT broker information
#define MQTT_HOST "broker.hivemq.com"
#define MQTT_PORT 1883
#define PUB_TOPIC "orca/log/topic"
#define SUB_TOPIC "orca/ctrl/topic"

AsyncMqttClient mqttClient;

void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("WiFi connected! Connecting to MQTT... ");
      mqttClient.connect();  // Connect to MQTT server
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi lost, reconnecting...");
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Reconnect to Wi-Fi  
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT!");
  mqttClient.subscribe(SUB_TOPIC, 1);  // Subscribe to MQTT topic
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Message received on topic: " + String(topic));

  // Received Message
  String message = "";
  for (size_t i = 0; i < len; i++) {
    message += (char)payload[i];
  }
  message.trim();
  message.toUpperCase();
  Serial.println("Message: "+ message);

  checkMessage(message);
}

void checkMessage(String msg) {
  if (msg == "ON") digitalWrite(LED_PIN, HIGH);
  else if (msg == "OFF") digitalWrite(LED_PIN, LOW);
  else return;

  String payload = "LED is "+ msg;
  mqttClient.publish(PUB_TOPIC, 1, false, payload.c_str());
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Setup MQTT Client
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
}

void loop() {
  // Kosong?
}