#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int dotPin = 13;
const int dashPin = 9;
const int buttonPin = 7;
const int resetButtonPin = 6;

const int dotTime = 200;
const int longPressThreshold = 500;
const int interLetterDelay = 1000;

String inputText = "";
String morseInput = "";
String decodedMessage = "";
bool newMessage = false;
unsigned long lastPressTime = 0;
bool buttonPressed = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Ready...");

  pinMode(dotPin, OUTPUT);
  pinMode(dashPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);
}

void loop() {
  handleSerialInput();
  handleButtonInput();
  handleResetButton();
}

void handleSerialInput() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      newMessage = true;
    } else {
      inputText += c;
    }
  }

  if (newMessage) {
    if (inputText == "!RESET") {
      morseInput = "";
      decodedMessage = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Input cleared");
      delay(500);
      lcd.clear();
      lcd.print("Ready...");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(inputText);
      playMorse(inputText);
    }
    inputText = "";
    newMessage = false;
  }
}

void handleButtonInput() {
  if (digitalRead(buttonPin) == LOW) {
    if (!buttonPressed) {
      buttonPressed = true;
      lastPressTime = millis();
    }
  } else if (buttonPressed) {
    buttonPressed = false;
    unsigned long pressDuration = millis() - lastPressTime;

    if (pressDuration < longPressThreshold) {
      morseInput += '.';
      flashLED(dotPin, dotTime);
    } else {
      morseInput += '-';
      flashLED(dashPin, dotTime * 3);
    }

    lastPressTime = millis();
  }

  if (!buttonPressed && morseInput != "" && millis() - lastPressTime > interLetterDelay) {
    char decoded = decodeMorse(morseInput);
    if (decoded != '?') {
      decodedMessage += decoded;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Morse: " + morseInput);

    String trimmedAlpha = decodedMessage;
    if (trimmedAlpha.length() > 10) {
      trimmedAlpha = trimmedAlpha.substring(trimmedAlpha.length() - 10);
    }

    lcd.setCursor(0, 1);
    lcd.print("Alpha: " + trimmedAlpha);

    morseInput = "";
  }
}

void handleResetButton() {
  if (digitalRead(resetButtonPin) == LOW) {
    morseInput = "";
    decodedMessage = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Input cleared");
    delay(500);
    lcd.clear();
    lcd.print("Ready...");
  }
}

void flashLED(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
  delay(dotTime);
}

void playMorse(String text) {
  text.toUpperCase();
  for (int i = 0; i < text.length(); i++) {
    char c = text.charAt(i);
    if (c == ' ') {
      delay(dotTime * 7);
      continue;
    }

    const char* morse = getMorse(c);
    if (morse != nullptr) {
      for (int j = 0; morse[j] != '\0'; j++) {
        if (morse[j] == '.') {
          flashLED(dotPin, dotTime);
        } else if (morse[j] == '-') {
          flashLED(dashPin, dotTime * 3);
        }
      }
      delay(dotTime * 3);
    }
  }
}

const char* getMorse(char c) {
  switch (c) {
    case 'A': return ".-"; case 'B': return "-..."; case 'C': return "-.-.";
    case 'D': return "-.."; case 'E': return "."; case 'F': return "..-.";
    case 'G': return "--."; case 'H': return "...."; case 'I': return "..";
    case 'J': return ".---"; case 'K': return "-.-"; case 'L': return ".-..";
    case 'M': return "--"; case 'N': return "-."; case 'O': return "---";
    case 'P': return ".--."; case 'Q': return "--.-"; case 'R': return ".-.";
    case 'S': return "..."; case 'T': return "-"; case 'U': return "..-";
    case 'V': return "...-"; case 'W': return ".--"; case 'X': return "-..-";
    case 'Y': return "-.--"; case 'Z': return "--..";
    default: return nullptr;
  }
}

char decodeMorse(String morse) {
  String codes[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--.."
  };
  for (int i = 0; i < 26; i++) {
    if (morse == codes[i]) return 'A' + i;
  }
  return '?';
}


