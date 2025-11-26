#pragma once
#include "common.h"

struct WeightedInt
{
  std::vector<int> v;
  std::vector<unsigned long long> pref; 
  RNG rng;

  WeightedInt(const std::vector<int> &vals,
              const std::vector<unsigned long long> &f)
  {
    if (vals.size() != f.size() || vals.empty())
      throw std::invalid_argument("bad sizes");
    v = vals;
    pref.resize(f.size());
    unsigned long long s = 0;
    for (size_t i = 0; i < f.size(); ++i)
    {
      if (f[i] == 0)
        throw std::invalid_argument("zero freq");
      s += f[i];
      pref[i] = s;
    }
    if (s == 0)
      throw std::invalid_argument("sum freq zero");
  }

  int operator()()
  {
    std::uniform_int_distribution<unsigned long long> d(1, pref.back());
    unsigned long long x = d(rng.gen);
    size_t i = std::lower_bound(pref.begin(), pref.end(), x) - pref.begin();
    return v[i];
  }
};
