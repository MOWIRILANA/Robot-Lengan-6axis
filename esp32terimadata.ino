#include <WiFi.h>
#define ledPin 2

// Konfigurasi WiFi
const char *ssid = "ARMTECH";
const char *password = "inverse6";

// Konfigurasi server
const int server_port = 12345;

WiFiServer server(server_port);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi");

  // Memulai server socket
  server.begin();
  Serial.println("Server socket dimulai");
}

void loop() {
  // Menerima koneksi dari klien
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Klien terhubung");

    while (client.connected()) {
      // Menerima data dari klien
      if (client.available()) {
        String data = client.readStringUntil('\n');
        Serial.println("Menerima pesan dari klien: " + data);

        // Mengonversi string menjadi integer
        int receivedValue = data.toInt();

        // Proses data di sini sesuai kebutuhan Anda
        if(receivedValue <=100){
          digitalWrite(ledPin, HIGH);
        }
        else{
          digitalWrite(ledPin, LOW);
        }
      }
    }

    // Menutup koneksi dengan klien
    Serial.println("Klien terputus");
    client.stop();
  }
}

void prosesdata(){

}
