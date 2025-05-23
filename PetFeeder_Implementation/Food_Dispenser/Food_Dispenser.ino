#define BLYNK_TEMPLATE_ID "TMPL4m_Cq21Yw"
#define BLYNK_TEMPLATE_NAME "Food Dispenser"
#define BLYNK_AUTH_TOKEN "Hnj6uO9huswASQZDjEXZO_jSycmhX71a"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "HX711.h"
#include <ESP32Servo.h>

char ssid[] = "MartinRouterKing";
char pass[] = "girlboss2002";

BlynkTimer timer;

#define LOADCELL_DT_PIN  19
#define LOADCELL_SCK_PIN  4
#define SERVO_PIN 13
#define LED_PIN 18
#define BUZZER_PIN 23

HX711 scale;
float calibration_factor = -124800;

Servo myServo;
bool servoOpened = false;

unsigned long startTime = 0;
float weight = 0.00;
float oldWeight = 0.00;

BLYNK_WRITE(V1)
{
  int value = param.asInt();
  if (value == 1) {
    if (!servoOpened) {
      // Se deschide containerul pentru a cadea mancare
      // Se va aprinde led-ul de pe placa, dar si din aplicatie
      myServo.write(180);
      Blynk.virtualWrite(V2, 1);
      digitalWrite(LED_PIN, HIGH); 
      // Se va aprinde buzzer-ul
      analogWrite(BUZZER_PIN, 128);
      startTime = millis(); 
      servoOpened = true;
    }
  }
}

BLYNK_CONNECTED()
{
  Serial.println("Connected");
}

void myTimerEvent() {
  Blynk.virtualWrite(V0, weight);
}

void setup()
{
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.begin(LOADCELL_DT_PIN, LOADCELL_SCK_PIN);
  // Seteaza valoarea SCALE folosita pentru a converti datele brute in date usor de citit (in unitati de masura)
  scale.set_scale();
  // Ajusteaza punctul intern de zero al scalei astfel incat sa afiseze zero atunci cand nu exista nicio greutate pe celula de incarcare
  scale.tare();

  long zero_factor = scale.read_average();
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);
  // Aceasta functie citeste mai multe probe de la celula de incarcare si returneaza valoarea medie
  // Utilizata in timpul configurarii pentru a asigura citiri stabile
  scale.read_average();

  pinMode(SERVO_PIN, OUTPUT);
  myServo.attach(SERVO_PIN, 1000, 2000);
  myServo.write(0);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  Blynk.run();
  foodDispenser();
}

void foodDispenser() 
{
  scale.set_scale(calibration_factor); // Ajustarea la factorul de calibrare
  Serial.print("Reading: ");
  weight = scale.get_units() * 453.60; // Citirea greutatii in grame

  if (weight < 0) {
    weight = 0.00;
  }

  Serial.print(weight, 2); 
  Serial.print(" grams");
  Serial.print(" Calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  timer.setInterval(2000L, myTimerEvent);
  timer.run();

  // Daca containerul este deschis si s-au scurs 5 secunde de cand se adauga mancare, se inchide totul
  if (servoOpened && millis() - startTime > 5000 && weight > 150) {
    myServo.write(0);
    servoOpened = false;
    Blynk.virtualWrite(V2, 0);
    digitalWrite(LED_PIN, LOW);
    analogWrite(BUZZER_PIN, 0);
    Blynk.virtualWrite(V1, 0);
  } 
  
  // Daca containerul este inchis si greutatea hranei este mai mica de 150 grame, se adauga cantitatea constanta de 100 grame
  if (weight < 150) {
    myServo.write(180);
    Blynk.virtualWrite(V2, 1);
    digitalWrite(LED_PIN, HIGH);
    analogWrite(BUZZER_PIN, 128);
    oldWeight = weight;
    servoOpened = true;
    if (weight - oldWeight >= 100) {
      myServo.write(0);
      Blynk.virtualWrite(V2, 0);    
      digitalWrite(LED_PIN, LOW);
      analogWrite(BUZZER_PIN, 0); 
    }
  }
}
