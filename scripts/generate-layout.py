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
GROUND_TUBE_COUNT = 8

GROUND_TUBE_LENGTH = LED_GAP_METERS * GROUND_LED_COUNT

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

gbo = 3.658 # distance from middle of chandelier to ground
gso = 2.438 # short offset from center
glo = 4.267 # long offset from center

ground_offsets = [
    [gso, -gbo, glo],
    [glo, -gbo, gso],
    [glo, -gbo, -gso],
    [gso, -gbo, -glo],
    [-gso, -gbo, -glo],
    [-glo, -gbo, -gso],
    [-glo, -gbo, gso],
    [-gso, -gbo, glo]
]

for t in range(GROUND_TUBE_COUNT):
    for p in range(GROUND_LED_COUNT):
        off = ground_offsets[t]
        x = off[0]
        y = off[1]
        z = off[2]
        if (t in [0, 3, 4, 7]):
            z = z + (p - GROUND_LED_COUNT / 2) * LED_GAP_METERS
        else:
            x = x + (p - GROUND_LED_COUNT / 2) * LED_GAP_METERS

        result.append('  {"point": [%.4f, %.4f, %.4f]},' % (x, y, z))

# Remove last comma
result[-1] = result[-1][:-1]

result.append(']')
print('\n'.join(result))
