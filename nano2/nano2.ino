#include <Arduino.h>
#include <Wire.h>

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
    Serial.begin(9600);

    Wire.begin(0x09);

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

void outdoorLedON() {
    Serial.println("Allumage des 12 LEDs via 74HC595");
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00001111); // 2e 595: LED 8-11 (Q0-Q3)
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1er 595: LED 0-7
    digitalWrite(LATCH_PIN, HIGH);
}

void outdoorLedOFF() {
    Serial.println("Extinction des 12 LEDs");
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0);
    digitalWrite(LATCH_PIN, HIGH);
}

void turnOnOffRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(rPin, r);
    analogWrite(gPin, g);
    analogWrite(bPin, b);
}

void loop() {
    outdoorLedON();

    Serial.println("allume bleu");
    turnOnOffRGB(RGB1_R, RGB1_G, RGB1_B, 0, 0, 255); // blue

    Serial.println("allume vert");
    turnOnOffRGB(RGB2_R, RGB2_G, RGB2_B, 0, 255, 0); // green

    delay(5000);

    outdoorLedOFF();
    turnOnOffRGB(RGB1_R, RGB1_G, RGB1_B, 0, 0, 0);
    turnOnOffRGB(RGB1_R, RGB1_G, RGB1_B, 0, 0, 0);
    Serial.println("Cycle terminé.\n");
}
