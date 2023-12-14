#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "ARMTECH";
const char* password = "inverse6";
const int server_port = 12345;

int ledPin = 2;  // Gantilah dengan pin LED pada ESP32 Anda

WiFiServer server(server_port);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("ip address: ");
  Serial.println(WiFi.localIP());
  // Start the server
  server.begin();
  Serial.printf("Server listening on port %d\n", server_port);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");

    // Receive marker IDs from the client
    String marker_ids = client.readStringUntil('\n');
    marker_ids.trim();

    Serial.print("Received Marker IDs: ");
    Serial.println(marker_ids);

    // Check if marker ID is 1, then turn on LED
    if (marker_ids.toInt() == 1) {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED turned ON");
    } else {
      digitalWrite(ledPin, LOW);
      Serial.println("LED turned OFF");
    }

    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
  delay(100);
}
