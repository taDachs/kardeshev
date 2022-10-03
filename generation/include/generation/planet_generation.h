#ifndef PLANET_GENERATION_H
#define PLANET_GENERATION_H

#include <utility>

#include "generation/name_generation.h"
#include "lib/planets.h"
namespace kardeshev {

struct PlanetEnvironemnt
{
};
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
  NameGenerator::Ptr m_name_generator;

public:
  NaivePlanetGenerator(NameGenerator::Ptr name_generator, std::vector<PlanetClass> classes)
    : m_name_generator(std::move(name_generator))
    , m_classes(std::move(classes))
  {
  }
  std::shared_ptr<Planet> generatePlanet() const override;
};
} // namespace kardeshev
#endif // !PLANET_GENERATION_H
