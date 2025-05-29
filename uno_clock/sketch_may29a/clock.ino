#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 colonnes, 2 lignes

int heures = 00;
int minutes = 00;
unsigned long previousMillis = 0;
const unsigned long interval = 25;

void setup() {

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);

  lcd.init();              // Initialisation de l'écran LCD
  lcd.backlight();         // Allume le rétroéclairage
  lcd.setCursor(0, 0);     // Colonne 0, ligne 0
  lcd.print("Horloge"); // Affiche le texte
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Avancer le temps simulé
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      heures++;
      if (heures >= 24) {
        heures = 0;
      }
    }
    lcd.setCursor(0, 1);
    lcd.print("Heure: ");
    if (heures < 10) lcd.print("0");
    lcd.print(heures);
    lcd.print(":");
    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print("  "); // Nettoie les anciens caractères

    if ((heures > 19) || (heures == 19 && minutes >= 30) || (heures < 7)) {
      digitalWrite(8, HIGH); // LED allumée entre 19h30 et 7h00
    } else {
      digitalWrite(8, LOW);  // LED éteinte entre 7h00 et 19h29
    }
  }
}
