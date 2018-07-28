// The number of LEDS per strip
#define LEDS_PER_STRIP 64

// The number of strips, horzontally
#define STRIP_COUNT 6

// In meters, the distance between two LEDS, vertically
#define LED_GAP_METERS 0.035

// In meters, the horizontal distance between two strips
#define STRIP_GAP_METERS 0.2667

namespace TraceUtil {
  float height = LED_GAP_METERS * LEDS_PER_STRIP; 
  float halfheight = height * 0.5;
}
