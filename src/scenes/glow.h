#pragma once

#include <math.h>

#include "../constants.h"
#include "../fadecandy/color.h"
#include "../fadecandy/noise.h"

namespace Pattern {

class Glow : public Effect
{
public:
  Glow() : t(0) {}

  float t;

  const float speed = 0.15;
  const float colorSpeed = 0.05;
  const float noiseScale = 5.0;
  const float noiseOffset = 10.0;

  virtual void beginFrame(const FrameInfo &f)
  {
    t += f.timeDelta;
  }
  
  virtual void shader(Vec3& rgb, const PixelInfo& p) const {
    float x = p.point[0] / TraceUtil::halfwidth + 0.5;
    float y = p.point[1] / TraceUtil::halfwidth + 0.5;
    float z = p.point[2] / TraceUtil::halfheight + 0.5;

    float theta = atan2(x - 0.5, y - 0.5) / 6.0;
    float floatdiff = sin(t * speed + y / 5.0 + x / 10.0);
    z += floatdiff * 0.3;
    theta = abs(theta) + floatdiff * M_PI / 3.0;
    theta = fmod(theta + t * colorSpeed, 2.0 * M_PI);

    float hue = theta / (2.0 * M_PI);
    float saturation = 0.8 + 0.2 * noise4(
        x * noiseScale,
        y * noiseScale,
        z * noiseScale,
        t * speed
    );
    saturation *= 0.5 + 0.5 * sin(t * speed);
    float value = noise4(
        noiseScale * (x + noiseOffset),
        noiseScale * (y + noiseOffset),
        noiseScale * (z + noiseOffset),
        t * speed
    );
    value = value * value;
        

    hsv2rgb(rgb, hue, saturation, value);
  }

};

}
