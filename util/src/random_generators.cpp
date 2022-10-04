#include "util/random_generators.h"

void Splitmix::discard(unsigned long long n) {

    for (unsigned long long i = 0; i < n; ++i)
    {
      operator()();
    }
}
