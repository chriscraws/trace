#pragma once

#include <math.h>

#include "../fadecandy/particle.h"

/**
 * Basic attempt at fire.
 *  - makes a bunch of particles
 *  - particles are big at base
 *  - move up at constant rate
 * TODO:
 *  - better colors
 *  - control intensity
 *  - use gradient to place particles?
 *  - fewer embers
 *  - fire should extend farther
 */ 

namespace Pattern {

namespace {

const float base = 32.0 * 0.035;
const float width = 0.2667 * 3;
const int NUM_PARTICLES = 50;
const float V_OFFSET = 0.2;

float getRand() {
    return (float) rand() / RAND_MAX;
}

}

class Fire : public ParticleEffect
{
public:
    Fire() : cycle (0) {
	appearance.resize(NUM_PARTICLES);

	for (int i = 0; i < NUM_PARTICLES; i++) {
	    ParticleAppearance* p = &appearance[i];
	    p->point = Vec3(getRand(), getRand(),
		getRand() * (1.0 + V_OFFSET) - V_OFFSET);
	}
    }

    float cycle;

    virtual void beginFrame(const FrameInfo &f)
    {
	for (int i = 0; i < NUM_PARTICLES; i++) {
	    ParticleAppearance* p = &appearance[i];
	    p->point[2] += 0.005;

	    if (p->point[2] > base) {
		p->point[2] = -V_OFFSET;
		p->point[0] = getRand();
		p->point[1] = getRand();
	    }

	    p->color[0] = 1.0;
	    p->radius = pow(1.0 - p->point[2] + V_OFFSET, 3.0) * V_OFFSET;

	    p->intensity = 1.0; //getRand() / 2.0 + 0.5;	
	}

	buildIndex();
    }

    virtual void shader(Vec3& rgb, const PixelInfo& p) const {
	Vec3 pp = p.point;
	pp[2] += base;
	pp[2] /= 2.0;
	pp[2] /= base;

	pp[0] += width;
	pp[0] /= 2.0;
	pp[0] /= width;
	pp[1] += width;
	pp[1] /= 2.0;
	pp[1] /= width;

	rgb = sampleColor(pp);
    }
};

} // namespace pattern
