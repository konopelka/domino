#include <bits/stdc++.h>
using namespace std;

#include "common.h"
#include "point.h"
#include "chaos1.h"
#include "affine2.h"
#include "weighted_int.h"
#include "cards.h"
#include "domino.h"
#include "board.h"

static void ensure_args(int argc)
{
  if (argc < 4)
  {
    cerr << "Usage: proj1 task[1-8] <input.txt> <output.txt>\n";
    exit(2);
  }
}
static vector<string> read_all_lines(const string &path)
{
  ifstream in(path);
  if (!in)
    throw runtime_error("Cannot open input file: " + path);
  vector<string> lines;
  string s;
  while (getline(in, s))
    if (!s.empty())
      lines.push_back(s);
  return lines;
}
static vector<double> read_doubles_line(const string &s)
{
  vector<double> v;
  istringstream is(s);
  double x;
  while (is >> x)
    v.push_back(x);
  return v;
}
static vector<long long> read_ll_line(const string &s)
{
  vector<long long> v;
  istringstream is(s);
  long long x;
  while (is >> x)
    v.push_back(x);
  return v;
}
static vector<int> read_int_line(const string &s)
{
  vector<int> v;
  istringstream is(s);
  int x;
  while (is >> x)
    v.push_back(x);
  return v;
}

static void task1(const string &inpath, const string &outpath)
{
  auto lines = read_all_lines(inpath);
  if (lines.size() < 3)
    throw runtime_error("task1: too few lines");
  size_t it = 0;
  long long N = stoll(lines[it++]);

  auto s0 = read_doubles_line(lines[it++]);
  if (s0.size() != 2)
    throw runtime_error("task1: bad start point");
  Point start{s0[0], s0[1]};

  vector<Point> bases;
  for (; it < lines.size(); ++it)
  {
    auto t = read_doubles_line(lines[it]);
    if (t.size() != 2)
      throw runtime_error("task1: bad base point line");
    bases.push_back({t[0], t[1]});
  }

  Chaos1 gen(start, bases, 0.5);

  ofstream out(outpath);
  if (!out)
    throw runtime_error("Cannot open output: " + outpath);
  Point cur = start;
  out << cur << "\n";
  for (long long i = 1; i < N; ++i)
  {
    cur = gen();
    out << cur << "\n";
  }
  cout << "Generated " << N << " points.\n";
}

static void task2(const string &inpath, const string &outpath)
{
  auto lines = read_all_lines(inpath);
  if (lines.size() < 3)
    throw runtime_error("task2: too few lines");
  size_t it = 0;
  long long N = stoll(lines[it++]);

  auto s0 = read_doubles_line(lines[it++]);
  if (s0.size() != 2)
    throw runtime_error("task2: bad start point");
  Point start{s0[0], s0[1]};

  vector<Affine> T;
  for (; it < lines.size(); ++it)
  {
    auto a = read_doubles_line(lines[it]);
    if (a.size() != 6)
      throw runtime_error("task2: transform needs 6 numbers");
    T.push_back({a[0], a[1], a[2], a[3], a[4], a[5]});
  }

  AffineIFS gen(start, T);
  ofstream out(outpath);
  if (!out)
    throw runtime_error("Cannot open output");
  Point cur = start;
  out << cur << "\n";
  for (long long i = 1; i < N; ++i)
  {
    cur = gen();
    out << cur << "\n";
  }
  cout << "Generated " << N << " points.\n";
}

static void task3(const string &inpath, const string &outpath)
{
  auto lines = read_all_lines(inpath);
  size_t it = 0;
  if (lines.size() < 4)
    throw runtime_error("task3: need N,K, values, freqs");
  long long N = stoll(lines[it++]);
  long long K = stoll(lines[it++]);

  auto vals = read_int_line(lines[it++]);
  auto freqs_ll = read_ll_line(lines[it++]);
  if ((long long)vals.size() != K || (long long)freqs_ll.size() != K)
    throw runtime_error("task3: K mismatch");
  vector<unsigned long long> freqs(freqs_ll.begin(), freqs_ll.end());

  WeightedInt gen(vals, freqs);
  unordered_map<int, long long> cnt;
  for (long long i = 0; i < N; ++i)
    ++cnt[gen()];

  ofstream out(outpath);
  if (!out)
    throw runtime_error("Cannot open output");
  unsigned long long sumf = 0;
  for (auto f : freqs)
    sumf += f;
  out << "value expected_frac empirical_frac count\n";
  double max_abs_diff = 0.0;
  for (size_t i2 = 0; i2 < vals.size(); ++i2)
  {
    int v = vals[i2];
    double expf = double(freqs[i2]) / double(sumf);
    double empf = double(cnt[v]) / double(N);
    max_abs_diff = max(max_abs_diff, fabs(expf - empf));
    out << v << " " << expf << " " << empf << " " << cnt[v] << "\n";
  }
  out << "max_abs_freq_diff " << max_abs_diff << "\n";
  cout << "Done. N=" << N << ", K=" << K << "\n";
}

