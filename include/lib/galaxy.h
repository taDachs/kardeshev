#ifndef GALAXY_H
#define GALAXY_H

#include "lib/id.h"
#include "lib/solar_systems.h"
namespace kardeshev {

class Galaxy : public GameObject
{
private:
  std::vector<std::shared_ptr<SolarSystem> > m_systems;

public:
  Galaxy() = default;
  Galaxy(std::vector<std::shared_ptr<SolarSystem> > systems)
    : m_systems(std::move(systems))
  {
  }
  std::vector<std::shared_ptr<SolarSystem> > getSystems() const { return m_systems; }
  void update() override
  {
    for (auto& s : m_systems)
    {
      s->update();
    }
  }
};
} // namespace kardeshev

#endif // !GALAXY_H
