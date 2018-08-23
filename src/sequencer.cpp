#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>

#include "sequencer.h"

Sequencer::Sequencer(
    float scene_time,
    float transition_time,
    std::vector<Scene>& scenes,
    std::vector<Transition>& transitions,
    Opc* opc,
    Gpu* gpu,
    int override_index
    ) :
  gpu(gpu),
  opc(opc),
  scenes(scenes),
  transitions(transitions),
  scene_time(scene_time),
  transition_time(transition_time),
  override_index(override_index) {
 
    if (override_index >= 0) {
      scene_index = override_index;
    } 
  }

void Sequencer::step(float time) {

  if (transitioning) {
    if (time - time_mark > transition_time) {
      scene_start = time_mark;
      time_mark = time;
      transitioning = false;
      scene_index = (scene_index + 1) % scenes.size();
      if (override_index >= 0) {
	scene_index = override_index;
      }
      transition_index = (transition_index + 1) % transitions.size();
    }
  } else {
    if (time - time_mark > scene_time) {
      time_mark = time;
      transitioning = true;
    }
  }

  if (!transitioning) {
    scenes[scene_index].draw(time - scene_start);
  } else {
    float mix = (time - time_mark) / transition_time;

    scenes[scene_index].draw(time - scene_start, 1);
    int next_index = override_index >= 0 ?
      override_index :
      (scene_index + 1) % scenes.size();
    scenes[next_index].draw(time - time_mark, 2);
    transitions[transition_index].draw(time - time_mark, mix);
  }

  gpu->read(opc->get_data_pointer());
  opc->send();
}

void Sequencer::run() {
  float time = 0.0, delta = 0.0;
  struct timeval now;
  gettimeofday(&now, 0);
  struct timeval lastTime = now;
  float filteredTimeDelta;
  float minTimeDelta = 1.0 / 300.0;
  float currentDelay = 0.0;
  const float filterGain = 0.05;

  time_mark = time;

  while (true) {
    step(time);


    // calculate timing stuff
    gettimeofday(&now, 0);
    delta = (now.tv_sec - lastTime.tv_sec)
      + 1e-6 * (now.tv_usec - lastTime.tv_usec);
    lastTime = now;

    // Max timestep; jump ahead if we get too far behind.
    const float maxStep = 0.1;
    if (delta > maxStep) {
      delta = maxStep;
    }

    time += delta;

    // Low-pass filter for timeDelta, to estimate our frame rate
    filteredTimeDelta += (delta - filteredTimeDelta) * filterGain;

    // Negative feedback loop to adjust the delay until we hit a target frame rate.
    // This lets us hit the target rate smoothly, without a lot of jitter between frames.
    // If we calculated a new delay value on each frame, we'd easily end up alternating
    // between too-long and too-short frame delays.
    currentDelay += (minTimeDelta - delta) * filterGain;

    // Make sure filteredTimeDelta >= currentDelay. (The "busy time" estimate will be >= 0)
    filteredTimeDelta = std::max(filteredTimeDelta, currentDelay);

    // Add the extra delay, if we have one. This is how we throttle down the frame rate.
    if (currentDelay > 0) {
      usleep(currentDelay * 1e6);
    }
  }
}

