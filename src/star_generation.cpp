#include "star_generation.h"

using namespace kardeshev;
std::shared_ptr<Star> NaiveStarGenerator::generateStar() const
{
  srand(time(nullptr));
  auto info               = std::make_shared<StarInfo>();
  StarClass star_class    = m_classes[rand() % m_classes.size()];
  info->star_class        = star_class;
  std::shared_ptr<Star> s = std::make_shared<Star>(info);
  s->getInfo()->star      = s;

  return s;
}
