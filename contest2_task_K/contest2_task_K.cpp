#include <algorithm>
#include <iostream>
#include <vector>

const long long kInf = 1e18;
const long long kCoefficientK = -2LL;
const long long kCoefficientB = 1LL;

class Line {
 public:
  long long slope;
  long long intercept;

  Line(long long slope = 0, long long intercept = 0) 
      : slope(slope), intercept(intercept) {}

  long long Evaluate(long long x) const {
    return slope * x + intercept;
  }
};

class ConvexHullTrick {
 public:
  std::vector<Line> lines;
  std::vector<double> points;

  void AddLine(const Line& newLine) {
    while (!points.empty() &&
           newLine.Evaluate(points.back()) <= lines.back().Evaluate(points.back())) {
      lines.pop_back();
      points.pop_back();
    }

    if (!lines.empty()) {
      double newPoint =
          static_cast<double>(lines.back().intercept - newLine.intercept) / (newLine.slope - lines.back().slope);
      points.push_back(newPoint);
    }
    
    lines.push_back(newLine);
  }

  long long GetMinimum(long long x) const {
    if (lines.empty()) {
      return kInf;
    }
    
    auto it = std::lower_bound(points.begin(), points.end(), x);
    int idx = it - points.begin();
    return lines[idx].Evaluate(x);
  }
};

int main() {
  int numSegments, numLines;
  std::cin >> numSegments >> numLines;

  if (numLines == 0) {
    std::cout << 0 << std::endl;
    return 0;
  }

  std::vector<std::vector<long long>> dp(numLines + 1, std::vector<long long>(numSegments + 1, kInf));
  dp[0][0] = 0;

  for (int j = 1; j <= numLines; ++j) {
    ConvexHullTrick cht;
    for (int i = 0; i <= numSegments; ++i) {
      dp[j][i] = cht.GetMinimum(i) + kCoefficientB * i * i;
      if (i < numSegments) {
        cht.AddLine(Line(kCoefficientK * (i + 1),
                         dp[j - 1][i] + kCoefficientB * (i + 1) * (i + 1)));
      }
    }
  }

  std::cout << dp[numLines][numSegments] << std::endl;
  return 0;
}