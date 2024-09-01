#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

// map() macro but for floats
#define mapf(val, in_min, in_max, out_min, out_max) \
  (float)(val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

#define LED_PIN 6
#define LED_COUNT 29

const uint32_t period0 = 2000;
const uint32_t period1 = 7000 * 3;
const uint32_t period2 = 10000 * 3;
const uint32_t period3 = 3000 * 3;
const int brightness0 = 255;
const int brightness1 = 255;
const int brightness2 = 255;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

float periodicFuncHSV(float);
float periodicFuncLinear(float, float, float);
float periodicFuncPow1(float);
float periodicFuncPow2(float);
float periodicFuncPow3(float);
float periodicFuncPow4(float);
float periodicFuncPow5(float);
float periodicFuncPow6(float);

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show();
  strip.setBrightness(255);
}

void loop() {
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
    float pixelI2 = periodicFuncHSV(t0 + 0.2 * (float)(i * 2) / LED_COUNT); //periodicFuncPow1(t2) * 4 / 6 + periodicFuncPow1(t3 + 3 * (float)(i * 2) / LED_COUNT) * 2 / 6;
    float pixelIInt2 = mapf(pixelI2, 0, 1, 0, UINT16_MAX);
    int val = (int)((pixelIInt0 + pixelIInt1) / 2);
    int hue = (int)pixelIInt2;
    strip.setPixelColor(i + 5, strip.ColorHSV(hue, 255, 255));
  }
  strip.show();
  delay(5);
}

float periodicFuncHSV(float input) {
  float x = input - ((int)input / 2 * 2);
  float y = x / 2;
  return y;
}

float periodicFuncLinear(float input, float bendX, float bendY) {
  float x = input - ((int)input / 2 * 2);
  if (x > 1) {
    x = 2 - x;
  }

  float y;
  if (x < bendX) {
    y = mapf(x, 0, bendX, 0, bendY);
  } else {
    y = mapf(x, bendX, 1, bendY, 1);
  }

  return y;
}

float periodicFuncPow1(float input) {
  float x = input - ((int)input / 2 * 2);
  float y;
  if (x < 1) {
    y = x;
  } else {
    y = 2 - x;
  }
  return y;
}

float periodicFuncPow2(float input) {
  float x = input - ((int)input / 2 * 2);
  float y;
  if (x < 1) {
    y = x * x;
  } else {
    y = (x - 2) * (x - 2);
  }
  return y;
}

float periodicFuncPow3(float input) {
  float x = input - ((int)input / 2 * 2);
  float y;
  if (x < 1) {
    y = x * x * x;
  } else {
    y = - (x - 2) * (x - 2) * (x - 2);
  }
  return y;
}

float periodicFuncPow4(float input) {
  float x = input - ((int)input / 2 * 2);
  float y;
  if (x < 1) {
    y = x * x * x * x;
  } else {
    y = (x - 2) * (x - 2) * (x - 2) * (x - 2);
  }
  return y;
}

float periodicFuncPow5(float input) {
  float x = input - ((int)input / 2 * 2);
  float y;
  if (x < 1) {
    y = x * x * x * x * x;
  } else {
    y = - (x - 2) * (x - 2) * (x - 2) * (x - 2) * (x - 2);
  }
  return y;
}

float periodicFuncPow6(float input) {
  float x = input - ((int)input / 2 * 2);
  float y;
  if (x < 1) {
    y = x * x * x * x * x * x;
  } else {
    y = (x - 2) * (x - 2) * (x - 2) * (x - 2) * (x - 2) * (x - 2);
  }
  return y;
}