#include "generation/planet_generation.h"
#include "lib/planets.h"
#include "util/util.h"

using namespace kardeshev;
using namespace generation;

const double KELVIN_ZERO = 273;

std::shared_ptr<lib::Planet> NaivePlanetGenerator::generatePlanet() const
{
  std::shared_ptr<lib::Planet> p = std::make_shared<lib::Planet>();
  p->setName(m_name_generator->generateName());
  lib::PlanetClass planet_class = m_classes.at(rand() % m_classes.size());
  p->setPlanetClass(planet_class);
  double min_temp               = planet_class.getTempRange().first;
  double max_temp               = planet_class.getTempRange().second;
  p->setTemperature(static_cast<double>( util::RandomDistribution::sample(static_cast<int>(min_temp), static_cast<int>(max_temp))));
  const double astronomical_unit = 1.496e+8;
  auto orbit_distance = static_cast<double>(util::RandomDistribution::sample(10, 300) / 10.0);
  p->setOrbitDistance(lib::Distance(orbit_distance));
  p->setOrbitDuration(lib::Duration(orbit_distance * orbit_distance * util::RandomDistribution::sample(100, 300)));
  p->setMass(lib::Mass(util::RandomDistribution::sample(55, 317800) / 1000.0));

  return p;
}
