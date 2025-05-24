#include <Arduino.h>

// 74HC595
#define DATA_PIN 9    // DS
#define LATCH_PIN 10   // ST_CP
#define CLOCK_PIN 11   // SH_CP

// RGB LED 1
#define RGB1_R 3
#define RGB1_G 4
#define RGB1_B 5

// RGB LED 2
#define RGB2_R 6
#define RGB2_G 7
#define RGB2_B 8

void setup() {
  Serial.begin(9600); // Initialisation du port série

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  Serial.println("Setup done.");
}

void loop() {
  // Allumer les 12 LEDs
  Serial.println("Allumage des 12 LEDs via 74HC595");
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00001111); // 2e 595: LED 8-11 (Q0-Q3)
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1er 595: LED 0-7
  digitalWrite(LATCH_PIN, HIGH);
  delay(1000);

  // Éteindre les 12 LEDs
  Serial.println("Extinction des 12 LEDs");
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0);
  digitalWrite(LATCH_PIN, HIGH);
  delay(1000);

  // RGB LED 1 ON (vert)
  analogWrite(RGB1_R, 0);
  analogWrite(RGB1_G, 255);
  analogWrite(RGB1_B, 0);
  delay(1000);
  analogWrite(RGB1_R, 0);
  analogWrite(RGB1_G, 0);
  analogWrite(RGB1_B, 0);

  // RGB LED 2 ON (Bleu)
  Serial.println("Allumage RGB2 en bleu");
  analogWrite(RGB2_R, 0);
  analogWrite(RGB2_G, 0);
  analogWrite(RGB2_B, 255);
  delay(1000);
  analogWrite(RGB2_R, 0);
  analogWrite(RGB2_G, 0);
  analogWrite(RGB2_B, 0);

  Serial.println("Cycle terminé.\n");
  delay(1000);
}
