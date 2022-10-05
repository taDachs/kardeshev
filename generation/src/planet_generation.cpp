#include "generation/planet_generation.h"
#include "lib/duration.h"
#include "lib/planets.h"
#include "util/util.h"

using namespace kardeshev;
using namespace generation;

const double KELVIN_ZERO = 273;

std::shared_ptr<lib::Planet> NaivePlanetGenerator::generatePlanet() const
{
  auto info                     = std::make_shared<lib::PlanetInfo>();
  info->name                    = m_name_generator->generateName();
  lib::PlanetClass planet_class = m_classes[rand() % m_classes.size()];
  info->planet_class            = planet_class;
  double min_temp               = planet_class.getTempRange().first;
  double max_temp               = planet_class.getTempRange().second;
  info->temperature             = static_cast<double>(
    util::RandomDistribution::sample(static_cast<int>(min_temp), static_cast<int>(max_temp)));
  info->orbit_duration           = lib::Duration(util::RandomDistribution::sample(360, 9999));
  info->orbit_distance           = static_cast<double>(util::RandomDistribution::sample(100, 500));
  std::shared_ptr<lib::Planet> p = std::make_shared<lib::Planet>(info);
  p->getInfo()->planet           = p;

  return p;
}
