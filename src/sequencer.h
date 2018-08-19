#pragma once

#include <vector>

#include "opc.h"
#include "scene.h"
#include "gpu.h"
#include "transition.h"

class Sequencer {
  Gpu* gpu;
  Opc* opc;
  std::vector<Scene>& scenes;
  std::vector<Transition>& transitions;
  float scene_time, transition_time;

  float time_mark = 0.0;
  float scene_start = 0.0;
  bool transitioning = false;

  int scene_index = 0;
  int transition_index = 0;

public:
  Sequencer(
    float scene_time,
    float transition_time,
    std::vector<Scene>& scenes,
    std::vector<Transition>& transitions,
    Opc* opc,
    Gpu* gpu
  );

  void run();
private:
  void step(float time);
};
