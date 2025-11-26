#pragma once
#include "common.h"

struct Domino
{
  int a, b;
}; 

struct DominoDealer
{
  RNG rng;
  std::vector<Domino> pool;

  void reset(int n)
  {
    pool.clear();
    for (int i = 0; i <= n; ++i)
      for (int j = i; j <= n; ++j)
        pool.push_back({i, j});
  }

 
  bool take_matching(int left, int right, Domino &out, bool &attach_left)
  {
    std::vector<size_t> idx;
    for (size_t k = 0; k < pool.size(); ++k)
    {
      auto [a, b] = pool[k];
      bool L = (a == left) || (b == left);
      bool R = (a == right) || (b == right);
      if (L || R)
        idx.push_back(k);
    }
    if (idx.empty())
      return false;

    size_t k = idx[rng.pick(idx.size())];
    out = pool[k];
    pool[k] = pool.back();
    pool.pop_back();

    bool fitsL = (out.a == left) || (out.b == left);
    bool fitsR = (out.a == right) || (out.b == right);

    if (fitsL && fitsR)
    {
      int newL = (out.a == left) ? out.b : out.a;
      int newR = (out.a == right) ? out.b : out.a;
      attach_left = (std::min(newL, right) <= std::min(left, newR));
    }
    else
    {
      attach_left = fitsL;
    }
    return true;
  }
};

inline long long one_deal_size(int n, DominoDealer &D)
{
  D.reset(n);
  if (D.pool.empty())
    return 0;

  size_t k = D.rng.pick(D.pool.size());
  Domino start = D.pool[k];
  D.pool[k] = D.pool.back();
  D.pool.pop_back();

  int L = start.a, R = start.b;
  long long used = 1;

  while (true)
  {
    Domino d;
    bool attach_left = false;
    if (!D.take_matching(L, R, d, attach_left))
      break;

    if (attach_left)
    {
      int x = (d.a == L) ? d.b : d.a;
      L = x;
    }
    else
    {
      int x = (d.a == R) ? d.b : d.a;
      R = x;
    }
    ++used;
  }
  return used;
}
