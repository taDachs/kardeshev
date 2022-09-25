#include "lib/population.h"
#include <iostream>
#include <map>

using namespace kardeshev;
void Population::update()
{
  if (m_basic_needs_satisfied && !m_dead)
  {
    m_reproduction_progress *= m_species.getReproductionRate();
  }
  else
  {
    m_reproduction_progress /= m_species.getReproductionRate();
  }
  if (m_reproduction_progress > REPRODUCTION_THRESH && !m_can_reproduce)
  {
    m_can_reproduce = (rand() % 1000) < (m_reproduction_progress - REPRODUCTION_THRESH);
  }
  if (m_reproduction_progress < DEATH_THRESH && !m_dead)
  {
    m_dead = ((rand() % 100) / 1000.0) < (DEATH_THRESH - m_reproduction_progress);
  }
  m_basic_needs_satisfied = false;
}

Population Population::reproduce()
{
  m_reproduction_progress = 1;
  m_can_reproduce         = false;
  return {m_species, m_needs};
}

void Population::satisfyNeeds(ResourceList& supplies)
{
  m_basic_needs_satisfied = true;
  for (auto const& need : m_species.getBasicNeeds())
  {
    Need left = supplies.provideNeed(need);
    if (left.getAmount() > 0)
    {
      m_basic_needs_satisfied = false;
    }
  }
}
