#define LED_PIN 6
#define LED_COUNT 29

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "mapf.h"
#include "mathFuncs.h"
#include "buttonDebounce.h"
#include "effects.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

const int buttonsCount = 7;
const int buttonPins[buttonsCount] = {10, 16, 14, 15, A0, A1, A2};
ButtonDebounce buttonStates[buttonsCount];

const int resetButtonPin = 2;
const int resetButtonGndPin = 5;
ButtonDebounce resetButtonState;

int effectState = 6;
int effectStatePrevious = effectState;

int effectCount = 0;



void updateButtons();
void logButtons();

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < buttonsCount; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(resetButtonGndPin, OUTPUT);
  digitalWrite(resetButtonGndPin, LOW);

  strip.begin();
  strip.show();
  strip.setBrightness(255);
}

void loop() {
  updateButtons();
  logButtons();

  for (int i = buttonsCount - 1; i > 0; i--) {
    if (buttonStates[i].isBtnPressed) {
      effectStatePrevious = effectState;
      effectState = i;
      break;
    }
  }
  if (resetButtonState.isBtnPressed || buttonStates[0].isBtnPressed) {
    effectStatePrevious = effectState;
    effectState = 0;
  }
  
  if (effectState == -1) {
    // nothing
  }

  if (effectState == 0) {
    strip.clear();
    strip.show();
    effectState = -1;
  }

  if (effectState == 1) {
    effectFlash();
    effectState = effectStatePrevious;
  }

  if (effectState == 2) {
    effectStripWhite();
    effectState = effectStatePrevious;
  }

  if (effectState == 3) {
    effectStripWhiteDouble();
    effectState = effectStatePrevious;
  }

  if (effectState == 4) {
    effectFlashColor();
    effectState = effectStatePrevious;
  }

  if (effectState == 5) {
    effectStripColor();
    effectState = effectStatePrevious;
  }

  // if (effectState == 5) {
  //   effectStripColorDouble();
  //   effectState = effectStatePrevious;
  // }

  if (effectState == 6) {
    effectPlasma();
  }

  delay(1);
}

void updateButtons() {
  for (int i = 0; i < buttonsCount; i++) {
    buttonStates[i].updateState(!digitalRead(buttonPins[i]));
  }
  resetButtonState.updateState(digitalRead(resetButtonPin));
}

void logButtons() {
  for (int i = 0; i < buttonsCount; i++) {
    if (buttonStates[i].isBtnPressed) {
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" pressed");
    }
  }
  if (resetButtonState.isBtnPressed) {
    Serial.println("Reset button pressed");
  }
}
