#include "config.h"

#include <iostream>

#include "fadecandy/rapidjson/rapidjson.h"
#include "fadecandy/rapidjson/filestream.h"

Config::Config() {}

int Config::load_config() {
  FILE *f = fopen("config.json", "r");
  if (!f) {
    std::cerr << "Error opening configuration json" << std::endl;
    return 1;
  }

  rapidjson::FileStream istr(f);
  config_document.ParseStream<0>(istr);
  fclose(f);

  if (config_document.HasParseError()) {
    std::cerr << "Error parsing configuration json" << std::endl;
    return 1;
  }

  return 0;
}

int Config::load_layout() {
  rapidjson::Document layout;

  const char* filename = "layout.json";

  FILE *f = fopen(filename, "r");

  if (!f) {
    std::cerr << "Layout file not found: " << filename << std::endl;
    return 1;
  }

  rapidjson::FileStream istr(f);
  layout.ParseStream<0>(istr);
  fclose(f);

  if (layout.HasParseError()) {
    std::cerr << "Parsing error in layout." << std::endl;
    return 1;
  }
  if (!layout.IsArray()) {
    std::cerr << "Layout is not a list of points." << std::endl;
    return 1;
  }

  pixel_locations.clear();
  for (rapidjson::SizeType i = 0; i < layout.Size(); i++) {
    push_pixel(layout[i]);
  }

  return 0;
}

vector<float>& Config::get_pixel_locations() {
  return pixel_locations;
}

void Config::push_pixel(rapidjson::Value& attribute) {
  const rapidjson::Value& a = attribute["point"];
  for (int i = 0; i < 3; i++) {
    pixel_locations.push_back(a[i].GetDouble());
  }
}

vector<SceneInfo>& Config::get_transition_info() {
  if (!config_document["transitions"].IsArray()) {
    std::cerr << "config.json missing transition definitions" << std::endl;
    return transition_info;
  }

  uint32_t scene_count = config_document["transitions"].Size();
  transition_info.resize(scene_count);
  for (uint32_t i = 0; i < scene_count; i++) {
    auto& scene = config_document["transitions"][i];
    transition_info[i].filename.assign(scene["filename"].GetString());
  }

  return transition_info;
}

vector<SceneInfo>& Config::get_scene_info() {
  if (!config_document["scenes"].IsArray()) {
    std::cerr << "config.json missing scene definitions" << std::endl;
    return info;
  }

  uint32_t scene_count = config_document["scenes"].Size();
  info.resize(scene_count);
  for (uint32_t i = 0; i < scene_count; i++) {
    auto& scene = config_document["scenes"][i];
    info[i].filename.assign(scene["filename"].GetString());
  }

  return info;
}

const char* Config::get_primary_address() {
  return config_document["address"]["ip"].GetString();
}

const char* Config::get_secondary_address() {
  return config_document["secondary_address"]["ip"].GetString();
}

float Config::get_scene_time() {
  return (float) config_document["scene_time"].GetDouble();
}

float Config::get_transition_time() {
  return (float) config_document["transition_time"].GetDouble();
}

int Config::get_primary_port() {
  return config_document["address"]["port"].GetInt();
}
int Config::get_secondary_port() {
  return config_document["secondary_address"]["port"].GetInt();
}

int Config::get_override_index() {
  if (!config_document.HasMember("override")) {
    return -1;
  }
  return config_document["override"].GetInt();
}
