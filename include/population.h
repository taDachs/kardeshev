#ifndef POPULATION_H
#define POPULATION_H

#include <string>
#include <utility>
#include <vector>

#include "resources.h"

namespace kardeshev {

#define REPRODUCTION_THRESH 1000
#define DEATH_THRESH 0.1


class Species : public GameClass
{
private:
  std::string m_name;
  double m_reproduction_rate;
  std::vector<Need> m_basic_needs;

public:
  Species(std::string key,
          std::string name,
          double reproduction_rate,
          std::vector<Need> basic_needs)
    : GameClass(std::move(key))
    , m_name(std::move(name))
    , m_reproduction_rate(reproduction_rate)
    , m_basic_needs(std::move(basic_needs))
  {
  }
  std::string getName() const { return m_name; }
  std::vector<Need> getBasicNeeds() const { return m_basic_needs; }
  double getReproductionRate() const { return m_reproduction_rate; }
};

class Population : public GameObject
{
private:
  std::vector<Need> m_needs;
  Species m_species;
  double m_reproduction_progress = 1;
  bool m_can_reproduce           = false;
  bool m_dead                    = false;
  bool m_basic_needs_satisfied   = false;

public:
  Population(Species species, std::vector<Need> needs)
    : m_species(std::move(species))
    , m_needs(std::move(needs)){};

  std::vector<Need> getNeeds() const { return m_needs; }
  Species getSpecies() const { return m_species; }
  double getReproductionProgress() const { return m_reproduction_progress; }
  bool canReproduce() const { return m_can_reproduce; }
  bool isDead() const { return m_dead; }
  void update() override;
  void satisfyNeeds(ResourceList& supplies);

  Population reproduce();
};
} // namespace kardeshev


#endif // !POPULATION_H
