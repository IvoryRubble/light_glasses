#pragma once
#include "mapf.h"

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
