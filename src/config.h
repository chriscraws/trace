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

  const char* get_layout_filename() {
    return (*doc)["layout"].GetString();
  }

  const char* get_override() {
    if (doc->HasMember("override")) {
      return (*doc)["override"].GetString();
    }
    return NULL;
  }
}
