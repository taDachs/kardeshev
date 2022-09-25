#include "generation/planet_generation.h"
#include "lib/duration.h"
#include "lib/planets.h"
#include "util/util.h"

using namespace kardeshev;

const double KELVIN_ZERO = 273;

std::shared_ptr<Planet> NaivePlanetGenerator::generatePlanet() const
{
  auto info                = std::make_shared<PlanetInfo>();
  PlanetClass planet_class = m_classes[rand() % m_classes.size()];
  info->planet_class       = planet_class;
  double min_temp          = planet_class.getTempRange().first;
  double max_temp          = planet_class.getTempRange().second;
  info->temperature        = static_cast<double>(
    RandomDistribution::sample(static_cast<int>(min_temp), static_cast<int>(max_temp)));
  info->m_orbit_duration    = Duration(RandomDistribution::sample(360, 9999));
  std::shared_ptr<Planet> p = std::make_shared<Planet>(info);
  p->getInfo()->planet      = p;

  return p;
}
