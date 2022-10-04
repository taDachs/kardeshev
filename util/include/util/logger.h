#ifndef KARDESHEV_LOGGER_H
#define KARDESHEV_LOGGER_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>

namespace kardeshev {

class LoggerOutput {
private:
public:
  using Ptr = std::shared_ptr<LoggerOutput>;
  virtual void logInfo(const std::string& s) = 0;
  virtual void logError(const std::string& s) = 0;
  virtual void logDebug(const std::string& s) = 0;
};

class Logger {
public:
  using Ptr = std::shared_ptr<Logger>;
private:
  std::vector<LoggerOutput::Ptr> m_logger;
  static std::string getStamp();
public:
  void logInfo(const std::string& s) {
    std::string prefix = "[" + getStamp() + "][INFO]: " + s;
    std::for_each(m_logger.begin(), m_logger.end(), [&](auto& log) { log->logInfo(prefix); });
  }
  void logError(const std::string& s) {
    std::string prefix = "[" + getStamp() + "][ERROR]: " + s;
    std::for_each(m_logger.begin(), m_logger.end(), [&](auto& log) { log->logError(prefix); });
  }
  void logDebug(const std::string& s) {
    std::string prefix = "[" + getStamp() + "][DEBUG]: " + s;
    std::for_each(m_logger.begin(), m_logger.end(), [&](auto& log) { log->logDebug(prefix); });
  }
  void addLogger(LoggerOutput::Ptr l) { m_logger.push_back(l); }
};

class StdOutLogger : public LoggerOutput {
  void logInfo(const std::string& s) final {
    std::cout << s << std::endl;
  }
  void logError(const std::string& s) final {
    std::cout << s << std::endl;
  }
  void logDebug(const std::string& s) final {
    std::cout << s << std::endl;
  }
};

}
#endif // !KARDESHEV_LOGGER_H
