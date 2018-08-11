#pragma once

#include "load.h"

namespace {
  rapidjson::Document* doc;

}

namespace config {
  void init() {
    doc = load::get_config();
  }

  const char* get_address() {
    return (*doc)["address"].GetString();
  }
}
