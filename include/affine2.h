#pragma once
#include "point.h"
#include "common.h"

struct Affine
{
  double a11, a12, a21, a22, tx, ty;

  Point apply(const Point &p) const
  {
    return {a11 * p.x + a12 * p.y + tx,
            a21 * p.x + a22 * p.y + ty};
  }
};

struct AffineIFS
{
  std::vector<Affine> T;
  RNG rng;
  Point cur;

  AffineIFS(Point s, std::vector<Affine> t)
      : T(std::move(t)), cur(s)
  {
    if (T.empty())
      throw std::invalid_argument("No affine transforms");
    if (!cur.ok())
      throw std::invalid_argument("Start not finite");
  }

  Point operator()()
  {
    const auto &A = T[rng.pick(T.size())];
    return (cur = A.apply(cur));
  }
};
