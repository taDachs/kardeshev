#include "generation/galaxy_generation.h"
#include "lib/galaxy.h"
#include "lib/solar_systems.h"
#include "util/util.h"
#include <iostream>

using namespace kardeshev;
using namespace generation;

std::shared_ptr<lib::Galaxy> NaiveGalaxyGenerator::generateGalaxy() const
{
  std::vector<std::shared_ptr<lib::SolarSystem> > systems;
  int num_systems = util::RandomDistribution::sample(m_min_systems, m_max_systems);
  systems.reserve(num_systems);
  for (int i = 0; i < num_systems; ++i)
  {
    systems.push_back(m_system_generator->generateSolarSystem());
    systems[i]->getInfo()->pos.x = static_cast<float>(util::NormalDistribution::sample(0, 100));
    systems[i]->getInfo()->pos.y = static_cast<float>(util::NormalDistribution::sample(0, 100));
  }

  return std::make_shared<lib::Galaxy>(systems);
}
