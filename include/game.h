#ifndef GAME_H
#define GAME_H

#include "duration.h"
#include "solar_systems.h"
#include <memory.h>

namespace kardeshev {
class Game {
private:
  Duration m_time;
  std::vector<std::shared_ptr<SolarSystem>> m_systems;
public:
  Duration getTime() const { return m_time; }
  void step(Duration step) {
    m_time.add(step.getTicks());
    for (auto& s : m_systems) {
      s->update();
    }
  }
  void addSystem(const std::shared_ptr<SolarSystem>& system) { m_systems.push_back(system); }
};
}
#endif // !GAME_H
