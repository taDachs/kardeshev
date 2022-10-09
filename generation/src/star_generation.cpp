#include "generation/star_generation.h"
#include "util/util.h"

using namespace kardeshev;
using namespace generation;

std::shared_ptr<lib::Star> NaiveStarGenerator::generateStar() const
{
  std::shared_ptr<lib::Star> s = std::make_shared<lib::Star>();
  s->setName(m_name_generator->generateName());
  s->setMass(util::RandomDistribution::sample(1e5, 1e9));
  s->setStarClass(m_classes.at(util::RandomDistribution::sample(0, m_classes.size())));
  s->setOrbitDuration(lib::Duration(0));
  s->setOrbitDistance(lib::Distance(0));

  return s;
}
