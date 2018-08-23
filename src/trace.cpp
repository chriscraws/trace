#include <iostream>

#include "gpu.h"
#include "pixel_converter.h"
#include "scene.h"
#include "transition.h"
#include "config.h"
#include "fadecandy/opc_client.h"
#include "opc.h"
#include "sequencer.h"

using namespace std;

int main()
{
  Config config;
  Gpu gpu;

  if (config.load_layout() ||
      config.load_config()) {
    return 1;
  }

  int pixel_count = config.get_pixel_locations().size() / 3;
  cout << "Loaded " << pixel_count << " pixels" << endl;

  if (gpu.initialize(pixel_count)) {
    return 1;
  }

  // Normalize location vector
  vector<float>& locations = config.get_pixel_locations();
  vector<float> normalized_locations(locations.size());
  vector<float> offsets(locations.size());

  PixelConverter::normalize_location(locations, normalized_locations, offsets);

  // Create textures
  gpu.generate_location_texture(normalized_locations);
  gpu.generate_offset_texture(offsets);
  gpu.generate_scene_textures();

  // Create scenes
  vector<Scene> scenes;
  for (const auto& settings : config.get_scene_info()) {
    scenes.emplace(scenes.end(), &gpu, settings.filename.c_str());
  }

  // Create transitions
  vector<Transition> transitions;
  for (const auto& settings : config.get_transition_info()) {
    transitions.emplace(transitions.end(), &gpu, settings.filename.c_str());
  }

  cout << "Loaded " << scenes.size() << " scenes.\n";

  // Connect to OPC
  Opc opc(
    config.get_primary_address(),
    config.get_primary_port(),
    config.get_secondary_address(),
    config.get_secondary_port(),
    pixel_count * 3
  );

  Sequencer sequencer(
    config.get_scene_time(), 
    config.get_transition_time(),
    scenes,
    transitions,
    &opc,
    &gpu,
    config.get_override_index()
  );

  cout << "Running\n";
  sequencer.run();

  return 0;
}
