#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "fadecandy/opc_client.h"
#include "gl.h"


int main(int argv, char** argc){
  const unsigned int headerSize = sizeof(OPCClient::Header);

  printf("running trace\n");

  // Create buffer to hold entire front buffer pixels
  // We multiply width and height by 3 to because we use RGB!
  std::vector<uint8_t> buffer;
  buffer.resize(headerSize + gl::dataSize);

  OPCClient opc;
  OPCClient::Header::view(buffer).init(0, opc.SET_PIXEL_COLORS, gl::dataSize);
  opc.resolve("192.168.1.12");
  opc.tryConnect();

  gl::init();
  gl::createProgram();

  float time, delta = 0.0;
  struct timeval now;
  gettimeofday(&now, 0);
  struct timeval lastTime = now;

  while (time < 10.0) {
    gettimeofday(&now, 0);
    float delta = (now.tv_sec - lastTime.tv_sec)
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
  }

  gl::exit();
}
