#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

bool isBusy = false;
#define OPEN 100
#define CLOSE 0

// entry door
#define OPENING_TIME_ENTRY 3000
    // RFID
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
    // Servo
Servo doorServo;
#define DOOR_PIN 4


// garage door
#define OPENING_TIME_GARAGE 5000
    //Servo
Servo garageServo;
#define GAR_PIN 3
    // HC-SR04
#define TRIG_PIN 6
#define ECHO_PIN 7

void setServo(Servo &servo, int pin, int angle, int waiting = 500) {
  servo.attach(pin);
  servo.write(angle);
  delay(waiting);
  servo.detach();
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("RFID Ready");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  setServo(doorServo, DOOR_PIN, CLOSE);
  delay(800); 
  setServo(garageServo, GAR_PIN, CLOSE);
}

void handleRFID() {
  isBusy = true;
  Serial.println("Badge detected");
  doorServo.attach(DOOR_PIN);
  doorServo.write(90);  // ouvrir
  delay(OPENING_TIME_ENTRY);
  doorServo.write(0); 
  delay(500); 
  doorServo.detach();
  rfid.PICC_HaltA();
  isBusy = false;
}

void handleProximity() {
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // Convertir en cm

  if (distance > 0 && distance < 5) {
    isBusy = true;
    garageServo.attach(GAR_PIN);
    garageServo.write(OPEN);
    delay(OPENING_TIME_GARAGE);
    garageServo.write(CLOSE);
    delay(500);
    garageServo.detach();
    isBusy = false;
  }
}

void loop() {
  if (!isBusy && rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
    handleRFID();

  if (!isBusy)
    handleProximity();

  delay(100);
}
