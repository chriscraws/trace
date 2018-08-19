#pragma once

#include <vector>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace {

enum TextureNames {
  POSITION_TEXTURE_NAME,
  OFFSET_TEXTURE_NAME,
  SCENE_A_TEXTURE_NAME,
  SCENE_B_TEXTURE_NAME,
  TEXTURE_NAME_COUNT
};

}

class Gpu {
  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  GLuint vert, vbo;
  GLuint framebuffers[3];
  int major, minor, width, height;
  GLuint texture_names[TEXTURE_NAME_COUNT];
  std::vector<GLuint> programs;

public:
  int initialize(int pixel_count);
  int generate_location_texture(std::vector<float>& pixel_locations);
  int generate_offset_texture(std::vector<float>& pixel_offsets);
  void generate_scene_textures();
  int create_program(const char* frag_source);
  void render(int program_id, int target, float time, float mix = 0.0f);
  void read(uint8_t* buffer);
};
