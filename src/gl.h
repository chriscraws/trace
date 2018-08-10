#pragma once

#include <unistd.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "constants.h"
#include "fadecandy/effect_runner.h"

namespace {
  static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_NONE
  }; 

  static const EGLint pbufwidth = TOP_TUBE_COUNT + GROUND_TUBE_COUNT;
  static const EGLint pbufheight  = LEDS_PER_TUBE + GROUND_LED_COUNT;
  static const EGLint pbufsize = pbufwidth * pbufheight;

  // Width and height of the desired framebuffer
  static const EGLint pbufferAttribs[] = {
    EGL_WIDTH, pbufwidth,
    EGL_HEIGHT, pbufheight,
    EGL_NONE,
  };

  static const EGLint contextAttribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
  };

  // The following array holds vec3 data of 
  // three vertex positions
  static const GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
  };

  // The following are GLSL shaders for rendering a triangle on the screen
#define STRINGIFY(x) #x
  static const char* vertexShaderCode = STRINGIFY(
      attribute vec3 aPos;
      void main() {
      gl_Position = vec4(aPos, 1.0);
      }
      );

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

  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  int major, minor;
  int desiredWidth, desiredHeight;
  GLuint program, vert, frag, vbo, pointPositionTexture;
  GLint posLoc, colorLoc, timeLoc, pointPosLoc;
  EffectRunner runner;

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

  GLubyte value_to_byte(float value) {
    float normalized = value / TraceUtil::halfheight;
    normalized /= 2.0;
    normalized += 0.5;
    return 255 * normalized;
  }
}

namespace gl {
  static const EGLint width = pbufwidth;
  static const EGLint height = pbufheight;
  static const EGLint byteCount = pbufsize * 4;

  int init() {
    if((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY){
      fprintf(stderr, "Failed to get EGL display! Error: %s\n", eglGetErrorStr());
      return EXIT_FAILURE;
    }

    if(eglInitialize(display, &major, &minor) == EGL_FALSE){
      fprintf(stderr, "Failed to get EGL version! Error: %s\n", eglGetErrorStr());
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

    // The desired width and height is defined inside of pbufferAttribs
    // Check top of this file for EGL_WIDTH and EGL_HEIGHT
    desiredWidth = pbufferAttribs[1]; // 800
    desiredHeight = pbufferAttribs[3]; // 600

    // Set GL Viewport size, always needed!
    glViewport(0, 0, desiredWidth, desiredHeight);

    // Get GL Viewport size and test if it is correct.
    // NOTE! DO NOT UPDATE EGL LIBRARY ON RASPBERRY PI AS IT WILL INSTALL FAKE EGL!
    // If you have fake/faulty EGL library, the glViewport and glGetIntegerv won't work!
    // The following piece of code checks if the gl functions are working as intended!
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // viewport[2] and viewport[3] are viewport width and height respectively
    printf("GL Viewport size: %dx%d\n", viewport[2], viewport[3]);

    // Test if the desired width and height match the one returned by glGetIntegerv
    if(desiredWidth != viewport[2] || desiredHeight != viewport[3]){
      fprintf(stderr, "Error! The glViewport/glGetIntegerv are not working! EGL might be faulty!\n");
    }

    // load layout
    printf("loading layout\n");
    runner.setLayout("layout.json");
    const Effect::PixelInfoVec& pixelInfo = runner.getPixelInfo();

    GLubyte pixelLocation[byteCount];
    for (Effect::PixelInfoIter i = pixelInfo.begin(), e = pixelInfo.end(); i != e; ++i) {

      const Effect::PixelInfo &p = *i;
      for (unsigned int j = 0; j < 4; j++) {
        pixelLocation[p.index * 4 + j] = value_to_byte(p.point[j]);
      }
    }

    printf("\n");

    printf("creating texture\n");

    // set pack and upack settings
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // create texture
    glGenTextures(1, &pointPositionTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pointPositionTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    glTexImage2D(
      GL_TEXTURE_2D, /* target */
      0, /* level of detail (mipmap) */
      GL_RGBA, /* internalFormat */
      (GLsizei) pbufwidth, /* width */
      (GLsizei) pbufheight, /* height */
      0, /* border must be 0 */
      GL_RGBA, /* format must match internalFormat */
      GL_UNSIGNED_BYTE, /* type */
      &pixelLocation[0] /* data */
    );

    dumpErrors();

    return EXIT_SUCCESS;
  }

  int createProgram(const char* fragSource) {
    program = glCreateProgram();
    glUseProgram(program);
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertexShaderCode, NULL);
    glCompileShader(vert);
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSource, NULL);
    glCompileShader(frag);
    glAttachShader(program, frag);
    glAttachShader(program, vert);
    glLinkProgram(program);
    glUseProgram(program);

    glValidateProgram(program);
    GLint pstatus;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &pstatus);
    if (pstatus == GL_FALSE) {
      printf("Invalid program state\n");
      return EXIT_FAILURE;
    }
      
    // Create Vertex Buffer Object
    // Again, NO ERRRO CHECKING IS DONE! (for the purpose of this example)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vertices, GL_STATIC_DRAW);

    // Get vertex attribute and uniform locations
    posLoc = glGetAttribLocation(program, "aPos");
    colorLoc = glGetUniformLocation(program, "color");
    timeLoc = glGetUniformLocation(program, "time");
    pointPosLoc = glGetUniformLocation(program, "spos");

    // set up the texture
    glUniform1i(pointPosLoc, 0);

    // Set the desired color of the triangle to pink
    // 100% red, 0% green, 50% blue, 100% alpha
    glUniform4f(colorLoc, 1.0, 1.0f, 1.0, 1.0);

    // Set our vertex data
    glEnableVertexAttribArray(posLoc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    return EXIT_SUCCESS;
  }
  
  void readFrame(float time, uint8_t* buffer) {
    // Clear whole screen (front buffer)
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniform1f(timeLoc, time);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glReadPixels(0, 0, desiredWidth, desiredHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
  }


  int exit() {
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
    return EXIT_SUCCESS;
  }
}
