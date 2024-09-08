#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include "mapf.h"
#include "mathFuncs.h"
#include "buttonDebounce.h"
#include "effects.h"

const int ledPin = 6;
const int ledCount = 29;

Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRBW + NEO_KHZ800);

const int buttonsCount = 7;
const int buttonPins[buttonsCount] = {10, 16, 14, 15, A0, A1, A2};
ButtonDebounce buttonStates[buttonsCount];

const int resetButtonPin = 2;
const int resetButtonGndPin = 5;
ButtonDebounce resetButtonState;

struct StorageData {
  bool isEffectEnabled;
  int effectState;
  int hash;
  int calcHash() {
    return (int)(isEffectEnabled * 10) + (int)(effectState) + 12;
  }
  void updateHash() {
    hash = calcHash();
  }
  bool checkHash() {
    return hash == calcHash();
  }
};
struct StorageData storageData;
const int storageDataAddress = 49;

bool isEffectEnabled = true;
int effectState = 0;

void updateButtons();
void logButtons();
void putStorageData();
void getStorageData();

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < buttonsCount; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(resetButtonGndPin, OUTPUT);
  digitalWrite(resetButtonGndPin, LOW);

  getStorageData();

  strip.begin();
  strip.show();
  strip.setBrightness(255);
}

void loop() {
  updateButtons();
  logButtons();

  if (buttonStates[0].isBtnPressed || resetButtonState.isBtnReleasedLongPress) {
    strip.clear();
    strip.show();
    isEffectEnabled = !isEffectEnabled;
    putStorageData();
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

  if (buttonStates[6].isBtnPressed || (resetButtonState.isBtnReleased && !resetButtonState.isBtnReleasedLongPress)) {
    if (isEffectEnabled) {
      effectState = (effectState + 1) % 2;
    } else {
      isEffectEnabled = true;
    }
    putStorageData();
  }

  if (isEffectEnabled) {
    if (effectState == 0) {
      effectPlasma();
    }
    if (effectState == 1) {
      effectRainbow();
    }
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

void putStorageData() {
  storageData.effectState = effectState;
  storageData.isEffectEnabled = isEffectEnabled;
  storageData.updateHash();
  EEPROM.put(storageDataAddress, storageData);
}

void getStorageData() {
  EEPROM.get(storageDataAddress, storageData);
  if (storageData.checkHash()) {
    effectState = storageData.effectState;
    isEffectEnabled = storageData.isEffectEnabled;
  }
}
