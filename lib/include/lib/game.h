#ifndef GAME_H
#define GAME_H

#include "duration.h"
#include "galaxy.h"
#include "solar_systems.h"
#include <memory.h>

namespace kardeshev {
class Game
{
public:
  using Ptr = std::shared_ptr<Game>;
private:
  Duration m_time;
  Galaxy::Ptr m_galaxy;

public:
  Duration getTime() const { return m_time; }
  void step(Duration step)
  {
    m_time.add(step.getTicks());
    m_galaxy->update();
  }
  void setGalaxy(const std::shared_ptr<Galaxy>& galaxy) { m_galaxy = galaxy; }
  Galaxy::Ptr getGalaxy() const { return m_galaxy; }
};
} // namespace kardeshev
#endif // !GAME_H
