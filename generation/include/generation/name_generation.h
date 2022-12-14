#ifndef NAME_GENERATION_H
#define NAME_GENERATION_H

#include "lib/generators.h"
#include "util/util.h"
#include <memory>
#include <string>
#include <vector>
namespace kardeshev {
namespace generation {

class TokenList
{
public:
  using Ptr = std::shared_ptr<TokenList>;

private:
  std::vector<std::string> m_token_list;
  bool m_unqiue = false;

public:
  void addToken(const std::string& s) { m_token_list.push_back(s); }
  // void addWord(const std::string& w, int token_length = 3)
  // {
  //   for (int i = 0; i < w.length(); i += token_length)
  //   {
  //     m_token_list.push_back(w.substr(i, i + token_length));
  //   }
  // }
  std::string getRandomToken()
  {
    if (!m_unqiue)
    {
      std::sort(m_token_list.begin(), m_token_list.end());
      m_token_list.erase(unique(m_token_list.begin(), m_token_list.end()), m_token_list.end());
      m_unqiue = true;
    }
    return m_token_list.at(util::RandomDistribution::sample(0, m_token_list.size() - 1));
  }
};

const std::vector<char> VOWELS     = {'a', 'e', 'i', 'o', 'u'};
const std::vector<char> CONSONANTS = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n',
                                      'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};
class NaiveNameGenerator : public NameGenerator
{
public:
  using Ptr = std::shared_ptr<NaiveNameGenerator>;

private:
  int m_min_length         = 3;
  int m_max_length         = 15;
  int m_max_num_consonants = 2;
  int m_max_num_vowels     = 2;


public:
  NaiveNameGenerator(int min_length         = 3,
                     int max_length         = 15,
                     int max_num_consonants = 2,
                     int max_num_vowels     = 2)
    : m_min_length(min_length)
    , m_max_length(max_length)
    , m_max_num_consonants(max_num_consonants)
    , m_max_num_vowels(max_num_vowels)
  {
  }

  std::string generateName() const override;
};

class TokenListNameGenerator : public NameGenerator
{
private:
  TokenList::Ptr m_tokens;
  int m_min_tokens;
  int m_max_tokens;

public:
  TokenListNameGenerator(TokenList::Ptr tokens, int min_tokens = 3, int max_tokens = 5)
    : m_tokens(std::move(tokens))
    , m_min_tokens(min_tokens)
    , m_max_tokens(max_tokens)
  {
  }
  std::string generateName() const override;
};

} // namespace generation
} // namespace kardeshev


#endif // !NAME_GENERATION_H
