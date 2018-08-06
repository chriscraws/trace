#!/usr/bin/env python

# LEDS on first part of the strip
STRIP_TOP = 32

#LEDS on second part of the strip
STRIP_BOTTOM = 35

# LEDS per strip
LEDS_PER_TUBE = STRIP_TOP + STRIP_BOTTOM

TUBES_PER_ROW = 6

# Strips per fadecandy device
STRIPS_PER_FC = 8

LISTEN_IP = "127.0.0.1"
LISTEN_PORT = 7890
FADECANDY_SERIAL_NUMBERS = [
  "XHSJGYTBVPJWQOPV",
  "TBGRCCSGLJDQKMHL",
  "LHCDYEAYZDXOSUNJ",
  "ZNAPISNRVVRAIQZZ",
  "PJSTCENDVXLGCUPP",
  "XBOHYUEQTZPSYOZN",
  "CEXKDBHPZLFUUYFZ",
  "XRMNSARZNTNGYMLB",
  "JVQHWMWSTTDACSNR"
]

import json
import math

config = {
  "listen": [LISTEN_IP, LISTEN_PORT],
  "color": {
    "gamma": 2.5,
    "whitepoint": [1.0, 1.0, 1.0]
  }
}

config["devices"] = []

def get_map(fc_index, fc_strip_index):
    opc_channel = 0
    fadecandy_start_index = fc_strip_index * 64
    actual_strip_index = fc_index * STRIPS_PER_FC + fc_strip_index

    # flip odd rows since they're zig-zagged
    tube_index = actual_strip_index / 2
    row_index = tube_index / TUBES_PER_ROW
    if row_index % 2 == 1:
        row_offset = tube_index % TUBES_PER_ROW
        virtual_row_offset = (TUBES_PER_ROW - 1) - row_offset
        virtual_tube_index = row_index * TUBES_PER_ROW + virtual_row_offset
    else:
        virtual_tube_index = tube_index

    opc_start_index = virtual_tube_index * LEDS_PER_TUBE

    # Fixing our mistakes - flipped strips in the halo
    swap = actual_strip_index in [0, 1, 40, 41, 42, 43, 44, 45, 46, 47]
    if ((actual_strip_index % 2 == 0 and not swap) or (actual_strip_index % 2 == 1 and swap)):
        led_count = STRIP_TOP
    else:
        led_count = STRIP_BOTTOM
        opc_start_index += STRIP_TOP

    return [opc_channel, opc_start_index, fadecandy_start_index, led_count]

for i in range(len(FADECANDY_SERIAL_NUMBERS)):
  device = {
    "type": "fadecandy",
    "serial": FADECANDY_SERIAL_NUMBERS[i],
    "led": False,
    "map": [
        get_map(i, 0),
        get_map(i, 1),
        get_map(i, 2),
        get_map(i, 3),
        get_map(i, 4),
        get_map(i, 5),
        get_map(i, 6),
        get_map(i, 7),
    ]
  }
  config["devices"].append(device)

print(json.dumps(config))
