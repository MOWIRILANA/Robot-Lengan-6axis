#include <Arduino.h>
#if defined(ESP8266)
  /* ESP8266 Dependencies */
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#elif defined(ESP32)
  /* ESP32 Dependencies */
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif
#include <ESPDash.h>

#include<ESP32Servo.h>
#define SERVO1 4  //capit
#define SERVO2 5  //putar
#define SERVO3 18 //topaxis
#define SERVO4 27 //Middle axis
#define SERVO5 26 //B
#define SERVO6 25 //A


int sudut1 = 0;
int sudut2 = 35;
int sudut3 = 0;
int sudut4 = 180;
int sudut5 = 120;//servo B
int sudut6 = 60; //servo A
Servo servoMotor1;
Servo servoMotor2;
Servo servoMotor3;
Servo servoMotor4;
Servo servoMotor5;
Servo servoMotor6;
/* Your WiFi Credentials */
const char* ssid = "ARMTECH"; // SSID
const char* password = "inverse6"; // Password

/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server); 

Card Slider1(&dashboard, SLIDER_CARD, "Capit", "", 0, 180);
Card Slider2(&dashboard, SLIDER_CARD, "Putar", "", 0, 180);
Card Slider3(&dashboard, SLIDER_CARD, "Top Axis", "", 0, 180);
Card Slider4(&dashboard, SLIDER_CARD, "Middle Axis", "", 0, 180);
Card Slider5(&dashboard, SLIDER_CARD, "Axis Bawah", "", 0, 180);

void setup() {
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

  /* Start AsyncWebServer */
  server.begin();

// Aktifkan Servo
  servoMotor1.attach(SERVO1);
  servoMotor2.attach(SERVO2);
  servoMotor3.attach(SERVO3);
  servoMotor4.attach(SERVO4);
  servoMotor5.attach(SERVO5);
  servoMotor6.attach(SERVO6);

  //start position
  servoMotor1.write(0);
  servoMotor2.write(35);
  servoMotor3.write(0);
  servoMotor4.write(0);
  servoMotor5.write(0);
  servoMotor6.write(180);
}

void loop(){

  espdash();

}

void ambilobjek1(){
  
}

void espdash(){
  //Servo 1
  Slider1.attachCallback([&](int value){
    sudut1 = 180-value;
    // Serial.println("sudutservo1: "+sudut1);
    Slider1.update(value);
    dashboard.sendUpdates();
  });
  servoMotor1.write(sudut1);

//Servo2
  Slider2.attachCallback([&](int value){
    sudut2 = value;
    Serial.println("sudutservo2: "+sudut2);
    Slider2.update(value);
    dashboard.sendUpdates();
  });
  servoMotor2.write(sudut2);


//Servo3
  Slider3.attachCallback([&](int value){
    sudut3 = value;
    Serial.println("sudutservo3: "+sudut3);
    Slider3.update(value);
    dashboard.sendUpdates();
  });
  servoMotor3.write(sudut3);

//Servo 4
  Slider4.attachCallback([&](int value){
    sudut4 = 180-value;
    // Serial.println("Servo4: "+String(value));
    Serial.println("sudutservo4: "+sudut4);
    /* Make sure we update our slider's value and send update to dashboard */
    Slider4.update(value);
    dashboard.sendUpdates();
  });
  servoMotor4.write(sudut4);

//Servo5
  Slider5.attachCallback([&](int value){
    sudut5 = value;
    sudut6 = 180-sudut5;
    /* Print our new slider value received from dashboard */
    Serial.println("Axis BawahB: "+String(value));
    Serial.print("sudut5:"+String(sudut5));
    Serial.print("||");
    Serial.println("sudut6:"+String(sudut6));
    /* Make sure we update our slider's value and send update to dashboard */
    Slider5.update(value);
    dashboard.sendUpdates();
  });
  servoMotor5.write(sudut5);
  servoMotor6.write(sudut6);

}