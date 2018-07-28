#pragma once

#include <math.h>

#include "../constants.h"
#include "../fadecandy/color.h"
#include "../fadecandy/noise.h"

/**
 * Basic attempt at fire.
 */ 

namespace Pattern {

namespace {

}

class Fire : public Effect
{
public:
  Fire() : t(0) {}

  float t;
  float noiseValue;

  virtual void beginFrame(const FrameInfo &f)
  {
    t += f.timeDelta;
  }

  virtual void shader(Vec3& rgb, const PixelInfo& p) const {
    float x = p.point[0] * 0.8;
    float y = p.point[1] * 0.8;
    float z = p.point[2];

    z += TraceUtil::halfheight;
    
    float pos = z / TraceUtil::height + 0.01;
    float cpos = powf(pos, 0.5);

    float speed = 2.0;
    float intensity = 0.8 * noise3(x, y, powf(pos, 0.8) - t * speed);
    intensity = powf(intensity, powf(pos, 2.0) * 10);
    float base = powf(1.0 - pos, 14.0);

    float hue = 0.1 * (1.0 - cpos);

    float saturation = 1.0;
    float value = base > intensity ? base : intensity;
    if (value == base) { 
      hue = powf(0.98 - pos, 0.2);
      value *= 0.5;
    }

    hsv2rgb(rgb, hue, saturation, value);
  }
};

} // namespace pattern
