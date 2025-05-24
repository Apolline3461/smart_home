#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// DHT11 setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define RAIN_PIN 3
#define RAIN_LED 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(9600);
    dht.begin();
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);

    pinMode(RAIN_PIN, INPUT_PULLUP);
    pinMode(RAIN_LED, OUTPUT);
    delay(2000);
}

void loop() {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    bool isRaining = digitalRead(RAIN_PIN) == HIGH;

Serial.println(digitalRead(RAIN_PIN));

    if (isnan(temp) || isnan(hum)) {
        Serial.println("Erreur lecture DHT!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Erreur capteur");
        delay(2000);
        return;
    }

   digitalWrite(RAIN_LED, isRaining ? HIGH : LOW);
    
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.setCursor(13, 0);
    lcd.print((char)223); // symbole ° degré
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print(" Hum: ");
    lcd.print(hum);
    lcd.setCursor(13, 1);
    lcd.print(" %");

    delay(2000); // Attente 2s avant prochaine mesure
}
