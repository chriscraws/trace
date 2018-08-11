#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "fadecandy/rapidjson/rapidjson.h"
#include "fadecandy/rapidjson/filestream.h"
#include "fadecandy/rapidjson/document.h"

using namespace std;

namespace {
  rapidjson::Document config;
  vector<double> pixel_locations;

  double get_double(const rapidjson::Value* a, int i) {
    if (a->IsArray()) {
        const rapidjson::Value& b = &((*a)["point"])[i];
        if (b.IsNumber()) {
            return b.GetDouble();
        }
    }
    return 0.0;
  }

  void push_pixel(rapidjson::Value *attribute) {
    pixel_locations.push_back(get_double(attribute, 0));
    pixel_locations.push_back(get_double(attribute, 1));
    pixel_locations.push_back(get_double(attribute, 2));
  }
}

namespace load {

const char* file(const char* name) {
  FILE *fp = fopen(name, "rb");
  if (fp) {
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    char* contents = (char*) malloc(size + 1);
    rewind(fp);
    fread(contents, 1, size, fp);
    contents[size] = '\0';
    fclose(fp);
    return (const char*) contents;
  }
  return "";
}

vector<double>* layout(const char* filename) {
  rapidjson::Document layout;

  FILE *f = fopen(filename, "r");

  if (!f) {
      return NULL;
  }

  rapidjson::FileStream istr(f);
  layout.ParseStream<0>(istr);
  fclose(f);

  if (layout.HasParseError()) {
      return NULL;
  }
  if (!layout.IsArray()) {
      return NULL;
  }

  pixel_locations.clear();
  for (unsigned int i = 0; i < layout.Size(); i++) {
    push_pixel(&layout[i]);
  }

  return &pixel_locations;
}

rapidjson::Document* get_config() {
  FILE *f = fopen("config.json", "r");
  if (!f) {
    printf("Error opening configuration json\n");
    return NULL;
  }

  rapidjson::FileStream istr(f);
  config.ParseStream<0>(istr);
  fclose(f);

  if (config.HasParseError()) {
    printf("Error parsing configuration json\n");
    return NULL;
  }

  return &config;
}

}
