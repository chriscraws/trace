#include "gpu.h"

#include <cmath>
#include <iostream>
#include <unistd.h>

namespace {
static const char* eglGetErrorStr(){
  switch(eglGetError()){
    case EGL_SUCCESS: return "The last function succeeded without error.";
    case EGL_NOT_INITIALIZED: return "EGL is not initialized, or could not be initialized, for the specified EGL display connection.";
    case EGL_BAD_ACCESS: return "EGL cannot access a requested resource (for example a context is bound in another thread).";
    case EGL_BAD_ALLOC: return "EGL failed to allocate resources for the requested operation.";
    case EGL_BAD_ATTRIBUTE: return "An unrecognized attribute or attribute value was passed in the attribute list.";
    case EGL_BAD_CONTEXT: return "An EGLContext argument does not name a valid EGL rendering context.";
    case EGL_BAD_CONFIG: return "An EGLConfig argument does not name a valid EGL frame buffer configuration.";
    case EGL_BAD_CURRENT_SURFACE: return "The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid.";
    case EGL_BAD_DISPLAY: return "An EGLDisplay argument does not name a valid EGL display connection.";
    case EGL_BAD_SURFACE: return "An EGLSurface argument does not name a valid surface (window, pixel buffer or pixmap) configured for GL rendering.";
    case EGL_BAD_MATCH: return "Arguments are inconsistent (for example, a valid context requires buffers not supplied by a valid surface).";
    case EGL_BAD_PARAMETER: return "One or more argument values are invalid.";
    case EGL_BAD_NATIVE_PIXMAP: return "A NativePixmapType argument does not refer to a valid native pixmap.";
    case EGL_BAD_NATIVE_WINDOW: return "A NativeWindowType argument does not refer to a valid native window.";
    case EGL_CONTEXT_LOST: return "A power management event has occurred. The application must destroy all contexts and reinitialise OpenGL ES state and objects to continue rendering.";
    default: break;
  }
  return "Unknown error!";
}

static const EGLint configAttribs[] = {
  EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
  EGL_BLUE_SIZE, 8,
  EGL_GREEN_SIZE, 8,
  EGL_RED_SIZE, 8,
  EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
  EGL_NONE
};

static const EGLint contextAttribs[] = {
  EGL_CONTEXT_CLIENT_VERSION, 2,
  EGL_NONE
};

static const GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
  };

#define STRINGIFY(x) #x
  static const char* vertexShaderCode = STRINGIFY(
      attribute vec3 aPos;
      void main() {
      gl_Position = vec4(aPos, 1.0);
      }
      );

void create_texture(
  GLuint name,
  const GLubyte* data,
  GLsizei width,
  GLsizei height,
  GLuint position
) {
  glActiveTexture(GL_TEXTURE0 + position);
  glBindTexture(GL_TEXTURE_2D, name);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(
    GL_TEXTURE_2D, /* target */
    0, /* level of detail (mipmap) */
    GL_RGB, /* internalFormat */
    width, /* width */
    height, /* height */
    0, /* border must be 0 */
    GL_RGB, /* format must match internalFormat */
    GL_UNSIGNED_BYTE, /* type */
    data /* data */
  );

  //glBindTexture(GL_TEXTURE_2D, 0);
}


void dumpErrors() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    printf("GL error: ");
    switch (err) {
      case GL_INVALID_ENUM:
        printf("invalid enum");
        break;
      case GL_INVALID_VALUE:
        printf("invalid value");
        break;
      case GL_INVALID_OPERATION:
        printf("invalid operation");
        break;
      case GL_OUT_OF_MEMORY:
        printf("out of memory");
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        printf("invalid framebuffer op");
        break;
      default:
        break;
    }
    printf("\n");
  }
}

}

int Gpu::initialize(int pixel_count) {
  if((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY){
    std::cerr << "Failed to get EGL display! Error: " <<  eglGetErrorStr() << std::endl;
    return EXIT_FAILURE;
  }

  if(eglInitialize(display, &major, &minor) == EGL_FALSE){
    std::cerr << "Failed to get EGL version! Error: " << eglGetErrorStr() << std::endl;
    eglTerminate(display);
    return EXIT_FAILURE;
  }

  printf("Initialized EGL version: %d.%d\n", major, minor);

  EGLint numConfigs;
  EGLConfig config;
  if(!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs)){
    fprintf(stderr, "Failed to get EGL config! Error: %s\n", eglGetErrorStr());
    eglTerminate(display);
    return EXIT_FAILURE;
  }

  // Get closest factors
  width = 64;
  height = (int) std::ceil((float) pixel_count / 64.0f);

  EGLint pbufferAttribs[5] = {
    EGL_WIDTH, width,
    EGL_HEIGHT, height,
    EGL_NONE
  };

  surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
  if(surface == EGL_NO_SURFACE){
    fprintf(stderr, "Failed to create EGL surface! Error: %s\n", eglGetErrorStr());
    eglTerminate(display);
    return EXIT_FAILURE;
  }

  eglBindAPI(EGL_OPENGL_API);

  context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
  if(context == EGL_NO_CONTEXT){
    fprintf(stderr, "Failed to create EGL context! Error: %s\n", eglGetErrorStr());
    eglDestroySurface(display, surface);
    eglTerminate(display);
    return EXIT_FAILURE;
  }

  eglMakeCurrent(display, surface, surface, context);

  EGLint desiredWidth = pbufferAttribs[1];
  EGLint desiredHeight = pbufferAttribs[3];

  // Set GL Viewport size, always needed!
  glViewport(0, 0, desiredWidth, desiredHeight);

  // Get GL Viewport size and test if it is correct.
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  // viewport[2] and viewport[3] are viewport width and height respectively
  printf("GL Viewport size: %dx%d\n", viewport[2], viewport[3]);

  // Test if the desired width and height match the one returned by glGetIntegerv
  if(desiredWidth != viewport[2] || desiredHeight != viewport[3]){
    std::cerr << "Error! The glViewport/glGetIntegerv are not working! EGL might be faulty!" << std::endl;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  // create texture
  glGenTextures(TEXTURE_NAME_COUNT, &texture_names[0]);

  // vertex buffer
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vertices, GL_STATIC_DRAW);

  // vertex shader
  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vertexShaderCode, NULL);
  glCompileShader(vert);

  // framebuffer objects
  glGenFramebuffers(2, &framebuffers[1]);
  framebuffers[0] = 0;

  dumpErrors();

  return EXIT_SUCCESS;
}

