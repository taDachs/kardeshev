#ifndef ASTEROID_GENERATION_H
#define ASTEROID_GENERATION_H

#include "lib/asteroid.h"
#include "lib/generators.h"
#include "util/util.h"
namespace kardeshev {
namespace generation {

int count = 0;

class NaiveAsteroidGenerator : public AsteroidGenerator
{
private:
  lib::Distance m_orbit;
public:
  NaiveAsteroidGenerator(const lib::Distance& orbit): m_orbit(orbit) {}
  std::shared_ptr<lib::Asteroid> generateAsteroid() const override {
    auto a = std::make_shared<lib::Asteroid>();
    double offset = util::NormalDistribution::sample(0, 0.1);
    lib::Distance orbit(m_orbit.getInAU() + offset);
    a->setOrbitDistance(orbit);
    double angle = util::RandomDistribution::sample(0, 3600) / 10.0;
    a->setAngle(angle);
    a->setMass(lib::Mass(util::RandomDistribution::sample(10, 500) / 1000.0));
    a->setName(std::to_string(count));
    count++;

    return a;
  }
};

} // namespace generation
} // namespace kardeshev

#endif // !ASTEROID_GENERATION_H
