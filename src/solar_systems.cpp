#include "solar_systems.h"

using namespace kardeshev;

std::string SolarSystemInfo::getNameOrId() const {
  if (name.empty()) {
    return solar_system->getIdAsString();
  }
  return name;
}
