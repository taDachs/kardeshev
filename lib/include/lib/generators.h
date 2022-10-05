#ifndef GENERATORS_H
#define GENERATORS_H
#include "lib/galaxy.h"
#include "lib/solar_systems.h"
#include "lib/stars.h"
#include <memory>

namespace kardeshev {
namespace generation {

class StarGenerator
{
public:
  using Ptr                                               = std::shared_ptr<StarGenerator>;
  virtual std::shared_ptr<lib::Star> generateStar() const = 0;
};

class SolarSystemGenerator
{
public:
  using Ptr = std::shared_ptr<SolarSystemGenerator>;
  virtual std::shared_ptr<lib::SolarSystem> generateSolarSystem() const = 0;
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
  using Ptr                                                   = std::shared_ptr<PlanetGenerator>;
  virtual std::shared_ptr<lib::Planet> generatePlanet() const = 0;
};

class GalaxyGenerator
{
public:
  using Ptr                                                   = std::shared_ptr<GalaxyGenerator>;
  virtual std::shared_ptr<lib::Galaxy> generateGalaxy() const = 0;
};

} // namespace generation
} // namespace kardeshev

#endif // !GENERATORS_H
