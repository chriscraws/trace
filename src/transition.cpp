#include "transition.h"

#include <iostream>
#include <fstream>
#include <sstream>

Transition::Transition(Gpu* g, const char* filename) : gpu(g) {
  std::ifstream t(filename);
  std::stringstream file_buffer;
  file_buffer << t.rdbuf();
  
  std::cout << "Compiling " << filename << std::endl;
  frag_id = gpu->create_program(file_buffer.str().c_str());

  if (frag_id < 0) {
    std::cerr << "Scene " << filename << " failed to compile" << std::endl;
  }
}

void Transition::draw(float time, float mix) {
  gpu->render(frag_id, 0, time, mix);
}
