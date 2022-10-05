#include "util/random_generators.h"

using namespace kardeshev;
using namespace util;

void Splitmix::discard(unsigned long long n)
{
  for (unsigned long long i = 0; i < n; ++i)
  {
    operator()();
  }
}
