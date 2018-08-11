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

  double get_double(const rapidjson::Value& a, int i) {
    return a[i].GetDouble();
  }

  void push_pixel(rapidjson::Value& attribute) {
    const rapidjson::Value& a = attribute["point"];
    pixel_locations.push_back(get_double(a, 0));
    pixel_locations.push_back(get_double(a, 1));
    pixel_locations.push_back(get_double(a, 2));
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

const vector<double>& layout(const char* filename) {
  rapidjson::Document layout;

  FILE *f = fopen(filename, "r");

  if (!f) {
    printf("Layout file not found: %s\n", filename);
  }

  rapidjson::FileStream istr(f);
  layout.ParseStream<0>(istr);
  fclose(f);

  if (layout.HasParseError()) {
    printf("Parsing error in layout.\n");
  }
  if (!layout.IsArray()) {
    printf("Layout is not a list of points.\n");
  }

  pixel_locations.clear();
  for (rapidjson::SizeType i = 0; i < layout.Size(); i++) {
    push_pixel(layout[i]);
  }

  return pixel_locations;
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
