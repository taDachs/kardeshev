#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "lib/id.h"
#include "lib/resources.h"

namespace kardeshev {
class Building : public GameObject
{
public:
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
} // namespace kardeshev
#endif // !BUILDINGS_H
