#pragma once

// map() macro but for floats
#define mapf(val, in_min, in_max, out_min, out_max) \
  (float)(val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min