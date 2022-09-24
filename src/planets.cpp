#include "planets.h"
#include "population.h"
#include <iostream>
using namespace kardeshev;

std::string PlanetInfo::getNameOrId() const
{
  if (name.empty())
  {
    return planet->getIdAsString();
  }
  return name;
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
  size_t size = m_pops.size();
  std::vector<Population> alive_pops;
  for (int i = 0; i < size; ++i)
  {
    m_pops[i].satisfyNeeds(m_stockpile);
    m_pops[i].update();
    if (m_pops[i].canReproduce())
    {
      alive_pops.push_back(m_pops[i].reproduce());
    }
    if (!m_pops[i].isDead())
    {
      alive_pops.push_back(m_pops[i]);
    }
  }
  m_pops = alive_pops;
  for (const auto& supply : m_stockpile.getSupplies())
  {
    /* std::cout << supply.getType().getName() << " : " << supply.getAmount() << std::endl; */
  }
}
