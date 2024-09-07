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

int effectState = 1;
int effectStatePrevious = effectState;

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

  if (resetButtonState.isBtnPressed || buttonStates[0].isBtnPressed) {
    strip.clear();
    strip.show();
    effectStatePrevious = effectState;
    effectState = 0;
  }
  
  if (effectState == -1) {
    // nothing
  }

  if (buttonStates[1].isBtnPressed) {
    effectFlash();
  }

  if (buttonStates[2].isBtnPressed) {
    effectStripWhite();
  }

  if (buttonStates[3].isBtnPressed) {
    effectStripWhiteDouble();
  }

  if (buttonStates[4].isBtnPressed) {
    effectFlashColor();
  }

  if (buttonStates[5].isBtnPressed) {
    effectStripColor();
  }

  // if (buttonStates[5].isBtnPressed) {
  //   effectStripColorDouble();
  // }

  if (buttonStates[6].isBtnPressed) {
    effectState = 1;
  }

  if (effectState == 1) {
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
