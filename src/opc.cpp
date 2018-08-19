#include "opc.h"

#include <iostream>
#include <chrono>
#include <thread>

Opc::Opc(
    const char* primary_address,
    int primary_port,
    const char* secondary_address,
    int secondary_port,
    int buf_size
) : buf_size(buf_size) {

  while (!main_client.isConnected()) {
    main_client.resolve(primary_address, primary_port);
    std::cout << "Connecting to " << primary_address << std::endl;
    main_client.tryConnect();

    if (!main_client.isConnected()) {
      std::cout << "connection failed, waiting to reconnect." << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

  while (!secondary_client.isConnected()) {
    secondary_client.resolve(secondary_address, secondary_port);
    std::cout << "Connecting to " << secondary_address << std::endl;
    secondary_client.tryConnect();

    if (!secondary_client.isConnected()) {
      std::cout << "connection failed, waiting to reconnect." << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
  
  buffer.resize(sizeof(OPCClient::Header) + buf_size);
  OPCClient::Header::view(buffer).init(0, main_client.SET_PIXEL_COLORS, buf_size);
}

uint8_t* Opc::get_data_pointer() {
  return OPCClient::Header::view(buffer).data();
}

void Opc::send() {
  // reduce / prevent flickering
  for (int i = 0; i < buf_size; i+=3) {
    uint8_t* c = &OPCClient::Header::view(buffer).data()[i];
    if (c[0] < 10 &&
        c[1] < 10 &&
        c[2] < 10) {
      c[0] = c[1] = c[2] = 0;
    }
  }
  main_client.write(buffer);
  secondary_client.write(buffer);
}

