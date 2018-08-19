#pragma once

#include "fadecandy/opc_client.h"

class Opc {
  OPCClient main_client;
  OPCClient secondary_client;
  std::vector<uint8_t> buffer;
  int buf_size;

public:
  Opc(
    const char* primary_address,
    int primary_port,
    const char* secondary_address,
    int secondary_port,
    int buf_size
  );
  uint8_t* get_data_pointer();
  void send();
};
