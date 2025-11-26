#pragma once
#include "common.h"

inline long long idx(long long n, long long r, long long c) { return r * n + c; }

struct BoardPicker
{
  long long n;
  RNG rng;
  explicit BoardPicker(long long n_) : n(n_)
  {
    if (n <= 0)
      throw std::invalid_argument("n>0");
  }
  std::pair<long long, long long> operator()()
  {
    std::uniform_int_distribution<long long> d(0, n - 1);
    return {d(rng.gen), d(rng.gen)};
  }
};

inline std::vector<long long> multiplicities(long long n, long long picks)
{
  BoardPicker P(n);
  std::vector<long long> cnt(n * n, 0);
  for (long long i = 0; i < picks; ++i)
  {
    auto [r, c] = P();
    ++cnt[idx(n, r, c)];
  }
  return cnt;
}

inline long long free_zone_once(long long n, long long picks, bool eight_neighbors = true)
{
  BoardPicker P(n);
  std::vector<char> banned(n * n, 0);

  auto ban = [&](long long r, long long c)
  {
    if (r < 0 || c < 0 || r >= n || c >= n)
      return;
    banned[idx(n, r, c)] = 1;
  };

  std::vector<std::pair<int, int>> dirs;
  if (eight_neighbors)
  {
    for (int dr = -1; dr <= 1; ++dr)
      for (int dc = -1; dc <= 1; ++dc)
        if (!(dr == 0 && dc == 0))
          dirs.push_back({dr, dc});
  }
  else
  {
    dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  }

  for (long long i = 0; i < picks; ++i)
  {
    auto [r, c] = P();
    ban(r, c);
    for (auto [dr, dc] : dirs)
      ban(r + dr, c + dc);
  }

  long long free_cells = 0;
  for (char b : banned)
    if (!b)
      ++free_cells;
  return free_cells;
}
