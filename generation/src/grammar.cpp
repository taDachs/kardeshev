#include "generation/grammar.h"
#include "util/util.h"

using namespace kardeshev;

std::string NonTerminal::toString(int min_length, int max_length) const
{
  std::vector<std::vector<Symbol::Ptr> > allowed;
  for (const auto& p : m_productions)
  {
    int min = 0;
    int max = 0;
    for (const auto& c : p)
    {
      min += c->getMinLength();
      max += c->getMaxLength();
    }
    if ((min_length <= 0 || min >= min_length) && ((max_length <= 0 || max <= max_length)))
    {
      allowed.push_back(p);
    }
  }
  std::string out;
  std::vector<Symbol::Ptr> production =
    m_productions.at(RandomDistribution::sample(0, allowed.size()));
  // m_productions.at(0);

  for (const auto& c : production)
  {
    std::string res = c->toString(min_length, max_length);
    min_length -= res.size();
    max_length -= res.size();
    out += res;
  }
  return out;
}

std::vector<Symbol::Ptr> kardeshev::stringToTerminals(const std::string s)
{
  std::vector<Symbol::Ptr> out;
  for (const auto& c : s)
  {
    out.push_back(std::make_shared<Terminal>(std::to_string(c)));
  }
  return out;
}
