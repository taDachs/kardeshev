#ifndef POPULATION_H
#define POPULATION_H

#include <string>
#include <utility>
#include <vector>

#include "resources.h"

namespace kardeshev {

class Species
{
private:
  std::string m_name;

public:
  Species(std::string name)
    : m_name(std::move(name))
  {
  }
  std::string getName() const { return m_name; }
};

class Need
{
private:
  ResourceType m_type;

public:
  Need(ResourceType type)
    : m_type(std::move(type))
  {
  }
  ResourceType getType() const { return m_type; }
};

class Population
{
private:
  std::vector<Need> m_needs;
  unsigned long m_size;

public:
  Population(std::vector<Need> needs, unsigned long size)
    : m_needs(std::move(needs))
    , m_size(size){};

  std::vector<Need> getNeeds() const { return m_needs; }
  unsigned long getSize() const { return m_size; }
};
} // namespace kardeshev


#endif // !POPULATION_H
