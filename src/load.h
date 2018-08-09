#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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

}
