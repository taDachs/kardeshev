#ifndef STAR_GENERATION_H
#define STAR_GENERATION_H

#include "lib/generators.h"
#include "lib/stars.h"
namespace kardeshev {
namespace generation {


class NaiveStarGenerator : public StarGenerator
{
private:
  std::vector<lib::StarClass> m_classes;
  NameGenerator::Ptr m_name_generator;

public:
  NaiveStarGenerator(NameGenerator::Ptr name_generator, std::vector<lib::StarClass> classes)
    : m_name_generator(std::move(name_generator))
    , m_classes(std::move(classes))
  {
  }
  std::shared_ptr<lib::Star> generateStar() const override;
};

} // namespace generation
} // namespace kardeshev

#endif //! STAR_GENERATION_H
