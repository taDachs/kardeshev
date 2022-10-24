#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "id.h"
#include "resources.h"
#include <memory>

namespace kardeshev {
namespace lib {

class Building : public GameObject
{
public:
  using Ptr = std::shared_ptr<Building>;
  virtual void satisfyProductionNeeds(const ResourceList& supplies) = 0;
  virtual std::vector<ResourceSupply> getProduce()                  = 0;
};

class Farm : public Building
{
private:
  ResourceType m_crop;
  bool m_production_needs_satisfied;
  std::vector<ResourceSupply> m_produce;

public:
  Farm(ResourceType crop)
    : m_crop(std::move(crop))
  {
  }
  std::vector<ResourceSupply> getProduce() override { return m_produce; };
  void satisfyProductionNeeds(const ResourceList& supplies) override;
  void update() override;
};
} // namespace lib
} // namespace kardeshev
#endif // !BUILDINGS_H
