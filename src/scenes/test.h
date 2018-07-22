#pragma once

#include <math.h>
#include "../fadecandy/effect.h"

namespace Pattern {

class Test : public Effect
{
public:
    Test()
        : time (0) {}

    float time;

    virtual void beginFrame(const FrameInfo &f)
    {
      time += f.timeDelta; 
    }

    virtual void shader(Vec3& rgb, const PixelInfo &p) const
    {
      float brightness = 0.5 + 0.5 * sinf(time * 2.0);
      rgb[0] = brightness;
      rgb[1] = brightness;
      rgb[2] = brightness;
    }
};

}
