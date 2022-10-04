#include "generation/name_generation.h"
#include "util/util.h"
#include <algorithm>
#include <iostream>
using namespace kardeshev;

std::string NaiveNameGenerator::generateName() const
{
  std::string out;

  int length = RandomDistribution::sample(m_min_length, m_max_length + 1);

  bool vowel = RandomDistribution::sample(0, 2) == 1;
  int num    = 0;
  for (int i = 0; i < length; i++)
  {
    if (num == 0)
    {
      vowel = !vowel;
      num   = RandomDistribution::sample(1, (vowel ? m_max_num_vowels : m_max_num_consonants) + 1);
    }
    char next_char;
    if (vowel)
    {
      next_char = VOWELS[RandomDistribution::sample(0, VOWELS.size())];
    }
    else
    {
      next_char = CONSONANTS[RandomDistribution::sample(0, CONSONANTS.size())];
    }
    out += next_char;
    num--;
  }
  out[0] = toupper(out[0]);
  return out;
}

std::string TokenListNameGenerator::generateName() const {
  std::string out;
  int length = RandomDistribution::sample(m_min_tokens, m_max_tokens);
  for (int i = 0; i < length; i++) {
    std::string next = m_tokens->getRandomToken();
    if (!out.empty()) {
      char last = out.at(out.length() - 1);
      bool vowel = std::find(VOWELS.begin(), VOWELS.end(), last) != VOWELS.end();
      if (vowel) {
        while (std::find(CONSONANTS.begin(), CONSONANTS.end(), next[0]) == CONSONANTS.end()) {
          next = m_tokens->getRandomToken();
        }
      } else {
        while (std::find(VOWELS.begin(), VOWELS.end(), next[0]) == VOWELS.end()) {
          next = m_tokens->getRandomToken();
        }
      }
    }
    out += m_tokens->getRandomToken();
  }
  out[0] = toupper(out[0]);
  return out;
}
