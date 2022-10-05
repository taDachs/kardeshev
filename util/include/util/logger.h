#ifndef KARDESHEV_LOGGER_H
#define KARDESHEV_LOGGER_H

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace kardeshev {
namespace util {

class LoggerOutput
{
private:
public:
  using Ptr                                   = std::shared_ptr<LoggerOutput>;
  virtual void logInfo(const std::string& s)  = 0;
  virtual void logError(const std::string& s) = 0;
  virtual void logDebug(const std::string& s) = 0;
};

class Logger
{
public:
  using Ptr = std::shared_ptr<Logger>;

private:
  std::vector<LoggerOutput::Ptr> m_logger;

public:
  void logInfo(const std::string& s)
  {
    std::for_each(m_logger.begin(), m_logger.end(), [&](auto& log) { log->logInfo(s); });
  }
  void logError(const std::string& s)
  {
    std::for_each(m_logger.begin(), m_logger.end(), [&](auto& log) { log->logError(s); });
  }
  void logDebug(const std::string& s)
  {
    std::for_each(m_logger.begin(), m_logger.end(), [&](auto& log) { log->logDebug(s); });
  }
  void addLogger(LoggerOutput::Ptr l) { m_logger.push_back(l); }
};

class StdOutLogger : public LoggerOutput
{
private:
  static std::string getStamp();

public:
  void logInfo(const std::string& s) final
  {
    std::string prefix = "[" + getStamp() + "][INFO]: " + s;
    std::cout << prefix << std::endl;
  }
  void logError(const std::string& s) final
  {
    std::string prefix = "[" + getStamp() + "][ERROR]: " + s;
    std::cout << prefix << std::endl;
  }
  void logDebug(const std::string& s) final
  {
    std::string prefix = "[" + getStamp() + "][DEBUG]: " + s;
    std::cout << prefix << std::endl;
  }
};

} // namespace util
} // namespace kardeshev
#endif // !KARDESHEV_LOGGER_H
