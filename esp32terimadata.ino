#include <WiFi.h>
#include <WiFiClient.h>
#include <AsyncTCP.h>

#define ledPin 2
int id1 = 128414914200000001402111011710911212146991111141014610911710811610597114114971211481401295114101991111101151161141179911614814714814051101171091121211481407110100971141149712114814714875013314867198148135148821484075175175113414810431405100116121112101148147148140210552148137136135148821484075314016014878787874255255255255742552552552557501161489813767410001481161489846;

const char *ssid = "ARMTECH";     // Replace with your WiFi SSID
const char *password = "inverse6"; // Replace with your WiFi password
const int serverPort = 12345;    // Port to listen on

WiFiServer server(serverPort);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("Ipaddress: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  client = server.available();

  if (client) {
    Serial.println("New client connected");

    while (client.connected()) {
      // Check if there is data available
      if (client.available()) {
        // Read the message size
        uint32_t messageSize;
        size_t size = client.readBytes((uint8_t*)&messageSize, sizeof(messageSize));

        if (size == sizeof(messageSize)) {
          // Read the actual data
          uint8_t* data = (uint8_t*)malloc(messageSize);
          size_t bytesRead = client.readBytes(data, messageSize);

          if (bytesRead == messageSize) {
            // Process the received data (assuming it's marker IDs)
            process_data(data, messageSize);
          }

          free(data);
        }
      }
    }

    client.stop();
    Serial.println("Client disconnected");
  }
}

void process_data(uint8_t* data, size_t size) {
  // Deserialize the data (marker IDs in this example)
  Serial.print("Received data: ");
  for (size_t i = 0; i < size; i++) {
    Serial.print(data[i]);
    // Tambahkan spasi jika bukan elemen terakhir
    if (i < size - 1) {
      Serial.print("");
    }
  }
  Serial.println();
  // Add your data processing logic here
  if (size > 0 && data[0] == id1) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED turned ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LED turned OFF");
  }
}
