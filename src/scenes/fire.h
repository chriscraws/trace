#pragma once

#include <math.h>

#include "../constants.h"
#include "../fadecandy/color.h"
#include "../fadecandy/noise.h"

/**
 * Basic attempt at fire.
 */ 

namespace Pattern {

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
    // squish space for simplex noise
    // (make larger blobs than default)
    float x = p.point[0];
    float y = p.point[1];
    float z = p.point[2];

    // make 0 the bottom and 1 the top
    z += TraceUtil::halfheight;
    float pos = z / TraceUtil::height + 0.01;

    // color positioning - used for red/yellow gradient
    float cpos = sqrt(pos);

    float speed = 2.0;

    // sample perlin noise to get flames
    float intensity = 0.8 * noise3(x, y, powf(pos, 3.0) - t * speed);
    intensity = powf(intensity, pos * pos * 10);

    // add a glowing base (not animated)
    float base = powf(1.0 - pos, 14.0);

    // set colors
    float hue = 0.1 * (1.0 - cpos);
    float saturation = 1.0;
    float value = intensity;

    // make the base look better
    if (pos < 0.5 && value < 0.25) { 
			float newbase = powf(1.0 - pos, 8.0);
			newbase = newbase * (3 + noise3(x, y, -t * speed)) / 3.0;
			value = fmax(newbase, value);
			saturation = 0.8 + 0.2 * (1.0 - powf(1.0 - pos, 32.0));
    } else {
			saturation = 1.0 - powf(1.0 - pos, 32.0);
		}

    hsv2rgb(rgb, hue, saturation, value);
  }
};

} // namespace pattern