static double median_vec(vector<long long> a)
{
  if (a.empty())
    return 0.0;
  sort(a.begin(), a.end());
  size_t n = a.size();
  return (n % 2) ? a[n / 2] : (a[n / 2 - 1] + a[n / 2]) / 2.0;
}
static double mean_vec(const vector<long long> &a)
{
  if (a.empty())
    return 0.0;
  long long s = 0;
  for (auto x : a)
    s += x;
  return double(s) / a.size();
}
static void write_lengths_summary(ofstream &out, const vector<long long> &lens)
{
  unordered_map<long long, long long> mp;
  for (auto L : lens)
    ++mp[L];
  long long tot = (long long)lens.size();
  vector<pair<long long, double>> percent;
  percent.reserve(mp.size());
  for (auto &[len, c] : mp)
    percent.push_back({len, 100.0 * double(c) / double(tot)});
  sort(percent.begin(), percent.end());

  out << "length %\n";
  for (auto &[L, p] : percent)
    out << L << " " << p << "\n";

  long long mode_len = max_element(mp.begin(), mp.end(),
                                   [](auto &a, auto &b)
                                   { return a.second < b.second; })
                           ->first;
  out << "mode " << mode_len << "\n";
  out << "mean " << mean_vec(lens) << "\n";
  out << "median " << median_vec(lens) << "\n";
}

static void task4(const string &inpath, const string &outpath)
{
  auto lines = read_all_lines(inpath);
  if (lines.size() < 2)
    throw runtime_error("task4: need M and TOTAL");
  long long M = stoll(lines[0]), TOTAL = stoll(lines[1]);
  Dealer D;
  D.build_deck(4, (int)M);
  D.reshuffle();
  auto lens = pile_lengths(D, TOTAL, false);
  ofstream out(outpath);
  if (!out)
    throw runtime_error("Cannot open output");
  write_lengths_summary(out, lens);
  cout << "Piles: " << lens.size() << "\n";
}

static void task5(const string &inpath, const string &outpath)
{
  auto lines = read_all_lines(inpath);
  if (lines.size() < 2)
    throw runtime_error("task5: need S and TOTAL");
  long long S_ = stoll(lines[0]), TOTAL = stoll(lines[1]);
  Dealer D;
  D.build_deck((int)S_, 13);
  D.reshuffle();
  auto lens = pile_lengths(D, TOTAL, true);
  ofstream out(outpath);
  if (!out)
    throw runtime_error("Cannot open output");
  write_lengths_summary(out, lens);
  cout << "Piles: " << lens.size() << "\n";
}


static void task6(const string &inpath, const string &outpath)
{
  auto lines = read_all_lines(inpath);
  if (lines.size() < 2)
    throw runtime_error("task6: need n and TRIALS");
  int n = stoi(lines[0]);
  long long TR = stoll(lines[1]);
  DominoDealer D;
  vector<long long> lens;
  lens.reserve(TR);
  for (long long t = 0; t < TR; ++t)
    lens.push_back(one_deal_size(n, D));
  ofstream out(outpath);
  if (!out)
    throw runtime_error("Cannot open output");
  write_lengths_summary(out, lens);
  cout << "Deals: " << TR << "\n";
}


static void task7(const string &inpath, const string &outpath)
{
  auto lines = read_all_lines(inpath);
  if (lines.size() < 2)
    throw runtime_error("task7: need n and PICKS");
  long long n = stoll(lines[0]), PICKS = stoll(lines[1]);
  auto cnt = multiplicities(n, PICKS);
  vector<long long> cnt_ll = cnt;
  ofstream out(outpath);
  if (!out)
    throw runtime_error("Cannot open output");
 
  double mean = 0.0;
  for (auto x : cnt_ll)
    mean += x;
  mean /= max(1, (int)cnt_ll.size());
  vector<long long> tmp = cnt_ll;
  sort(tmp.begin(), tmp.end());
  double med = tmp.empty() ? 0.0 : (tmp.size() % 2 ? tmp[tmp.size() / 2] : 0.5 * (tmp[tmp.size() / 2 - 1] + tmp[tmp.size() / 2]));
  auto [mn_it, mx_it] = minmax_element(cnt_ll.begin(), cnt_ll.end());
  out << "mean " << mean << "\n";
  out << "median " << med << "\n";
  out << "min " << *mn_it << "\n";
  out << "max " << *mx_it << "\n";
  cout << "Cells: " << n * n << "\n";
}

// ---------- task8 ----------
static void task8(const string &inpath, const string &outpath)
{
  auto lines = read_all_lines(inpath);
  if (lines.size() < 3)
    throw runtime_error("task8: need n, PICKS, TRIALS");
  long long n = stoll(lines[0]), PICKS = stoll(lines[1]), TR = stoll(lines[2]);
  vector<long long> vals;
  vals.reserve(TR);
  for (long long t = 0; t < TR; ++t)
    vals.push_back(free_zone_once(n, PICKS, true));
  sort(vals.begin(), vals.end());
  double mean = 0.0;
  for (auto x : vals)
    mean += x;
  mean /= max(1, (int)vals.size());
  double med = vals.empty() ? 0.0 : (vals.size() % 2 ? vals[vals.size() / 2] : 0.5 * (vals[vals.size() / 2 - 1] + vals[vals.size() / 2]));
  ofstream out(outpath);
  if (!out)
    throw runtime_error("Cannot open output");
  out << "mean " << mean << "\n";
  out << "median " << med << "\n";
  cout << "Trials: " << TR << "\n";
}

// ---------- main ----------
int main(int argc, char *argv[])
{
  try
  {
    ensure_args(argc);
    string task = argv[1], inpath = argv[2], outpath = argv[3];
    if (task == "task1")
      task1(inpath, outpath);
    else if (task == "task2")
      task2(inpath, outpath);
    else if (task == "task3")
      task3(inpath, outpath);
    else if (task == "task4")
      task4(inpath, outpath);
    else if (task == "task5")
      task5(inpath, outpath);
    else if (task == "task6")
      task6(inpath, outpath);
    else if (task == "task7")
      task7(inpath, outpath);
    else if (task == "task8")
      task8(inpath, outpath);
    else
    {
      cerr << "Unknown task\n";
      return 2;
    }
    return 0;
  }
  catch (const exception &e)
  {
    cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
