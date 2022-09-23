#ifndef GAME_H
#define GAME_H

#include "duration.h"
namespace kardeshev {
class Game {
private:
  Duration m_time;
public:
  Duration getTime() const { return m_time; }
  void step(Duration step) { m_time.add(step.getTicks()); }
};
}
#endif // !GAME_H
