#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define AANTAL_LEDS 5  // 4 gekleurde + 1 oranje indicator

#define KNOP1 2
#define KNOP2 3
#define KNOP3 4
#define KNOP4 5
#define START_KNOP 7

Adafruit_NeoPixel leds(AANTAL_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int knopPinnen[4] = {KNOP1, KNOP2, KNOP3, KNOP4};
bool vorigeKnopStatus[4] = {true, true, true, true};
bool vorigeStartStatus = true;

const uint32_t KLEUREN[4] = {
  Adafruit_NeoPixel::Color(255, 0, 0),
  Adafruit_NeoPixel::Color(255, 255, 0),
  Adafruit_NeoPixel::Color(0, 255, 0),
  Adafruit_NeoPixel::Color(0, 0, 255)
};

const char* KLEUR_NAAM[4] = {"rood", "geel", "groen", "blauw"};

int kleurPerKnop[4] = {0, 1, 2, 3};
int doelVolgorde[4];
int spelerInput[4];
int invoerIndex = 0;

int volgordeIndex = 0;
bool klaarOmInTeVoeren = false;
bool wachtOpStart = false;

void setup() {
  Serial.begin(9600);
  leds.begin();
  leds.show();

  for (int i = 0; i < 4; i++) {
    pinMode(knopPinnen[i], INPUT_PULLUP);
  }
  pinMode(START_KNOP, INPUT_PULLUP);

  randomSeed(analogRead(0));
  nieuweRonde();
  zetIndicator(true);

  while (digitalRead(START_KNOP) == LOW);
  vorigeStartStatus = digitalRead(START_KNOP);
}

void loop() {
  bool huidigeStart = digitalRead(START_KNOP);

  if (huidigeStart == LOW && vorigeStartStatus == HIGH) {
    if (wachtOpStart) {
      Serial.println("start");
      nieuweRonde();
      wachtOpStart = false;
      zetIndicator(true);
    } else if (!klaarOmInTeVoeren && volgordeIndex < 4) {
      toonVolgendeKleur();
    }
  }
  vorigeStartStatus = huidigeStart;

  for (int i = 0; i < 4; i++) {
    bool knopNu = digitalRead(knopPinnen[i]);
    if (knopNu == LOW && vorigeKnopStatus[i] == HIGH && klaarOmInTeVoeren) {
      verwerkKnopdruk(i);
    }
    vorigeKnopStatus[i] = knopNu;
  }
}

void nieuweRonde() {
  int indices[4] = {0, 1, 2, 3};
  for (int i = 3; i > 0; i--) {
    int j = random(i + 1);
    int temp = indices[i];
    indices[i] = indices[j];
    indices[j] = temp;
  }
  for (int i = 0; i < 4; i++) {
    doelVolgorde[i] = indices[i];
  }

  for (int i = 3; i > 0; i--) {
    int j = random(i + 1);
    int temp = kleurPerKnop[i];
    kleurPerKnop[i] = kleurPerKnop[j];
    kleurPerKnop[j] = temp;
  }

  invoerIndex = 0;
  volgordeIndex = 0;
  klaarOmInTeVoeren = false;

  wisLeds();
  zetIndicator(true);
}

void toonVolgendeKleur() {
  int kleurIndex = doelVolgorde[volgordeIndex];
  Serial.println(KLEUR_NAAM[kleurIndex]);

  if (volgordeIndex == 3) {
    delay(600);
    Serial.println("einde_volgorde");

    for (int i = 0; i < 4; i++) {
      leds.setPixelColor(i, KLEUREN[kleurPerKnop[i]]);
    }
    leds.show();

    klaarOmInTeVoeren = true;
    zetIndicator(false);
  } else {
    zetIndicator(true);
  }

  volgordeIndex++;
}

void verwerkKnopdruk(int knopIndex) {
  if (invoerIndex >= 4) return;

  leds.setPixelColor(knopIndex, 0);
  leds.show();

  spelerInput[invoerIndex] = kleurPerKnop[knopIndex];
  invoerIndex++;

  if (invoerIndex == 4) {
    delay(300);
    controleerAntwoord();
  }
}

void controleerAntwoord() {
  bool juist = true;
  for (int i = 0; i < 4; i++) {
    if (spelerInput[i] == doelVolgorde[i]) {
      leds.setPixelColor(i, Adafruit_NeoPixel::Color(0, 255, 0));
    } else {
      leds.setPixelColor(i, Adafruit_NeoPixel::Color(255, 0, 0));
      juist = false;
    }
  }
  leds.show();

  if (juist) {
    Serial.println("succes");
  } else {
    Serial.println("fout");
  }

  delay(2000);
  wisLeds();
  wachtOpStart = true;
  klaarOmInTeVoeren = false;
  zetIndicator(true);
}

void wisLeds() {
  for (int i = 0; i < 4; i++) {
    leds.setPixelColor(i, 0);
  }
  leds.show();
}

void zetIndicator(bool aan) {
  if (aan) {
    leds.setPixelColor(4, Adafruit_NeoPixel::Color(255, 100, 0));
  } else {
    leds.setPixelColor(4, 0);
  }
  leds.show();
}