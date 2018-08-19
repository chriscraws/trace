#pragma once

#include "gpu.h"

class Transition {
  int frag_id;
  Gpu* gpu;

public:
  Transition(Gpu* g, const char* filename);
  void draw(float time, float mix);
};
