#pragma once
#include "point.h"
#include "common.h"

struct Chaos1
{
  Point cur;
  std::vector<Point> bases;
  double r; 
  RNG rng;

  Chaos1(Point s, std::vector<Point> b, double ratio = 0.5)
      : cur(s), bases(std::move(b)), r(ratio)
  {
    if (bases.empty())
      throw std::invalid_argument("No base points");

    if (!cur.ok())
      throw std::invalid_argument("Start not finite");

    if (!(r > 0 && r <= 1))
      throw std::invalid_argument("ratio must be in (0, 1]");
  }

  Point operator()()
  {
    const auto &b = bases[rng.pick(bases.size())];
    cur.x = (1.0 - r) * cur.x + r * b.x;
    cur.y = (1.0 - r) * cur.y + r * b.y;
    return cur;
  }
};
