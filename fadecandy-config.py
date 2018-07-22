#!/usr/bin/env python

# LEDS on first part of the strip
STRIP_CUTOFF = 32

# LEDS per strip
LEDS_PER_STRIP = 64

LISTEN_IP = "127.0.0.1"
LISTEN_PORT = "8000"
FADECANDY_SERIAL_NUMBERS = [
  "please fill this in",
  "with a bunch of serial numbers",
]

import json

config = {
  "listen": [LISTEN_IP, LISTEN_PORT],
  "color": {
    "gamma": 2.5,
    "whitepoint": [1.0, 1.0, 1.0]
  }
}

second_strip = LEDS_PER_STRIP - STRIP_CUTOFF
config["devices"] = []

for i in range(len(FADECANDY_SERIAL_NUMBERS)):
  device = {
    "type": "fadecandy",
    "serial": FADECANDY_SERIAL_NUMBERS[i],
    "led": False,
    "map": [
      [0, i * LEDS_PER_STRIP, 0, STRIP_CUTOFF],
      [0, i * LEDS_PER_STRIP + STRIP_CUTOFF, STRIP_CUTOFF, second_strip],
      [0, (i + 1) * LEDS_PER_STRIP, 0, STRIP_CUTOFF],
      [0, (i + 1) * LEDS_PER_STRIP + STRIP_CUTOFF, STRIP_CUTOFF, second_strip],
      [0, (i + 2) * LEDS_PER_STRIP, 0, STRIP_CUTOFF],
      [0, (i + 2) * LEDS_PER_STRIP + STRIP_CUTOFF, STRIP_CUTOFF, second_strip],
      [0, (i + 3) * LEDS_PER_STRIP, 0, STRIP_CUTOFF],
      [0, (i + 3) * LEDS_PER_STRIP + STRIP_CUTOFF, STRIP_CUTOFF, second_strip],
    ]
  }
  config["devices"].append(device)

print(json.dumps(config))
