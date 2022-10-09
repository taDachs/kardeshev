#include "generation/solar_system_generation.h"
#include "util/util.h"
#include "util/logger.h"
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
    planets.at(i) = m_planet_generator->generatePlanet();
    planets.at(i)->setParent(std::static_pointer_cast<lib::AstronomicalObject>(star));
    while ((util::RandomDistribution::sample(0, 100) / 100.0) < m_moon_prob) {
      lib::Planet::Ptr moon = m_planet_generator->generatePlanet();
      util::Logger::logDebug("Generated Moon");
      moon->setOrbitDistance(lib::Distance(util::RandomDistribution::sample(1, 500) / 1000.0));
      moon->setOrbitDuration(lib::Duration(util::RandomDistribution::sample(10, 500)));
      moon->setMass(lib::Mass(util::RandomDistribution::sample(1, 500) / 1000.0));
      moon->setParent(planets.at(i));
      planets.push_back(moon);
    }
  }

  auto solar_system  = std::make_shared<lib::SolarSystem>(info, star, planets);
  info->solar_system = solar_system;
  return solar_system;
}
