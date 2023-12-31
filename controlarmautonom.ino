
#include <WiFi.h>

#include<ESP32Servo.h>
#define SERVO1 4  //capit
#define SERVO2 5  //putar
#define SERVO3 18 //topaxis
#define SERVO4 27 //Middle axis
#define SERVO5 26 //B
#define SERVO6 25 //A

//deklarasi button
#define buttonKanan 23
#define buttonKiri 22
#define button 19

//setting start posisi
int sudut1 = 180;
int sudut2 = 35;
int sudut3 = 90;
int sudut4 = 180;
int sudut5 = 120;//servo B
int sudut6 = 60; //servo A
Servo servoMotor1;
Servo servoMotor2;
Servo servoMotor3;
Servo servoMotor4;
Servo servoMotor5;
Servo servoMotor6;

//Stepper Motor
#include <Stepper.h>
#define ledPin 2
const int stepPin = 33;
const int dirPin = 32;
const int microsteps = 10;

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, stepPin, dirPin);
float desiredRotations = 0; //posisi motor stepper

//konfigurasi wifi
const char* ssid = "ARMTECH"; 
const char* password = "inverse6"; 

// Konfigurasi server
const int server_port = 12345;
WiFiServer server(server_port);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(buttonKanan, INPUT_PULLUP);
  pinMode(buttonKiri, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(115200);

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
  }
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server Start");

  // /* Start AsyncWebServer */
  // server.begin();

// Aktifkan Servo
  servoMotor1.attach(SERVO1);
  servoMotor2.attach(SERVO2);
  servoMotor3.attach(SERVO3);
  servoMotor4.attach(SERVO4);
  servoMotor5.attach(SERVO5);
  servoMotor6.attach(SERVO6);

  //start position
  servoMotor1.write(180);
  servoMotor2.write(35);
  servoMotor3.write(90);
  servoMotor4.write(180);
  servoMotor5.write(120);
  servoMotor6.write(60);
  digitalWrite(ledPin, LOW);
  
}

void loop(){
  terimaData();

  byte buttonState = digitalRead(button);
  if (buttonState == LOW){
    Serial.println("kanan");
    ambilobjek1();
    // baseKanan();
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
  delay(100);
  
  byte buttonState2 = digitalRead(buttonKanan);
  if (buttonState2 == LOW){
    Serial.println("Kanan");
    baseKanan();
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }

  byte buttonState3 = digitalRead(buttonKiri);
  if (buttonState3 == LOW){
    Serial.println("Kiri");
    baseKiri();
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
}

void ambilobjek1(){
  //otomatis1
  // card1.attachCallback([&](int value){
  digitalWrite(ledPin, !digitalRead(ledPin));
  axisBawah(170,10,120,60);
  // servoMotor5.write(170);// sudut5 = 170;
  // servoMotor6.write(10);// sudut6 = 180-sudut5;
  delay(500);
  servoMotor4.write(155);// sudut4 = 180-25;
  delay(500);
  servoMotor1.write(44);// sudut1 = 180-136;
  delay(500);
  axisBawah(124,56,120,60);
  // servoMotor5.write(124);// sudut5 = 124;
  // servoMotor6.write(56);// sudut6 = 180-sudut5;
  delay(500);
  baseKiri();
  // baseKiri();
  delay(500);
  axisBawah(170,10,120,60);
  // servoMotor5.write(170);// sudut5 = 170;
  // servoMotor6.write(15);// sudut6 = 180-sudut5;
  delay(1000);
  servoMotor1.write(180);//buka
  delay(5000);
  resetPosisi();
}

void baseKanan(){
  float desiredRotations = 10; //posisi motor stepper 10=satu putaran atau 360derajat
  float posisi = desiredRotations/100;
  int stepsToMove = stepsPerRevolution * microsteps * posisi;
  int delay = 10;
  int stepDelay = delay*100;
  digitalWrite(dirPin, LOW);

  // Gerakkan motor sesuai jumlah langkah yang dihitung
  for (int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);  
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay); 
  }
 
}

void baseKiri(){
  float desiredRotations = 12; //posisi motor stepper
  float posisi = desiredRotations/100;
  int stepsToMove = stepsPerRevolution * microsteps * posisi;
  int delay = 10;
  int stepDelay = delay*100;
  digitalWrite(dirPin, HIGH);

  // Gerakkan motor sesuai jumlah langkah yang dihitung
  for (int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);  
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay); 
  }
 
}

void axisBawah(int targetPos5, int targetPos6, int delayBetweenSteps5, int delayBetweenSteps6) {
  int currentPos5 = servoMotor5.read();  // Sudut saat ini servo5
  int currentPos6 = servoMotor6.read();  // Sudut saat ini servo6

  int direction5 = (targetPos5 > currentPos5) ? 1 : -1;
  int direction6 = (targetPos6 > currentPos6) ? 1 : -1;

  int steps5 = abs(targetPos5 - currentPos5);
  int steps6 = abs(targetPos6 - currentPos6);

  int stepSize5 = direction5;
  int stepSize6 = direction6;

  for (int i = 0; i < max(steps5, steps6); i++) {
    if (i < steps5) {
      currentPos5 += stepSize5;
      servoMotor5.write(currentPos5);
    }

    if (i < steps6) {
      currentPos6 += stepSize6;
      servoMotor6.write(currentPos6);
    }

    delay(max(delayBetweenSteps5, delayBetweenSteps6));
  }

  delay(500);
}

void terimaData(){
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
          ambilobjek1();
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

void resetPosisi(){
  servoMotor1.write(180);
  servoMotor2.write(35);
  servoMotor3.write(90);
  servoMotor4.write(180);
  servoMotor5.write(120);
  servoMotor6.write(60);
}
