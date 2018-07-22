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

dev: openpixelcontrol/bin/gl_server trace layout.json

all: trace fadecandy-config.json

fadecandy-config.json:
	./fadecandy-config.py > fadecandy-config.json

openpixelcontrol/bin/gl_server:
	git submodule init
	git submodule update
	make -C openpixelcontrol bin/gl_server

fadecandy/bin/server:
	git submodule init
	git submodule update
	make -C fadecandy/server submodules
	make -C fadecandy/server

trace:
	$(CXX) $(CXXFLAGS) src/trace.cpp -o trace $(LDFLAGS)

layout.json:
	./generate-layout.py > layout.json

.PHONY: clean all

clean:
	rm -rf trace layout.json **.dSYM
