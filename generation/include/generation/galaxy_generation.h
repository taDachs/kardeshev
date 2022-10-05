#ifndef GALAXY_GENERATION_H
#define GALAXY_GENERATION_H

#include "lib/galaxy.h"
#include "lib/generators.h"
#include <memory>

namespace kardeshev {
namespace generation {

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
  std::shared_ptr<lib::Galaxy> generateGalaxy() const override;
};
} // namespace generation
} // namespace kardeshev

#endif // !GALAXY_GENERATION_H
