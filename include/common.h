#pragma once
#include <bits/stdc++.h>

struct RNG
{
  std::mt19937_64 gen;
  RNG() : gen(std::random_device{}()) {}

  template <class It>
  void shuffle(It b, It e) { std::shuffle(b, e, gen); }

  size_t pick(size_t n)
  {
    std::uniform_int_distribution<size_t> d(0, n - 1);
    return d(gen);
  }

  double uni01()
  {
    std::uniform_real_distribution<double> d(0.0, 1.0);
    return d(gen);
  }
};
