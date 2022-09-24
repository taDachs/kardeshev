#include "galaxy.h"
#include "galaxy_generation.h"
#include "solar_systems.h"
#include "util.h"

using namespace kardeshev;

std::shared_ptr<Galaxy> NaiveGalaxyGenerator::generateGalaxy() const {
  std::vector<std::shared_ptr<SolarSystem>> systems;
  int num_systems = RandomDistribution::sample(m_min_systems, m_max_systems);
  systems.reserve(num_systems);
  for (int i = 0; i < num_systems; ++i) {
    systems.push_back(m_system_generator->generateSolarSystem());
    systems[i]->getInfo()->pos.x = static_cast<float>(NormalDistribution::sample(0, 100));
    systems[i]->getInfo()->pos.y = static_cast<float>(NormalDistribution::sample(0, 100));
  }

  return std::make_shared<Galaxy>(systems);
}
