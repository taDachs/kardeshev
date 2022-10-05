#include "lib/buildings.h"

using namespace kardeshev;
using namespace lib;

void Farm::satisfyProductionNeeds(const ResourceList& supplies)
{
  m_production_needs_satisfied = true;
}

void Farm::update()
{
  if (!m_production_needs_satisfied)
  {
    m_produce.clear();
  }
  else
  {
    m_produce                    = {ResourceSupply(m_crop, 4)};
    m_production_needs_satisfied = false;
  }
}
