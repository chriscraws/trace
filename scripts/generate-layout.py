#!/usr/bin/env python

# The number of LEDS per strip
LEDS_PER_STRIP = 67

# The number of strips, horzontally
STRIP_COUNT = 6

# In meters, the distance between two LEDS, vertically
LED_GAP_METERS = 0.035

# In meters, the horizontal distance between two strips
STRIP_GAP_METERS = 0.2

# Number of leds per ground tube
GROUND_LED_COUNT = 22

# Number of ground tubes
GROUND_TUBE_COUNT = 4


import math
import optparse
import sys

parser = optparse.OptionParser(description='''
Outputs the layout for the trace burning man project in json format.

Units are expressed in meters with the origin at the center of the cube.''')

result = ['[']

for zi in range(STRIP_COUNT):
	for xi in range(STRIP_COUNT):
		for yi in range(LEDS_PER_STRIP):
			x = (0.5 + xi - STRIP_COUNT / 2) * STRIP_GAP_METERS
			z = (0.5 + zi - STRIP_COUNT / 2) * STRIP_GAP_METERS
			y = -(0.5 + yi - LEDS_PER_STRIP / 2) * LED_GAP_METERS
			result.append('  {"point": [%.4f, %.4f, %.4f]},' % (x, y, z))

for t in range(GROUND_TUBE_COUNT):
  for p in range(GROUND_LED_COUNT):
    result.append('  {"point": [%.4f, %.4f, %.4f]},' % (50, 50, 50))

# Remove last comma
result[-1] = result[-1][:-1]

result.append(']')
print('\n'.join(result))
