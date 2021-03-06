# Important optimization options
CXXFLAGS = -O3 -ffast-math -fno-rtti

# Standard libraries
#LDFLAGS = -lm -lstdc++ -lpthread
LDFLAGS = -lm -lpthread -lEGL -lGLESv2

# Debugging
CXXFLAGS += -g -Wall
LDFLAGS += -g

CFLAGS += -std=c++11

# Annoying warnings on by default on Mac OS
CXXFLAGS += -Wno-tautological-constant-out-of-range-compare -Wno-gnu-static-float-init

dev: openpixelcontrol/bin/gl_server trace layout.json

all: trace fadecandy-config.json layout.json

fadecandy-config.json:
	./scripts/fadecandy-config.py > fadecandy-config.json

openpixelcontrol/bin/gl_server:
	git submodule init
	git submodule update
	make -C openpixelcontrol bin/gl_server

fadecandy/bin/server:
	git submodule init
	git submodule update
	make -C fadecandy/server submodules
	make -C fadecandy/server

objects := $(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp))

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I/opt/vc/include

trace: $(objects)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) -L/opt/vc/lib

layout.json:
	./scripts/generate-layout.py > layout.json

.PHONY: clean all

clean:
	rm -rf trace trace.o layout.json **.dSYM obj/*
