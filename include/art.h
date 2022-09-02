#ifndef ART_H
#define ART_H

#include "planets.h"
#include "util.h"
#include <map>
#include <string>
#include <utility>
#include "stars.h"

namespace kardeshev {
class TerminalVisualizer
{
private:
  std::map<std::string, std::string> m_map;

public:
  TerminalVisualizer(std::map<std::string, std::string> map)
    : m_map(std::move(map))
  {
  }
  std::string drawPlanet(const Planet& planet) const;
  std::string drawStar(const Star& star) const;
};
} // namespace kardeshev

#endif // !ART_H
