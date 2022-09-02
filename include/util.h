#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

std::vector<std::string> splitString(std::string s, const std::string& delimiter);
std::string joinLines(const std::vector<std::string>& lines,
                      const std::string& delimiter,
                      const std::string& prefix  = "",
                      const std::string& postfix = "");

std::string encodeBase32(long i);

#endif // !UTIL_H
