#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Servo.h>

// temperature and humidity sensor information
#define DHT_PIN 2
#define MEASUREMENT_INTERVAL 2000

DHT dht(DHT_PIN, DHT11);

// rain sensor information
#define RAIN_PIN 3
#define RAIN_LED 4

// window information
#define OPEN_WIN_POS 90
#define CLOSE_WIN_POS 0
#define SERVO_PIN 5
Servo windowServo;
bool winIsOpen = false;
bool night = false;
bool lastNight = false;

// Display module information
LiquidCrystal_I2C lcd(0x27, 16, 2);

void receiveEvent(int byteRcv) {
    if (byteRcv >= 1) {
        byte value = Wire.read();
        night = (value == 1);
    }
}

void manageWindow(int degree) {
    windowServo.attach(SERVO_PIN);
    windowServo.write(degree);
    delay(800);
    windowServo.detach();
}

void displayMeteoInformation(float &temp, float &hum) {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.setCursor(13, 0);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print(" Hum: ");
    lcd.print(hum);
    lcd.setCursor(13, 1);
    lcd.print(" %");
}

void setup() {
    Serial.begin(9600);
    dht.begin();
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);

    pinMode(RAIN_PIN, INPUT_PULLUP);
    pinMode(RAIN_LED, OUTPUT);

    manageWindow(CLOSE_WIN_POS);
    Wire.begin(0x08);
    Wire.onReceive(receiveEvent);
    delay(2000);
}

void loop() {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    bool isRaining = digitalRead(RAIN_PIN) == HIGH;

    if (isnan(temp) || isnan(hum)) {
        static bool dhtErrorShown = false;
        if (!dhtErrorShown) {
            Serial.println("ERROR reading DHT!");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Sensor error");
            dhtErrorShown = true;
        }
        delay(MEASUREMENT_INTERVAL);
        return;
    }
    displayMeteoInformation(temp, hum);
    if (isRaining) {
        digitalWrite(RAIN_LED, HIGH);
        if(winIsOpen) {
            manageWindow(CLOSE_WIN_POS);
            winIsOpen = false;
        }
    } else {
        digitalWrite(RAIN_LED, LOW);
        if (!winIsOpen && !night) {
            manageWindow(OPEN_WIN_POS);
            winIsOpen = true;
        }
    }

    if (night != lastNight) {
        // Close the window if the night has come
        if (night && winIsOpen) {
            manageWindow(CLOSE_WIN_POS);
            winIsOpen = false;
        }
        lastNight = night;
    }
    delay(MEASUREMENT_INTERVAL);
}
