#include "generation/solar_system_generation.h"
#include "util/util.h"
#include <iostream>

using namespace kardeshev;
using namespace generation;
std::shared_ptr<lib::SolarSystem> NaiveSolarSystemGenerator::generateSolarSystem() const
{
  auto info                       = std::make_shared<lib::SolarSystemInfo>();
  info->name                      = m_name_generator->generateName();
  std::shared_ptr<lib::Star> star = m_star_generator->generateStar();

  int num_planets = util::RandomDistribution::sample(m_min_planets, m_max_planets);

  std::vector<std::shared_ptr<lib::Planet> > planets(num_planets);
  for (int i = 0; i < num_planets; ++i)
  {
    planets[i] = m_planet_generator->generatePlanet();
  }

  auto solar_system  = std::make_shared<lib::SolarSystem>(info, star, planets);
  info->solar_system = solar_system;
  return solar_system;
}
