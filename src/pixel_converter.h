#pragma once
#include <cmath>

// The number of LEDS per strip
#define LEDS_PER_TUBE 67

// number of tubes in chandelier
#define TOP_TUBE_COUNT 36

// The number of strips, horzontally
#define STRIP_COUNT 6

// In meters, the distance between two LEDS, vertically
#define LED_GAP_METERS 0.035

// In meters, the horizontal distance between two strips
#define STRIP_GAP_METERS 0.2

#define GROUND_LED_COUNT 22
#define GROUND_TUBE_COUNT 8

namespace {
  const float gbo = 3.658f;
  const float gso = 2.438f;
  const float glo = 4.267f;

  const float ground_tube_offsets[] = {
    gso, -gbo, glo, // tube 0
    glo, -gbo, gso, // tube 1
    glo, -gbo, -gso, // tube 2
    gso, -gbo, -glo, // tube 3
    -gso, -gbo, -glo, // tube 4
    -glo, -gbo, -gso, // tube 5
    -glo, -gbo, gso, // tube 6
    -gso, -gbo, glo, // tube 7
  };

  const float height = ((LEDS_PER_TUBE - 1.0f) * LED_GAP_METERS);
  const float halfheight = height / 2.0f;

  float normalize_value(float value) {
    value /= halfheight;
    value /= 2.0f;
    value += 0.5f;
    return std::max(0.0f, std::min(1.0f, value));
  }

  float normalize_offset(float value) {
    value /= glo;
    value /= 2.0f;
    value += 0.5f;
    return std::max(0.0f, std::min(1.0f, value));
  }

  float get_offset(int index, float value) {
    int off = index % 3;
    if (off == 0 || off == 2) {
      if (value > gso) {
        return glo;
      } else if (value > height) {
        return gso;
      } else if (value < -gso) {
        return -glo;
      } else if (value < -height) {
        return -gso;
      }
    } else if (value < -height) {
        return -gbo;
    }
    return 0.0;
  }
}

namespace PixelConverter {

  void normalize_location(
    std::vector<float>& in_location,
    std::vector<float>& out_location,
    std::vector<float>& out_offset
  ) {
    for (uint32_t i = 0; i < in_location.size(); i++) {
      float in = in_location[i];
      float offset = get_offset(i, in);
      in -= offset;
      out_location[i] = normalize_value(in);  
      out_offset[i] = normalize_offset(offset);
    }
  }
}
