#pragma once

#include <math.h>

#include "../fadecandy/particle.h"

/**
 * Basic attempt at fire.
 */ 

namespace Pattern {

namespace {

}

class Fire : public Effect
{
public:
  Fire() {
  }

  virtual void beginFrame(const FrameInfo &f)
  {
  }

  virtual void shader(Vec3& rgb, const PixelInfo& p) const {
    rgb = Vec3(1, 1, 1);
  }
};

} // namespace pattern
