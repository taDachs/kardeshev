#include "solar_system_generation.h"
#include "util.h"
#include <iostream>

using namespace kardeshev;
std::shared_ptr<SolarSystem> NaiveSolarSystemGenerator::generateSolarSystem() const
{
  auto info                  = std::make_shared<SolarSystemInfo>();
  std::shared_ptr<Star> star = m_star_generator->generateStar();

  int num_planets = RandomDistribution::sample(m_min_planets, m_max_planets);

  std::vector<std::shared_ptr<Planet> > planets(num_planets);
  for (int i = 0; i < num_planets; ++i)
  {
    planets[i] = m_planet_generator->generatePlanet();
  }

  auto solar_system  = std::make_shared<SolarSystem>(info, star, planets);
  info->solar_system = solar_system;
  return solar_system;
}
