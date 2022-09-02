#include "art.h"
#include "ascii_planets.h"
#include <iomanip>
#include <string>
#include <cstdio>
#include <sstream>


using namespace kardeshev;

std::string TerminalVisualizer::drawPlanet(const Planet& planet) const
{
  std::shared_ptr<PlanetInfo> info = planet.getInfo();
  std::string art                  = m_map.at(info->planet_class.getKey());

  std::vector<std::string> lines = splitString(art, "\n");
  for (auto& line : lines)
  {
    line += " # ";
  }

  lines[0] += "\e[32mPlanet " + info->getNameOrId() + "\e[0m";
  lines[1] += "Id: " + planet.getIdAsString();
  std::ostringstream temp_stream;
  temp_stream << std::fixed;
  temp_stream << std::setprecision(2);
  temp_stream << "Temperature: " << (info->temperature - 273) << " C";
  lines[2] += temp_stream.str();
  lines[3] += "Class: " + info->planet_class.getName();
  lines[4] += "Description: " + info->planet_class.getDescription();


  return joinLines(lines, "\n");
}

std::string TerminalVisualizer::drawStar(const Star& star) const
{
  std::shared_ptr<StarInfo> info = star.getInfo();
  std::string art                  = m_map.at(info->star_class.getKey());

  std::vector<std::string> lines = splitString(art, "\n");
  for (auto& line : lines)
  {
    line += " # ";
  }

  lines[0] += "\e[32mStar " + info->getNameOrId() + "\e[0m";
  lines[1] += "Id: " + star.getIdAsString();
  std::ostringstream temp_stream;
  temp_stream << std::fixed;
  temp_stream << std::setprecision(2);
  temp_stream << "Temperature: " << (info->star_class.getTemp() - 273) << " C";
  lines[2] += temp_stream.str();
  lines[3] += "Class: " + info->star_class.getName();
  lines[4] += "Description: " + info->star_class.getDescription();


  return joinLines(lines, "\n");
}
