# Important optimization options
CXXFLAGS = -O3 -ffast-math -fno-rtti

# Standard libraries
#LDFLAGS = -lm -lstdc++ -lpthread
LDFLAGS = -lm -lpthread

# Debugging
CXXFLAGS += -g -Wall
LDFLAGS += -g

CFLAGS += -std=c++11

# Annoying warnings on by default on Mac OS
CXXFLAGS += -Wno-tautological-constant-out-of-range-compare -Wno-gnu-static-float-init

all: openpixelcontrol/bin/gl_server trace layout.json

openpixelcontrol/bin/gl_server:
	cd openpixelcontrol
	make bin/gl_server

trace:
	$(CXX) $(CXXFLAGS) src/trace.cpp -o trace $(LDFLAGS)

layout.json:
	./generate-layout.py > layout.json

.PHONY: clean all

clean:
	rm -rf trace layout.json **.dSYM
