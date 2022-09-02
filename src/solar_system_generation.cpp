#include "solar_system_generation.h"

using namespace kardeshev;
std::shared_ptr<SolarSystem> NaiveSolarSystemGenerator::generateSolarSystem() const {

  srand(time(nullptr));
  auto info                  = std::make_shared<SolarSystemInfo>();
  std::shared_ptr<Star> star = m_star_generator->generateStar();
  int num_planets = (rand() % (m_max_planets - m_min_planets)) + m_min_planets;

  std::vector<std::shared_ptr<Planet>> planets(num_planets);
  for (int i = 0; i < num_planets; ++i) {
    planets[i] = m_planet_generator->generatePlanet();
  }

  auto solar_system = std::make_shared<SolarSystem>(info, star, planets);
  return solar_system;

}
