#ifndef GALAXY_GENERATION_H
#define GALAXY_GENERATION_H

#include "generation/solar_system_generation.h"
#include "lib/galaxy.h"
#include <memory>

namespace kardeshev {

class GalaxyGenerator
{
public:
  using Ptr = std::shared_ptr<GalaxyGenerator>;
  virtual std::shared_ptr<Galaxy> generateGalaxy() const = 0;
};

class NaiveGalaxyGenerator : public GalaxyGenerator
{
private:
  std::shared_ptr<SolarSystemGenerator> m_system_generator;
  int m_min_systems;
  int m_max_systems;

public:
  NaiveGalaxyGenerator(std::shared_ptr<SolarSystemGenerator> system_generator,
                       int min_systems,
                       int max_systems)
    : m_system_generator(system_generator)
    , m_min_systems(min_systems)
    , m_max_systems(max_systems)
  {
  }
  std::shared_ptr<Galaxy> generateGalaxy() const override;
};
} // namespace kardeshev

#endif // !GALAXY_GENERATION_H
