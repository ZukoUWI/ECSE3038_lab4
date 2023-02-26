#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* server_address = "your_SERVER_ADDRESS";

// Define pin numbers for LEDs
int led1 = 2;
int led2 = 3;
int led3 = 4;

void setup() {
  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Create an HTTPClient object
    HTTPClient http;
    
    // Send GET request to server
    String url = server_address + "/api/state";
    http.begin(url);
    int http_code = http.GET();

    if (http_code > 0) {
      // If response is received
      String response = http.getString();
      Serial.println(response);
      
      // Parse JSON response
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, response);

      if (error) {
        Serial.println("Failed to parse JSON response");
      } else {
        // Extract LED states from JSON
        bool led1_state = doc["led1"];
        bool led2_state = doc["led2"];
        bool led3_state = doc["led3"];

        // Set LED states in circuit
        digitalWrite(led1, led1_state);
        digitalWrite(led2, led2_state);
        digitalWrite(led3, led3_state);
      }
    } else {
      Serial.println("Failed to connect to server");
    }

    http.end();
  }

  delay(5000); // Wait for 5 seconds before making next request
}
