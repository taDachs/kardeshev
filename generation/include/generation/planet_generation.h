#ifndef PLANET_GENERATION_H
#define PLANET_GENERATION_H

#include <utility>

#include "lib/planets.h"

struct PlanetEnvironemnt
{
};
namespace kardeshev {
class PlanetGenerator
{
public:
  using Ptr                                              = std::shared_ptr<PlanetGenerator>;
  virtual std::shared_ptr<Planet> generatePlanet() const = 0;
};

class NaivePlanetGenerator : public PlanetGenerator
{
private:
  std::vector<PlanetClass> m_classes;

public:
  NaivePlanetGenerator(std::vector<PlanetClass> classes)
    : m_classes(std::move(classes))
  {
  }
  std::shared_ptr<Planet> generatePlanet() const override;
};
} // namespace kardeshev
#endif // !PLANET_GENERATION_H
