#ifndef NAME_GENERATION_H
#define NAME_GENERATION_H

#include <memory>
#include <string>
#include <vector>
namespace kardeshev {

class NameGenerator
{
public:
  using Ptr = std::shared_ptr<NameGenerator>;

  virtual std::string generateName() const = 0;
};

class NaiveNameGenerator : public NameGenerator
{
public:
  using Ptr = std::shared_ptr<NaiveNameGenerator>;

private:
  int m_min_length         = 3;
  int m_max_length         = 15;
  int m_max_num_consonants = 2;
  int m_max_num_vowels     = 2;

  const std::vector<char> m_vowels     = {'a', 'e', 'i', 'o', 'u'};
  const std::vector<char> m_consonants = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n',
                                          'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};

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

} // namespace kardeshev


#endif // !NAME_GENERATION_H
