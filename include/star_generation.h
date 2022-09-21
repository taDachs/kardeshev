#ifndef STAR_GENERATION_H
#define STAR_GENERATION_H

#include "stars.h"
namespace kardeshev {
class StarGenerator
{
public:
  virtual std::shared_ptr<Star> generateStar() const = 0;
};

class NaiveStarGenerator : public StarGenerator
{
private:
  std::vector<StarClass> m_classes;

public:
  NaiveStarGenerator(std::vector<StarClass> classes)
    : m_classes(std::move(classes))
  {
  }
  std::shared_ptr<Star> generateStar() const override;
};

}

#endif //! STAR_GENERATION_H