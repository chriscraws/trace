#!/usr/bin/env python

import math
import optparse
import sys

parser = optparse.OptionParser(description='''
Outputs the layout for the trace burning man project in json format.

Units are expressed in meters with the origin at the center of the cube.''')

num_pixels = 2048
result = ['[']
tube_gap_meters = 0.2667
led_gap_meters = 0.035

for xi in range(8):
	for zi in range(8):
		for yi in range(64):
			x = (xi - 4) * tube_gap_meters
			z = (zi - 4) * tube_gap_meters
			y = (yi - 32) * led_gap_meters
			result.append('  {"point": [%.4f, %.4f, %.4f]},' % (x, z, y))

# Remove last comma
result[-1] = result[-1][:-1]

result.append(']')
print('\n'.join(result))
