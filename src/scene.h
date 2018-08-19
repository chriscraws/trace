#pragma once

#include "gpu.h"

class Scene {
  int frag_id;
  Gpu* gpu;

public:
  Scene(Gpu* g, const char* filename);
  void draw(float time, int target = 0);
};
