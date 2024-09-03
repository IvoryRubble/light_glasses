#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "mapf.h"
#include "mathFuncs.h"
#include "buttonDebounce.h"

#define LED_PIN 6
#define LED_COUNT 29

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

const int buttonsCount = 7;
const int buttonPins[buttonsCount] = {10, 16, 14, 15, A0, A1, A2};
ButtonDebounce buttonStates[buttonsCount];

const int resetButtonPin = 2;
const int resetButtonGndPin = 5;
ButtonDebounce resetButtonState;

const uint32_t period0 = 2000;
const uint32_t period1 = 7000 * 3;
const uint32_t period2 = 10000 * 3;
const uint32_t period3 = 3000 * 3;
const int brightness0 = 255;
const int brightness1 = 255;
const int brightness2 = 255;

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

void updateButtons() {
  for (int i = 0; i < buttonsCount; i++) {
    buttonStates[i].updateState(!digitalRead(buttonPins[i]));
  }
  resetButtonState.updateState(digitalRead(resetButtonPin));
}

void loop() {
  updateButtons();
  for (int i = 0; i < buttonsCount; i++) {
    if (buttonStates[i].isBtnPressed) {
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" pressed");
    }
  }
  if (resetButtonState.isBtnPressed || buttonStates[1].isBtnPressed) {
    Serial.println("Reset button pressed");
    strip.fill(strip.Color(255, 255, 255, 255));
    strip.show();
    delay(200);
    strip.clear();
    strip.show();
    //delay(2000);
  }
  //return;

  uint32_t currentTime0 = millis() % period0;
  uint32_t currentTime1 = millis() % period1;
  uint32_t currentTime2 = millis() % period2;
  uint32_t currentTime3 = millis() % period3;

  float t0 = mapf(currentTime0, 0, period0, 1000, 1002);
  float t1 = mapf(currentTime1, 0, period1, 1000, 1002);
  float t2 = mapf(currentTime2, 0, period2, 1000, 1002);
  float t3 = mapf(currentTime3, 0, period3, 1000, 1002);

  strip.clear();
  for (int i = 0; i < LED_COUNT; i++) {
    float pixelI0 = periodicFuncPow6(t0 + 1.6 * (float)(i * 2) / LED_COUNT);
    float pixelIInt0 = mapf(pixelI0, 0, 1, 0, brightness0); 
    float pixelI1 = periodicFuncPow6(t1 - 4 * (float)(i * 2) / LED_COUNT);
    float pixelIInt1 = mapf(pixelI1, 0, 1, 0, brightness1);
    float pixelI2 = periodicFuncPow1(t2) * 4 / 6 + periodicFuncPow1(t3 + 3 * (float)(i * 2) / LED_COUNT) * 2 / 6;
    float pixelIInt2 = mapf(pixelI2, 0, 1, 0, UINT16_MAX);
    int val = (int)((pixelIInt0 + pixelIInt1) / 2);
    int hue = (int)pixelIInt2;
    strip.setPixelColor(i, strip.ColorHSV(hue, 255, val));
  }
  strip.show();
  delay(5);
}
