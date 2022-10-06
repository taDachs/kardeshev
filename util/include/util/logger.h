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
private:
  static std::vector<LoggerOutput::Ptr> m_logger;
  static std::string m_last_info;
  static std::string m_last_error;
  static std::string m_last_debug;
  static std::string m_last;
  Logger() = default;

public:
  static void logInfo(const std::string& s)
  {
    m_last_info = s;
    m_last = s;
    std::for_each(m_logger.begin(), m_logger.end(), [&](auto& log) { log->logInfo(s); });
  }
  static void logError(const std::string& s)
  {
    m_last_error = s;
    m_last = s;
    std::for_each(m_logger.begin(), m_logger.end(), [&](auto& log) { log->logError(s); });
  }
  static void logDebug(const std::string& s)
  {
    m_last_debug = s;
    m_last = s;
    std::for_each(m_logger.begin(), m_logger.end(), [&](auto& log) { log->logDebug(s); });
  }
  static void addLogger(const LoggerOutput::Ptr& l) { m_logger.push_back(l); }

  static std::string getLastInfo() { return m_last_info; }
  static std::string getLastError() { return m_last_error; }
  static std::string getLastDebug() { return m_last_debug; }
  static std::string getLast() { return m_last; }
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
