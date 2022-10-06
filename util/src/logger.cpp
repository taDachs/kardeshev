#include "util/logger.h"
#include <cstring>

using namespace kardeshev;
using namespace util;

std::vector<LoggerOutput::Ptr> Logger::m_logger;
std::string Logger::m_last_info;
std::string Logger::m_last_error;
std::string Logger::m_last_debug;
std::string Logger::m_last;

std::string StdOutLogger::getStamp()
{
  std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  return strtok(std::ctime(&time), "\n");
}
