#include <stdio.h>
#include <unistd.h>

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
  gl::readFrame(OPCClient::Header::view(buffer).data());

  opc.write(buffer);

  gl::exit();
}
