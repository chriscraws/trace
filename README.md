# trace
LED control for Trace

https://www.indiegogo.com/projects/trace-burning-man-2018-design-community


## Installation

On linux ensure the packages gcc, g++, and freeglut3-dev are installed.

The dev script requires `entr` to be installed (`sudo apt install entr` on linux, `brew install entr` on mac)

## Development

Open a terminal to the trace directory and run `./scripts/dev`. This will launch `openpixelcontrol/bin/gl_server` to listen on port 8000. The trace executable will then run - it will exit, recompile, and run again if any `.h` or `.cpp` files inside the trace folder are edited.

## Configuration

The file `fadecandy-config.py` has a section beginning on line 12 that needs all the serial numbers of the attached fadecandy devices. Currently the test pattern oscillates the entire cube white, so order shouldn't matter yet.


## Running

Compile trace with `make`

This should generate layout.json, fadecandy-config.json, and the trace executable.

Run trace and the server with `./trace -layout layout.json -server 127.0.0.1:8000 & ./fcserver-rpi fadecandy-config.json`
