#include "util/util.h"
#include <cmath>

using namespace kardeshev;
using namespace util;

std::vector<std::string> kardeshev::util::splitString(std::string s, const std::string& delimiter)
{
  size_t pos = 0;
  std::string token;
  std::vector<std::string> out;
  while ((pos = s.find(delimiter)) != std::string::npos)
  {
    token = s.substr(0, pos);
    out.push_back(token);
    s.erase(0, pos + delimiter.length());
  }

  return out;
}

std::string kardeshev::util::joinLines(const std::vector<std::string>& lines,
                                       const std::string& delimiter,
                                       const std::string& prefix,
                                       const std::string& postfix)
{
  std::string out = prefix;
  bool first      = true;
  for (const auto& line : lines)
  {
    if (!first)
    {
      out += delimiter;
    }
    out += line;
    first = false;
  }
  out += prefix;
  return out;
}

long b32tol(std::string b32)
{
  return strtol(b32.c_str(), 0, 32);
}

// stolen from https://stackoverflow.com/questions/1916155/base32-conversion-in-c
std::string kardeshev::util::encodeBase32(long i)
{
  unsigned long u = *(reinterpret_cast<unsigned long*>(&i));
  std::string b32;

  do
  {
    int d = u % 32;
    if (d < 10)
    {
      b32.insert(0, 1, '0' + d);
    }
    else
    {
      b32.insert(0, 1, 'A' + d - 10);
    }

    u /= 32;

  } while (u > 0);

  return b32;
}

glm::vec2 kardeshev::util::polarToCart(const double radius, const double angle)
{
  return {radius * cos(angle), radius * sin(angle)};
}

glm::vec2 kardeshev::util::polarToCart(const double radius, const int angle)
{
  double radians = static_cast<double>(angle) * ((2 * M_PI) / 360.0);
  return polarToCart(radius, radians);
}
