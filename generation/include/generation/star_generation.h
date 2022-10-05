#ifndef STAR_GENERATION_H
#define STAR_GENERATION_H

#include "lib/generators.h"
#include "lib/stars.h"
namespace kardeshev {


class NaiveStarGenerator : public StarGenerator
{
private:
  std::vector<StarClass> m_classes;
  NameGenerator::Ptr m_name_generator;

public:
  NaiveStarGenerator(NameGenerator::Ptr name_generator, std::vector<StarClass> classes)
    : m_name_generator(std::move(name_generator))
    , m_classes(std::move(classes))
  {
  }
  std::shared_ptr<Star> generateStar() const override;
};

} // namespace kardeshev

#endif //! STAR_GENERATION_H
