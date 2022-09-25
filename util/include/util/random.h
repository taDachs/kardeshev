/* Copyright (c) 2018 Arvid Gerstmann. */
/* This code is licensed under MIT license. */
#ifndef AG_RANDOM_H
#define AG_RANDOM_H

#include <cstdint>
#include <random>
class Splitmix
{
public:
  using result_type = uint32_t;
  static constexpr result_type(min)() { return 0; }
  static constexpr result_type(max)() { return UINT32_MAX; }
  friend bool operator==(Splitmix const&, Splitmix const&);
  friend bool operator!=(Splitmix const&, Splitmix const&);

  Splitmix()
    : m_seed(1)
  {
  }
  explicit Splitmix(std::random_device& rd) { seed(rd); }

  void seed(std::random_device& rd) { m_seed = uint64_t(rd()) << 31 | uint64_t(rd()); }

  result_type operator()()
  {
    uint64_t z = (m_seed += UINT64_C(0x9E3779B97F4A7C15));
    z          = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z          = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return result_type((z ^ (z >> 31)) >> 31);
  }

  void discard(unsigned long long n)
  {
    for (unsigned long long i = 0; i < n; ++i)
    {
      operator()();
    }
  }

private:
  uint64_t m_seed;
};

class Xorshift
{
public:
  using result_type = uint32_t;
  static constexpr result_type(min)() { return 0; }
  static constexpr result_type(max)() { return UINT32_MAX; }
  friend bool operator==(Xorshift const&, Xorshift const&);
  friend bool operator!=(Xorshift const&, Xorshift const&);

  Xorshift()
    : m_seed(0xc1f651c67c62c6e0ULL)
  {
  }
  explicit Xorshift(std::random_device& rd) { seed(rd); }

  void seed(std::random_device& rd) { m_seed = uint64_t(rd()) << 31 | uint64_t(rd()); }

  result_type operator()()
  {
    uint64_t result = m_seed * 0xd989bcacc137dcd5ULL;
    m_seed ^= m_seed >> 11;
    m_seed ^= m_seed << 31;
    m_seed ^= m_seed >> 18;
    return uint32_t(result >> 32ULL);
  }

  void discard(unsigned long long n)
  {
    for (unsigned long long i = 0; i < n; ++i)
    {
      operator()();
    }
  }

private:
  uint64_t m_seed;
};

class Pcg
{
public:
  using result_type = uint32_t;
  static constexpr result_type(min)() { return 0; }
  static constexpr result_type(max)() { return UINT32_MAX; }
  friend bool operator==(Pcg const&, Pcg const&);
  friend bool operator!=(Pcg const&, Pcg const&);

  Pcg()
    : m_state(0x853c49e6748fea9bULL)
    , m_inc(0xda3e39cb94b95bdbULL)
  {
  }
  explicit Pcg(std::random_device& rd) { seed(rd); }

  void seed(std::random_device& rd)
  {
    uint64_t s0 = uint64_t(rd()) << 31 | uint64_t(rd());
    uint64_t s1 = uint64_t(rd()) << 31 | uint64_t(rd());

    m_state = 0;
    m_inc   = (s1 << 1) | 1;
    (void)operator()();
    m_state += s0;
    (void)operator()();
  }

  result_type operator()()
  {
    uint64_t oldstate = m_state;
    m_state           = oldstate * 6364136223846793005ULL + m_inc;
    auto xorshifted   = uint32_t(((oldstate >> 18U) ^ oldstate) >> 27U);
    int rot           = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
  }

  void discard(unsigned long long n)
  {
    for (unsigned long long i = 0; i < n; ++i)
    {
      operator()();
    }
  }

private:
  uint64_t m_state;
  uint64_t m_inc;
};
#endif /* AG_RANDOM_H */
