#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#include "fadecandy/opc_client.h"
#include "gl.h"
#include "load.h"
#include "config.h"

void sigint_handler(int sig_num) {
  exit(gl::exit());
}

int main(int argv, char** argc) {

  signal(SIGINT, sigint_handler);

  const unsigned int headerSize = sizeof(OPCClient::Header);

  printf("running trace\n");
  printf("loading configuration\n");
  config::init();

  // Create buffer to hold entire front buffer pixels
  // We multiply width and height by 3 to because we use RGB!
  std::vector<uint8_t> buffer;
  buffer.resize(headerSize + gl::byteCount);

  OPCClient opc;
  OPCClient::Header::view(buffer).init(0, opc.SET_PIXEL_COLORS, gl::width * gl::height * 3);
  printf("connecting to address: %s\n", config::get_address());
  opc.resolve(config::get_address());
  opc.tryConnect();

  gl::init();

  printf("loading shaders\n");
  printf("found override: %s\n", config::get_override());
  const char* fragSource = load::file(config::get_override());
  struct gl::Program p;
  gl::create_program(fragSource, p);

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
    gl::read_frame(time, p, &read_buffer[0]);
    for (unsigned int i = 0; i < gl::byteCount; i++) {
      data_out[i] = read_buffer[i];
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
