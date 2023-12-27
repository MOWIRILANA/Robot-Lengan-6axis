// #include <Arduino.h>
// #if defined(ESP8266)
//   /* ESP8266 Dependencies */
//   #include <ESP8266WiFi.h>
//   #include <ESPAsyncTCP.h>
//   #include <ESPAsyncWebServer.h>
// #elif defined(ESP32)
//   /* ESP32 Dependencies */
#include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #endif
// #include <ESPDash.h>

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

/* Your WiFi Credentials */
const char* ssid = "ARMTECH"; // SSID
const char* password = "inverse6"; // Password


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
  servoMotor5.write(170);// sudut5 = 170;
  servoMotor6.write(10);// sudut6 = 180-sudut5;
  delay(3000);
  servoMotor4.write(155);// sudut4 = 180-25;
  delay(1000);
  servoMotor1.write(44);// sudut1 = 180-136;
  delay(3000);
  servoMotor5.write(124);// sudut5 = 124;
  servoMotor6.write(56);// sudut6 = 180-sudut5;
  delay(3000);
  baseKiri();
  servoMotor5.write(170);// sudut5 = 176;
  servoMotor6.write(10);// sudut6 = 180-sudut5;
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
  float desiredRotations = 8; //posisi motor stepper
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