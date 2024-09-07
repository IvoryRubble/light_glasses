#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "mapf.h"
#include "mathFuncs.h"

#ifndef LED_COUNT
#define LED_COUNT 0
#endif

extern Adafruit_NeoPixel strip;

const uint32_t period0 = 2300 * 2;
const uint32_t period1 = 7100 * 2;
const uint32_t period2 = (uint32_t)11000 * 6;
const uint32_t period3 = 3100 * 5;
const int brightness0 = 255;
const int brightness1 = 255;
const int brightness2 = 255;

uint16_t hueState = 0;
uint16_t hueCurrentStep = UINT16_MAX / (3 * LED_COUNT);
uint16_t hueStateStep = UINT16_MAX / 3;

void effectPlasma() {
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
      float pixelI0 = periodicFuncPow5(t0 + 1.7 * (float)(i * 2) / LED_COUNT);
      float pixelIInt0 = mapf(pixelI0, 0, 1, 0, brightness0); 
      float pixelI1 = periodicFuncPow6(t1 - 2.9 * (float)(i * 2) / LED_COUNT);
      float pixelIInt1 = mapf(pixelI1, 0, 1, 0, brightness1);
      float pixelI2 = periodicFuncPow1(t2) * 4 / 6 + periodicFuncPow1(t3 + 3 * (float)(i * 2) / LED_COUNT) * 2 / 6;
      float pixelIInt2 = mapf(pixelI2, 0, 1, 0, UINT16_MAX);
      int val = (int)(mapf(pixelIInt0 + pixelIInt1, 0, brightness0 + brightness1, 0, 255));
      uint16_t hue = (uint16_t)pixelIInt2;
      strip.setPixelColor(i, strip.ColorHSV(hue, 255, val));
    }
    strip.show();
}

void effectFlash() {
     strip.clear();
    strip.show();
    for (int i = LED_COUNT - 1; i >= 0; i--) {
      strip.setPixelColor(i, strip.Color(255, 255, 255, 255));
      strip.show();
      delay(3);
    }
    delay(330);
    for (int i = LED_COUNT - 1; i >= 0; i--) {
      strip.setPixelColor(i, strip.ColorHSV(0, 0, 0));
      strip.show();
      delay(3);
    }
    strip.clear();
    strip.show();
    delay(100);
}

void effectStripWhite() {
    int patternWidth = 4;
    for (int i = LED_COUNT + patternWidth - 1; i >= 0; i--) {
      strip.clear();
      for (int j = 0; j < patternWidth; j++) {
        if (i - j < LED_COUNT && i - j >= 0) {
          strip.setPixelColor(i - j, strip.Color(255, 255, 255, 255));
        }
      }
      strip.show();
      delay(20);
    }
    strip.clear();
    strip.show();
    delay(200);
}

void effectStripWhiteDouble() {
    int patternWidth = 3;
    int centerOffset = -3;
    for (int i = LED_COUNT + patternWidth - 1; i >= LED_COUNT / 2; i--) {
      strip.clear();
      for (int j = 0; j < patternWidth; j++) {
        int currentPix = i - j + centerOffset; 
        if (currentPix < LED_COUNT && currentPix >= LED_COUNT / 2 + centerOffset) {
          strip.setPixelColor(currentPix, strip.Color(255, 255, 255, 255));
        }
        int currentPixMirrored = LED_COUNT - 1 - (i - j) + centerOffset;
        if (currentPixMirrored < LED_COUNT / 2 + centerOffset && currentPixMirrored >= 0) {
          strip.setPixelColor(currentPixMirrored, strip.Color(255, 255, 255, 255));
        }
      }
      strip.show();
      delay(30);
    }
    strip.clear();
    strip.show();
    delay(200);
}

void effectFlashColor() {
    strip.clear();
    strip.show();
    for (int i = LED_COUNT - 1; i >= 0; i--) {
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hueState + hueCurrentStep * i, 255, 255)));
      strip.show();
      delay(10);
    }
    delay(100);
    for (int i = LED_COUNT - 1; i >= 0; i--) {
      strip.setPixelColor(i, strip.ColorHSV(0, 0, 0));
      strip.show();
      delay(10);
    }
    strip.clear();
    strip.show();
    delay(100);
    hueState += hueStateStep;
}

void effectStripColor() {
    int patternWidth = 4;
    for (int i = LED_COUNT + patternWidth - 1; i >= 0; i--) {
      strip.clear();
      for (int j = 0; j < patternWidth; j++) {
        if (i - j < LED_COUNT && i - j >= 0) {
          strip.setPixelColor(i - j, strip.gamma32(strip.ColorHSV(hueState + hueCurrentStep * i, 255, 255)));
        }
      }
      strip.show();
      delay(20);
    }
    strip.clear();
    strip.show();
    delay(200);
    hueState += hueStateStep;
}

void effectStripColorDouble() {
    int patternWidth = 3;
    int centerOffset = -3;
    for (int i = LED_COUNT + patternWidth - 1; i >= LED_COUNT / 2; i--) {
      strip.clear();
      for (int j = 0; j < patternWidth; j++) {
        int currentPix = i - j + centerOffset; 
        if (currentPix < LED_COUNT && currentPix >= LED_COUNT / 2 + centerOffset) {
          strip.setPixelColor(currentPix, strip.gamma32(strip.ColorHSV(hueState + hueCurrentStep * i, 255, 255)));
        }
        int currentPixMirrored = LED_COUNT - 1 - (i - j) + centerOffset;
        if (currentPixMirrored < LED_COUNT / 2 + centerOffset && currentPixMirrored >= 0) {
          strip.setPixelColor(currentPixMirrored, strip.gamma32(strip.ColorHSV(hueState + hueCurrentStep * i, 255, 255)));
        }
      }
      strip.show();
      delay(30);
    }
    strip.clear();
    strip.show();
    delay(100);
    hueState += hueStateStep;
}
