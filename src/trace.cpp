// Simple example effect:
// Draws a noise pattern modulated by an expanding sine wave.

#include <math.h>
#include "fadecandy/color.h"
#include "fadecandy/effect.h"
#include "fadecandy/effect_runner.h"
#include "fadecandy/noise.h"

#include "scenes/test.h"
#include "scenes/fire.h"

int main(int argc, char **argv)
{
    EffectRunner r;

    Pattern::Test test;
    Pattern::Fire fire;

    r.addEffect(&fire);

    // Defaults, overridable with command line options
    r.setMaxFrameRate(100);
    r.setLayout("../layouts/grid32x16z.json");

    return r.main(argc, argv);
}
