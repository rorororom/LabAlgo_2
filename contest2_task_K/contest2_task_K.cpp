#include <algorithm>
#include <iostream>
#include <vector>

const long long INF = 1e18;
const long long COEFFICIENT_K = -2LL;
const long long COEFFICIENT_B = 1LL;

class Line {
 public:
  long long k, b;
  Line(long long k = 0, long long b = 0) : k(k), b(b) {}
  long long e_l(long long x) const { return k * x + b; }
};

class ConvexHullTrick {
 public:
  std::vector<Line> lines;
  std::vector<double> points;

  void a_line(const Line& newLine) {
    while (!points.empty() &&
           newLine.e_l(points.back()) <= lines.back().e_l(points.back())) {
      lines.pop_back();
      points.pop_back();
    }
    if (!lines.empty()) {
      double newPoint =
          (double)(lines.back().b - newLine.b) / (newLine.k - lines.back().k);
      points.push_back(newPoint);
    }
    lines.push_back(newLine);
  }

  long long get_min(long long x) const {
    if (lines.empty()) {
      return INF;
    }
    auto it = lower_bound(points.begin(), points.end(), x);
    int idx = it - points.begin();
    return lines[idx].e_l(x);
  }
};

int main() {
  int n, k;
  std::cin >> n >> k;
  if (k == 0) {
    std::cout << 0 << std::endl;
    return 0;
  }
  std::vector<std::vector<long long>> dp(k + 1,
                                         std::vector<long long>(n + 1, INF));
  dp[0][0] = 0;
  for (int j = 1; j <= k; ++j) {
    ConvexHullTrick res_r;
    for (int i = 0; i <= n; ++i) {
      dp[j][i] = res_r.get_min(i) + COEFFICIENT_B * i * i;
      if (i < n) {
        res_r.a_line(Line(COEFFICIENT_K * (i + 1),
                          dp[j - 1][i] + COEFFICIENT_B * (i + 1) * (i + 1)));
      }
    }
  }
  std::cout << dp[k][n] << std::endl;
  return 0;
}