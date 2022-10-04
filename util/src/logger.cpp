#include "util/logger.h"
#include <cstring>

using namespace kardeshev;
std::string Logger::getStamp()
{
  std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  return strtok(std::ctime(&time), "\n");
}
