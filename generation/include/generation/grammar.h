#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace kardeshev {

class Symbol
{
public:
  using Ptr                        = std::shared_ptr<Symbol>;
  virtual int getMaxLength() const = 0;
  virtual int getMinLength() const = 0;
  virtual std::string toString(int min_length = -1, int max_length = -1) const = 0;
};

class Terminal : public Symbol
{
private:
  std::string m_val;

public:
  Terminal(std::string val)
    : m_val(std::move(val))
  {
  }
  int getMaxLength() const override { return m_val.length(); }
  int getMinLength() const override { return m_val.length(); }
  std::string toString(int min_length = -1, int max_length = -1) const override { return m_val; }
};

std::vector<Symbol::Ptr> stringToTerminals(const std::string s);
class NonTerminal : public Symbol
{
private:
  std::vector<std::vector<Symbol::Ptr> > m_productions;
  int m_min_length = INT32_MAX;
  int m_max_length = -INT32_MAX;

public:
  NonTerminal(std::vector<std::vector<Symbol::Ptr>> productions)
    : m_productions(std::move(productions))
  {
    for (const auto& p : m_productions)
    {
      int min = 0;
      int max = 0;
      for (const auto& c : p)
      {
        min += c->getMinLength();
        max += c->getMaxLength();
      }
      if (max > m_max_length)
      {
        m_max_length = max;
      }
      if (min < m_min_length)
      {
        m_min_length = min;
      }
    }
  }
  int getMaxLength() const override { return m_max_length; }
  int getMinLength() const override { return m_min_length; }
  std::string toString(int min_length = -1, int max_length = -1) const override;
};
} // namespace kardeshev

#endif // !GRAMMAR_H
