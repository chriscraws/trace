#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#include "fadecandy/opc_client.h"
#include "gl.h"

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
  OPCClient::Header::view(buffer).init(0, opc.SET_PIXEL_COLORS, gl::byteCount);
  opc.resolve("192.168.1.12");
  opc.tryConnect();

  gl::init();
  gl::createProgram();

  float time = 0.0, delta = 0.0;
  struct timeval now;
  gettimeofday(&now, 0);
  struct timeval lastTime = now;
  float filteredTimeDelta;
  float minTimeDelta = 1.0 / 300.0;
  float currentDelay = 0.0;
  const float filterGain = 0.05;

  while (true) {
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

    gl::readFrame(time, OPCClient::Header::view(buffer).data());
    opc.write(buffer);

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
