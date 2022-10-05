#include "generation/star_generation.h"
#include "util/util.h"
#include <iostream>

using namespace kardeshev;
using namespace generation;

std::shared_ptr<lib::Star> NaiveStarGenerator::generateStar() const
{
  auto info                    = std::make_shared<lib::StarInfo>();
  info->name                   = m_name_generator->generateName();
  int randval                  = util::RandomDistribution::sample(0, m_classes.size());
  lib::StarClass star_class    = m_classes[randval];
  info->star_class             = star_class;
  std::shared_ptr<lib::Star> s = std::make_shared<lib::Star>(info);
  s->getInfo()->star           = s;

  return s;
}
