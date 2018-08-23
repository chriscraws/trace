#pragma once

#include <vector>
#include <string>

#include "fadecandy/rapidjson/document.h"

using std::vector;

class SceneInfo {
public:
  std::string filename;
};

class Config {
  vector<float> pixel_locations;
  vector<SceneInfo> info;
  vector<SceneInfo> transition_info;
  rapidjson::Document config_document;

public:
  Config();
  vector<float>& get_pixel_locations();
  int load_config();
  int load_layout();
  const char* get_primary_address();
  const char* get_secondary_address();
  int get_primary_port();
  int get_secondary_port();
  float get_scene_time();
  float get_transition_time();
  int get_override_index();

  vector<SceneInfo>& get_scene_info();
  vector<SceneInfo>& get_transition_info();

private:
  void push_pixel(rapidjson::Value& attribute);
};