void dump_program_log(GLuint program_id) {
  GLint log_length;
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
  GLchar* log = (GLchar*) malloc(log_length);
  glGetProgramInfoLog(program_id, log_length, NULL, log);

  std::cout << "program log dump: " << std::endl << log << std::endl;
}

int Gpu::create_program(const char* frag_source) {
  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_id, 1, &frag_source, NULL);
  glCompileShader(frag_id);

  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vert);
  glAttachShader(program_id, frag_id);
  glLinkProgram(program_id);

  GLint link_status;
  glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
  if (!link_status) {
    std::cerr << "Failed to link GL program" << std::endl;
    dump_program_log(program_id);
    return -1;
  }

  GLint validate_status;
  glValidateProgram(program_id);
  glGetProgramiv(program_id, GL_VALIDATE_STATUS, &validate_status);
  if (!validate_status) {
    std::cerr << "Failed to validate GL program" << std::endl;
    dump_program_log(program_id);
    return -1;
  }

  // Set vertex data
  glUseProgram(program_id);
  GLint pos_loc = glGetAttribLocation(program_id, "aPos");
  glEnableVertexAttribArray(pos_loc);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  programs.push_back(program_id);
  return programs.size() - 1;
}

int Gpu::generate_offset_texture(std::vector<float>& pixel_offsets) {
  std::vector<GLubyte> out;
  out.resize(pixel_offsets.size());

  for (unsigned int i = 0; i < pixel_offsets.size(); i++) {
    out[i] = 255 * pixel_offsets[i];
  }

  create_texture(
    texture_names[OFFSET_TEXTURE_NAME],
    &out[0],
    width,
    height,
    1
  );

  return 0;
}

int Gpu::generate_location_texture(std::vector<float>& pixel_locations) {
  std::vector<GLubyte> out;
  out.resize(pixel_locations.size());

  for (unsigned int i = 0; i < pixel_locations.size(); i++) {
    out[i] = 255 * pixel_locations[i];
  }

  create_texture(
    texture_names[POSITION_TEXTURE_NAME],
    &out[0],
    width,
    height,
    0
  );

  return 0;
}

void Gpu::generate_scene_textures() {
  create_texture(
    texture_names[SCENE_A_TEXTURE_NAME],
    NULL,
    width,
    height,
    2
  );

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[1]);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    GL_TEXTURE_2D,
    texture_names[SCENE_A_TEXTURE_NAME],
    0
  );

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
    std::cerr << "framebuffer ded" << std::endl;
  }

  create_texture(
    texture_names[SCENE_B_TEXTURE_NAME],
    NULL,
    width,
    height,
    3
  );

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[2]);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    GL_TEXTURE_2D,
    texture_names[SCENE_B_TEXTURE_NAME],
    0
  );

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
    std::cerr << "framebuffer ded" << std::endl;
  }
}

void Gpu::render(int program_id, int target, float time, float mix) {
  GLuint pid = programs[program_id];

  if (target != 0) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, 0);
  } else {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_names[SCENE_A_TEXTURE_NAME]);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture_names[SCENE_B_TEXTURE_NAME]);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[target]);
  if (target > 0) {
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        texture_names[1 + target],
        0
        );
  }

  glUseProgram(pid);
  GLint timeLoc, pointPosLoc, pointOffsetLoc, mixLoc, aLoc, bLoc;
  timeLoc = glGetUniformLocation(pid, "time");
  pointPosLoc = glGetUniformLocation(pid, "spos");
  pointOffsetLoc = glGetUniformLocation(pid, "soff");
  mixLoc = glGetUniformLocation(pid, "tmix");
  aLoc = glGetUniformLocation(pid, "sceneA");
  bLoc = glGetUniformLocation(pid, "sceneB");

  // set up the textures
  glUniform1i(pointPosLoc, 0);
  glUniform1i(pointOffsetLoc, 1);

  if (target == 0) {
    glUniform1i(aLoc, 2);
    glUniform1i(bLoc, 3);
  }

  // Clear whole screen (front buffer)
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniform1f(timeLoc, time);
  glUniform1f(mixLoc, mix);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Gpu::read(uint8_t* buffer) {
  glFinish();
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, (GLuint*) buffer);
}
