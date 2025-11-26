#pragma once
#include <vector>
#include <optional>
#include "common.h"

struct Card
{
  int suit;
  int rank;


  bool operator>(const Card &other) const
  {
    return rank > other.rank;
  }
  bool operator>=(const Card &other) const
  {
    return rank >= other.rank;
  }
};

struct Dealer
{
  RNG rng;
  std::vector<Card> deck, cur;

  void build_deck(int S, int M)
  {
    deck.clear();
    for (int s = 0; s < S; ++s)
      for (int r = 1; r <= M; ++r)
        deck.push_back({s, r});
  }

  void reshuffle()
  {
    cur = deck;
    rng.shuffle(cur.begin(), cur.end());
  }

  Card next()
  {
    if (cur.empty())
      reshuffle();
    Card c = cur.back();
    cur.pop_back();
    return c;
  }
};

inline std::vector<long long> pile_lengths(Dealer &D, long long total, bool nondecreasing)
{
  std::vector<long long> lens;
  if (total <= 0)
    return lens;

  long long len = 0;
  std::optional<Card> prev;

  for (long long i = 0; i < total; ++i)
  {
    Card c = D.next();

    if (!prev.has_value())
    {
      prev = c;
      len = 1;
      continue;
    }

    bool ok = nondecreasing ? (c >= prev.value())
                            : (c > prev.value());

    if (ok)
    {
      ++len;
      prev = c;
    }
    else
    {
      lens.push_back(len);
      prev = c;
      len = 1;
    }
  }

  if (len > 0)
    lens.push_back(len);

  return lens;
}
