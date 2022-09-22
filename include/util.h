#ifndef UTIL_H
#define UTIL_H

#include "glm/glm.hpp"
#include "random.h"
#include <string>
#include <vector>
#include <random>

std::vector<std::string> splitString(std::string s, const std::string& delimiter);
std::string joinLines(const std::vector<std::string>& lines,
                      const std::string& delimiter,
                      const std::string& prefix  = "",
                      const std::string& postfix = "");

std::string encodeBase32(long i);

glm::vec2 polarToCart(const double radius, const double angle);
glm::vec2 polarToCart(const double radius, const int angle);

class RandomDistribution {
private:

public:
  static int sample(const int min, const int max)  {
    std::random_device rd;
    pcg rand(rd);
    std::uniform_int_distribution<> u(min, max - 1);
    return u(rand);
  }
};

#endif // !UTIL_H
