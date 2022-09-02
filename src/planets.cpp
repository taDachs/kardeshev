#include "planets.h"
using namespace kardeshev;

std::string PlanetInfo::getNameOrId() const
{
  if (name.empty())
  {
    return planet->getIdAsString();
  }
  return name;
}
