#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#include "fadecandy/opc_client.h"
#include "gl.h"
#include "load.h"

void sigint_handler(int sig_num) {
  exit(gl::exit());
}

int main(int argv, char** argc){

  signal(SIGINT, sigint_handler);

  const unsigned int headerSize = sizeof(OPCClient::Header);

  printf("running trace\n");

  // Create buffer to hold entire front buffer pixels
  // We multiply width and height by 3 to because we use RGB!
  std::vector<uint8_t> buffer;
  buffer.resize(headerSize + gl::byteCount);

  OPCClient opc;
  OPCClient::Header::view(buffer).init(0, opc.SET_PIXEL_COLORS, gl::width * gl::height * 3);
  opc.resolve("192.168.1.12");
  opc.tryConnect();

  gl::init();

  printf("loading shaders\n");
  const char* fragSource = load::file("shaders/test.frag");
  gl::createProgram(fragSource);

  float time = 0.0, delta = 0.0;
  struct timeval now;
  gettimeofday(&now, 0);
  struct timeval lastTime = now;
  float filteredTimeDelta;
  float minTimeDelta = 1.0 / 300.0;
  float currentDelay = 0.0;
  const float filterGain = 0.05;

  uint8_t read_buffer[gl::byteCount];

  while (true) {
    // send frame
    uint8_t* data_out = OPCClient::Header::view(buffer).data();
    gl::readFrame(time, &read_buffer[0]);
    for (unsigned int i = 0; i < gl::width * gl::height; i++) {
      for (int j = 0; j < 3; j++) {
        data_out[i * 3 + j] = read_buffer[i * 4 + j];
      }
    }
    opc.write(buffer);

    // calculate timing stuff
    gettimeofday(&now, 0);
    delta = (now.tv_sec - lastTime.tv_sec)
        + 1e-6 * (now.tv_usec - lastTime.tv_usec);
    lastTime = now;

    // Max timestep; jump ahead if we get too far behind.
    const float maxStep = 0.1;
    if (delta > maxStep) {
        delta = maxStep;
    }

    time += delta;

    // Low-pass filter for timeDelta, to estimate our frame rate
    filteredTimeDelta += (delta - filteredTimeDelta) * filterGain;

    // Negative feedback loop to adjust the delay until we hit a target frame rate.
    // This lets us hit the target rate smoothly, without a lot of jitter between frames.
    // If we calculated a new delay value on each frame, we'd easily end up alternating
    // between too-long and too-short frame delays.
    currentDelay += (minTimeDelta - delta) * filterGain;

    // Make sure filteredTimeDelta >= currentDelay. (The "busy time" estimate will be >= 0)
    filteredTimeDelta = std::max(filteredTimeDelta, currentDelay);

    // Add the extra delay, if we have one. This is how we throttle down the frame rate.
    if (currentDelay > 0) {
        usleep(currentDelay * 1e6);
    }
  }

  gl::exit();
}
