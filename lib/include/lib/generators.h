#ifndef GENERATORS_H
#define GENERATORS_H
#include "lib/stars.h"
#include "lib/solar_systems.h"
#include <memory>

namespace kardeshev {

class StarGenerator
{
public:
  using Ptr                                          = std::shared_ptr<StarGenerator>;
  virtual std::shared_ptr<Star> generateStar() const = 0;
};

class SolarSystemGenerator
{
public:
  using Ptr = std::shared_ptr<SolarSystemGenerator>;
  virtual std::shared_ptr<SolarSystem> generateSolarSystem() const = 0;
};

class NameGenerator
{
public:
  using Ptr = std::shared_ptr<NameGenerator>;

  virtual std::string generateName() const = 0;
};

class PlanetGenerator
{
public:
  using Ptr                                              = std::shared_ptr<PlanetGenerator>;
  virtual std::shared_ptr<Planet> generatePlanet() const = 0;
};
}

#endif // !GENERATORS_H
