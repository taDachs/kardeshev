#ifndef UTIL_H
#define UTIL_H

#include "glm/glm.hpp"
#include "random.h"
#include <cmath>
#include <random>
#include <string>
#include <vector>

std::vector<std::string> splitString(std::string s, const std::string& delimiter);
std::string joinLines(const std::vector<std::string>& lines,
                      const std::string& delimiter,
                      const std::string& prefix  = "",
                      const std::string& postfix = "");

std::string encodeBase32(long i);

glm::vec2 polarToCart(const double radius, const double angle);
glm::vec2 polarToCart(const double radius, const int angle);

class RandomDistribution
{
private:
public:
  static int sample(const int min, const int max)
  {
    std::random_device rd;
    Pcg rand(rd);
    std::uniform_int_distribution<> u(min, max - 1);
    return u(rand);
  }
};

class NormalDistribution
{
private:
public:
  static double sample(const double mean, const double variance)
  {
    std::random_device rd;
    Pcg rand(rd);
    std::uniform_int_distribution<> u(0, 1000);
    double u1 = static_cast<double>(u(rand)) / 1000.0;
    double u2 = static_cast<double>(u(rand)) / 1000.0;
    double z1 = std::sqrt(-2 * std::log(u1)) * std::cos(2 * M_PI * u2);
    return mean + z1 * variance;
  }
};


#endif // !UTIL_H
