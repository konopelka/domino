#pragma once
#include <cmath>
#include <ostream>

struct Point
{
  double x{};
  double y{};

  bool ok() const
  {
    return std::isfinite(x) && std::isfinite(y);
  }
};

inline std::ostream &operator<<(std::ostream &os, const Point &p)
{
  return os << p.x << ' ' << p.y;
}
