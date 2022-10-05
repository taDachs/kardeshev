#ifndef DURATION_H
#define DURATION_H

using verylargenumber = unsigned long long int;

namespace kardeshev {
namespace lib {

class Duration
{
private:
  verylargenumber m_ticks;

public:
  Duration(verylargenumber ticks = 0)
    : m_ticks(ticks)
  {
  }
  verylargenumber getTicks() const { return m_ticks; }
  void add(verylargenumber a) { m_ticks += a; }
};
} // namespace lib
} // namespace kardeshev

#endif // !DURATION_H
