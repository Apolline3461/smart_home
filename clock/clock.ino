#include <Wire.h>

int heures = 00;
int minutes = 00;
unsigned long previousMillis = 0;
const unsigned long interval = 25;

bool wasNight = false;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    pinMode(6, OUTPUT);
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        minutes++;
        if (minutes >= 60) {
          minutes = 0;
          heures++;
          if (heures >= 24) {
            heures = 0;
          }
        }
        

        bool isNight = (heures > 19) || (heures == 19 && minutes >= 30) || (heures < 7);

        if (isNight != wasNight) {
            digitalWrite(6, isNight ? HIGH : LOW);
          Serial.print("Heure simulée : ");
          if (heures < 10) Serial.print("0");
          Serial.print(heures);
          Serial.print(":");
          if (minutes < 10) Serial.print("0");
          Serial.println(minutes);
      
            wasNight = isNight; // mise à jour de l’état précédent

            byte valueToSend = isNight ? 1 : 0;

            Serial.print("Envoi I2C : ");
            Serial.println(isNight ? "NUIT" : "JOUR");

             Wire.beginTransmission(0x08);
             Wire.write(valueToSend);
             Wire.endTransmission();
            delay(5);
             Wire.beginTransmission(0x09);
             Wire.write(valueToSend);
             Wire.endTransmission();
        }

    }
}
