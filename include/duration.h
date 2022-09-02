#ifndef DURATION_H
#define DURATION_H

namespace kardeshev {
class Duration
{
private:
  int m_ticks;

public:
  Duration(int ticks)
    : m_ticks(ticks)
  {
  }
  int getTicks() const { return m_ticks; }
};
} // namespace kardeshev

#endif // !DURATION_H
