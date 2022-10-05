#ifndef SOLAR_SYSTEM_GENERATION_H
#define SOLAR_SYSTEM_GENERATION_H

#include "lib/generators.h"
#include "lib/solar_systems.h"
#include <memory>

namespace kardeshev {
namespace generation {

class NaiveSolarSystemGenerator : public SolarSystemGenerator
{
private:
  std::shared_ptr<PlanetGenerator> m_planet_generator;
  std::shared_ptr<StarGenerator> m_star_generator;
  int m_min_planets;
  int m_max_planets;
  NameGenerator::Ptr m_name_generator;

public:
  NaiveSolarSystemGenerator(NameGenerator::Ptr name_generator,
                            std::shared_ptr<PlanetGenerator> planet_generator,
                            std::shared_ptr<StarGenerator> star_generator,
                            int min_planets,
                            int max_planets)
    : m_name_generator(std::move(name_generator))
    , m_planet_generator(std::move(planet_generator))
    , m_star_generator(std::move(star_generator))
    , m_min_planets(min_planets)
    , m_max_planets(max_planets)
  {
  }
  std::shared_ptr<lib::SolarSystem> generateSolarSystem() const override;
};

} // namespace generation
} // namespace kardeshev

#endif //! SOLAR_SYSTEM_GENERATION_H
