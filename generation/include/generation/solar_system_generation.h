#ifndef SOLAR_SYSTEM_GENERATION_H
#define SOLAR_SYSTEM_GENERATION_H

#include "generation/planet_generation.h"
#include "generation/star_generation.h"
#include "lib/solar_systems.h"
#include <memory>
namespace kardeshev {
class SolarSystemGenerator
{
public:
  virtual std::shared_ptr<SolarSystem> generateSolarSystem() const = 0;
};

class NaiveSolarSystemGenerator : public SolarSystemGenerator
{
private:
  std::shared_ptr<PlanetGenerator> m_planet_generator;
  std::shared_ptr<StarGenerator> m_star_generator;
  int m_min_planets;
  int m_max_planets;

public:
  NaiveSolarSystemGenerator(std::shared_ptr<PlanetGenerator> planet_generator,
                            std::shared_ptr<StarGenerator> star_generator,
                            int min_planets,
                            int max_planets)
    : m_planet_generator(std::move(planet_generator))
    , m_star_generator(std::move(star_generator))
    , m_min_planets(min_planets)
    , m_max_planets(max_planets)
  {
  }
  std::shared_ptr<SolarSystem> generateSolarSystem() const override;
};

} // namespace kardeshev

#endif //! SOLAR_SYSTEM_GENERATION_H
