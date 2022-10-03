#include "generation/name_generation.h"
#include "util/util.h"
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
      next_char = m_vowels[RandomDistribution::sample(0, m_vowels.size())];
    }
    else
    {
      next_char = m_consonants[RandomDistribution::sample(0, m_consonants.size())];
    }
    out += next_char;
    num--;
  }
  out[0] = toupper(out[0]);
  return out;
}
