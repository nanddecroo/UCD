const int buttonPins[] = {2, 3, 4, 5, 6};
const char* buttonNames[] = {"Knop 1", "Knop 2", "Knop 3", "Knop 4", "Knop 5"};
bool buttonStates[] = {HIGH, HIGH, HIGH, HIGH, HIGH}; // Vorige toestand

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int i = 0; i < 5; i++) {
    int currentState = digitalRead(buttonPins[i]);
    if (currentState == LOW && buttonStates[i] == HIGH) {
      Serial.println(buttonNames[i]);
    }
    buttonStates[i] = currentState;
  }
  delay(50);
}
