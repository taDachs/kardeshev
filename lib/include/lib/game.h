#ifndef GAME_H
#define GAME_H

#include "lib/units.h"
#include "lib/galaxy.h"
#include "lib/solar_systems.h"
#include <memory>

namespace kardeshev {
namespace lib {
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
    m_time.add(step.getDays());
    m_galaxy->update();
  }
  void setGalaxy(const std::shared_ptr<Galaxy>& galaxy) { m_galaxy = galaxy; }
  Galaxy::Ptr getGalaxy() const { return m_galaxy; }
};
} // namespace lib
} // namespace kardeshev
#endif // !GAME_H
