#include "lib/solar_systems.h"

using namespace kardeshev;
using namespace lib;

std::string SolarSystemInfo::getNameOrId() const
{
  if (name.empty())
  {
    return solar_system->getIdAsString();
  }
  return name;
}

void SolarSystem::update()
{
  for (auto& p : m_planets)
  {
    p->update();
  }
}
