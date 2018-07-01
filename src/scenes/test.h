#pragma once

#include <math.h>
#include "../fadecandy/effect.h"

namespace Pattern {

namespace {

const float cubeHeight = 0.035 * 64;
const float cubeLength = 0.2667 * 8;

}

class Test : public Effect
{
public:
    Test()
        : cycle (0) {}

    float cycle;

    virtual void beginFrame(const FrameInfo &f)
    {
        const float speed = 0.25;
        cycle = fmodf(cycle + f.timeDelta * speed, 1.0);
    }

    virtual void shader(Vec3& rgb, const PixelInfo &p) const
    {
        if (cycle < 0.5) {
            rainbow(rgb, p);
        } else {
          edges(rgb, p);
        }
    }

    void rainbow(Vec3& rgb, const PixelInfo &p) const {
	rgb[0] = p.point[0]  / (cubeLength / 2.0) + 0.5;
	rgb[1] = p.point[1]  / (cubeHeight / 2.0) + 0.5;
	rgb[2] = p.point[2]  / (cubeLength / 2.0) + 0.5;
    }

    void edges(Vec3& rgb, const PixelInfo &p) const {
        if (
            // four vertical edges
            p.index < 64 ||
            (p.index >= 5 * 64 && p.index < 6 * 64) ||
            (p.index >= 30 * 64 && p.index < 31 * 64) ||
            (p.index >= 35 * 64 && p.index < 36 * 64) ||
            
            // top and bottom edges
            (
                (
                    p.index % 64 == 0 ||
                    p.index % 64 == 63
                ) && (
                    p.index < 6 * 64 ||
                    p.index >= 35 * 64 ||
                    (p.index / 64) % 6 == 0.0 ||
                    (p.index / 64) % 6 == 5.0 ||
                    ((p.index / 64) / 6) % 6 == 0.0 ||
                    ((p.index / 64) / 6) % 6 == 5.0
                )
            )
        ) {
            rgb = Vec3(1.0, 1.0, 1.0);
        } else {
            rgb = Vec3(0.0, 0.0, 0.0);
        }
    }
};

}
