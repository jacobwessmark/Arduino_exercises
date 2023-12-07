#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C 
#define NUM_BUTTONS 4
#define ADRESS 17

// Skapar en instans av Adafruit_SSD1306 för att kontrollera OLED-skärmen.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definierar knapparna och tonerna som är kopplade till dem.
const int buttons[NUM_BUTTONS] = {2, 3, 4, 5};
const int buttonTones[NUM_BUTTONS] = {261, 293, 329, 349};
int oldButtonStates[NUM_BUTTONS] = {1, 1, 1, 1};
int inputCode[NUM_BUTTONS] = {0, 0, 0, 0};
int oldProgButtonState = 1;
int pressCount = 0;

// Definierar den hemliga koden och andra konstanter.
int SECRETCODE[NUM_BUTTONS] = {4, 2, 3, 1};
const int LOCKED = 90;
const int OPEN = -90;

// Definierar stift för olika komponenter.
int buzzPin = 6;
int prgPin = 7;
int greenLED = 8;
int redLED = 9;
int servoPin = 10;

// Skapar en instans av Servo för att styra servomotorn.
Servo myServo;
bool isLocked = true;
bool newCodeFlag = false;

// Funktion för att skriva en array med heltal till EEPROM.
void writeIntArrayIntoEEPROM(int address, int numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++) 
  {
    EEPROM.write(addressIndex, numbers[i] >> 8);
    EEPROM.write(addressIndex + 1, numbers[i] & 0xFF);
    addressIndex += 2;
  }
}

// Funktion för att läsa en array med heltal från EEPROM.
void readIntArrayFromEEPROM(int address, int numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    numbers[i] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
    addressIndex += 2;
  }
}

// Funktion för att öppna skattkistan och visa meddelanden på OLED-skärmen.
void openChest(int num) {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);

    if (num == 0) {
        display.println("Öppnar kistan......");
    } else if (num == 1) {
        display.println("Du hittade: ");
        display.println("Blodig svärd");
    }

    display.display();
}

// Funktion för att kontrollera om angiven kod är korrekt.
bool codeChecker(int code[NUM_BUTTONS]) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (code[i] != SECRETCODE[i]) {
            return false;
        }
    }
    Serial.println();
    isLocked = false;
    return true;
}

// Funktion för att kontrollera en knapp och uppdatera kodinmatningen.
void checkButton(int idx) {
    int val = digitalRead(buttons[idx]);
    // idx 4 är progButton
    if (val == 1 && oldButtonStates[idx] == 0) {
        // Sista knappen är vår progButton
        for (int i = 0; i < NUM_BUTTONS - 1; i++) {
            inputCode[i] = inputCode[i + 1];
        }
        inputCode[NUM_BUTTONS - 1] = idx + 1;
        tone(buzzPin, buttonTones[idx], 500);
        if (newCodeFlag == false){
          codeChecker(inputCode);
        }
        else {
          pressCount++; 
        }

    }
    // Kod för progButton

    oldButtonStates[idx] = val;
}

// Funktion för att kontrollera progButton.
void progCheckButton(int pin){
  int val = digitalRead(pin);
  if (val == 1 && oldProgButtonState == 0){
    newCodeFlag = true;
    Serial.println("Prog-knapp tryckt");
    while (pressCount < 4){
      for (int i = 0; i < NUM_BUTTONS; i++) {
        checkButton(i);
        delay(10);
      }
    }
    pressCount = 0;
    for (int i = 0; i < 4; i++){
      SECRETCODE[i] = inputCode[i];
      inputCode[i] = 0;
      Serial.print(SECRETCODE[i]);
    }
    Serial.println("");
    Serial.println("SECRETCODE uppdaterad");
    writeIntArrayIntoEEPROM(ADRESS, SECRETCODE, 4);
    newCodeFlag = false;
  }
   oldProgButtonState = val;
}

// Setup-funktionen som körs en gång vid start.
void setup() {
    // Starta seriell kommunikation med baud rate 9600.
    Serial.begin(9600);

    // Läs in den hemliga koden från EEPROM.
    readIntArrayFromEEPROM(ADRESS, SECRETCODE, 4);
    Serial.println(SECRETCODE[0]);

    // Koppla servomotorn till rätt stift.
    myServo.attach(servoPin);

    // Starta OLED-skärmen och rensa den.
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
    display.display();

    // Konfigurera knapparna som ingångar med pull-up resistorer.
    for (int i = 0; i < NUM_BUTTONS; i++) {
        pinMode(buttons[i], INPUT_PULLUP);
    }

    // Konfigurera stift för LED-lampor, servomotor och summer.
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(servoPin, OUTPUT);
    pinMode(prgPin, INPUT_PULLUP);
    pinMode(buzzPin, OUTPUT);
}

// Loop-funktionen som körs upprepade gånger.
void loop() {
    // Om skattkistan inte är låst
    if (!isLocked) {
        // Visa öppningsmeddelande på OLED-skärmen
        openChest(0);
        delay(500);

        // Slå på grönt LED-ljus och stäng av rött
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);
        myServo.write(OPEN);
        
        // Spela ljudsignal
        tone(buzzPin, 440, 150); delay(150);
        tone(buzzPin, 494, 150); delay(150);
        tone(buzzPin, 523, 150); delay(150);
        tone(buzzPin, 560, 650); delay(150);

        // Visa meddelande om innehåll på OLED-skärmen
        openChest(1);
        delay(5000);
        display.clearDisplay();
        display.display();

        // Stäng skattkistan och ändra LED-färg till rött
        myServo.write(LOCKED);
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        isLocked = true;
    } 

    // Om skattkistan är låst
    if (isLocked) {
        digitalWrite(redLED, HIGH);
    }

    // Kontrollera progButton
    progCheckButton(prgPin);

    // Kontrollera varje knapp
    for (int i = 0; i < NUM_BUTTONS; i++) {
        checkButton(i);
    }

    delay(100);
}
