#ifndef PLANET_GENERATION_H
#define PLANET_GENERATION_H

#include <utility>

#include "lib/generators.h"
#include "lib/planets.h"
namespace kardeshev {
namespace generation {

struct PlanetEnvironemnt
{
};


class NaivePlanetGenerator : public PlanetGenerator
{
private:
  std::vector<lib::PlanetClass> m_classes;
  NameGenerator::Ptr m_name_generator;

public:
  NaivePlanetGenerator(NameGenerator::Ptr name_generator, std::vector<lib::PlanetClass> classes)
    : m_name_generator(std::move(name_generator))
    , m_classes(std::move(classes))
  {
  }
  std::shared_ptr<lib::Planet> generatePlanet() const override;
};
} // namespace generation
} // namespace kardeshev
#endif // !PLANET_GENERATION_H
