#include "generation/star_generation.h"
#include "util/util.h"
#include <iostream>

using namespace kardeshev;
std::shared_ptr<Star> NaiveStarGenerator::generateStar() const
{
  auto info               = std::make_shared<StarInfo>();
  int randval             = RandomDistribution::sample(0, m_classes.size());
  StarClass star_class    = m_classes[randval];
  info->star_class        = star_class;
  std::shared_ptr<Star> s = std::make_shared<Star>(info);
  s->getInfo()->star      = s;

  return s;
}
