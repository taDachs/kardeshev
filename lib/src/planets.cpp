#include "lib/planets.h"
#include "lib/population.h"
#include "lib/astronomical_object_visitor.h"
#include <iostream>
using namespace kardeshev;
using namespace lib;

void Planet::visited(AstronomicalObjectVisitor& visitor) {
  visitor.visitPlanet(*this);
}
void Planet::update()
{
  for (auto& building : m_buildings)
  {
    building->satisfyProductionNeeds(m_stockpile);
    building->update();
    std::vector<ResourceSupply> produce = building->getProduce();
    for (const auto& prod : produce)
    {
      m_stockpile.addSupply(prod);
    }
  }
  size_t size = m_populations.size();
  std::vector<Population> alive_pops;
  for (auto& pop : m_populations)
  {
    pop.satisfyNeeds(m_stockpile);
    pop.update();
    if (pop.canReproduce())
    {
      alive_pops.push_back(pop.reproduce());
    }
    if (!pop.isDead())
    {
      alive_pops.push_back(pop);
    }
  }
  m_populations = alive_pops;
  // for (const auto& supply : m_stockpile.getSupplies())
  // {
  //   std::cout << supply.getType().getName() << " : " << supply.getAmount() << std::endl;
  // }
}
