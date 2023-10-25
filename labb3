#include <LiquidCrystal.h>

// Skapar en instans av LiquidCrystal för att styra en LCD-skärm.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;
int itemIndex = -1; // Initialiserar med en ogiltig index
String items[] = { // Använd std::vector och std::string
  "Sword of Power",
  "Magic Potion",
  "Golden Helmet",
  "Enchanted Ring",
  "Diamond Amulet",
  "Huge boulder",
  "Extra helper"
};

const int btnPin = A5; // Definiera den analoga knappens stift
const int BuzzPin = 8; // Definiera det digitala högtalarstiftet

int toner[] = {262, 294, 330, 349};
int threshold = 10; // Justera detta tröskelvärde vid behov

// Setup-funktion som körs en gång vid start.
void setup() {
  // Starta LCD-skärmen med 16x2 tecken.
  lcd.begin(16, 2);

  // Konfigurera stift för switchPin som ingång.
  pinMode(switchPin, INPUT);

  // Konfigurera stift för btnPin som ingång.
  pinMode(btnPin, INPUT);

  // Starta seriell kommunikation med baud rate 9600.
  Serial.begin(9600);

  // Visa startmeddelande på LCD-skärmen.
  lcd.print("Secret Chest");
  lcd.setCursor(0, 1);
  lcd.print("Press button");
}

// Loop-funktion som körs upprepade gånger.
void loop() {
  // Läs switchPin:s aktuella tillstånd.
  switchState = digitalRead(switchPin);

  // Om switchPin:s tillstånd ändras
  if (switchState != prevSwitchState) {
    // Om knappen trycks ner (LOW)
    if (switchState == LOW) {
      // Generera ett slumpmässigt index inom det giltiga intervallet.
      itemIndex = random(7);
      Serial.println(itemIndex);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You found:");
      lcd.setCursor(0, 1);

      // Visa det slumpmässiga föremålet på LCD-skärmen.
      switch(itemIndex){
        case 0:
        lcd.print(items[0]);
        break;
        case 1:
        lcd.print(items[1]);
        break;
        case 2:
        lcd.print(items[2]);
        break;
        case 3:
        lcd.print(items[3]);
        break;
        case 4:
        lcd.print(items[4]);
        break;
        case 5:
        lcd.print(items[5]);
        break;
        case 6:
        lcd.print(items[6]);
        break;
      }
    }
  }

  // Spara switchPin:s tidigare tillstånd.
  prevSwitchState = switchState;

  // Läs av btnPin:s värde.
  int btnVal = analogRead(btnPin);

  // Spela toner beroende på knappens avlästa värde.
  if (btnVal > 1023 - threshold) {
    tone(BuzzPin, toner[0]);
  } 
  else if (btnVal >= 990 && btnVal <= 1010) {
    tone(BuzzPin, toner[1]);
  } 
  else if (btnVal >= 505 && btnVal <= 515) {
    tone(BuzzPin, toner[2]);
  } 
  else if (btnVal >= 5 && btnVal <= 10) {
    tone(BuzzPin, toner[3]);
  } 
  else {
    noTone(BuzzPin);
  }
  
  delay(450);
}
