#include "planet_generation.h"
#include "planets.h"

using namespace kardeshev;

const double KELVIN_ZERO = 273;

std::shared_ptr<Planet> NaivePlanetGenerator::generatePlanet() const
{
  srand(time(nullptr));
  auto info                = std::make_shared<PlanetInfo>();
  PlanetClass planet_class = m_classes[rand() % m_classes.size()];
  info->planet_class       = planet_class;
  double min_temp          = planet_class.getTempRange().first;
  double max_temp          = planet_class.getTempRange().second;
  info->temperature =
    static_cast<double>(rand() % static_cast<int>(max_temp - min_temp) + min_temp);
  std::shared_ptr<Planet> p = std::make_shared<Planet>(info);
  p->getInfo()->planet      = p;

  return p;
}
