/*
Robust estimation of 2[mean(X)-mean(Y)]^2 time normalization factor
This is the E-Divisive E-statistic when alpha = 2
Instead of calculating mean(X), we calculate median(X), and similarly for Y
*/

#include <algorithm>
#include <cmath>
#include <functional>
#include <queue>
#include <vector>

void AddToHeaps(std::priority_queue<double, std::vector<double>, std::greater<double>>& m, std::priority_queue<double>& M, double x);

double getMedian(const std::priority_queue<double, std::vector<double>, std::greater<double>>& m, const std::priority_queue<double>& M);

std::vector<int> EDMX(const std::vector<double>& Z, int min_size = 24, double alpha = 2) {

  alpha = 2; // Not used, just here for uniform function signature

  std::priority_queue<double> LeftMax;
  std::priority_queue<double, std::vector<double>, std::greater<double>> LeftMin;

  double stat = -3, stat_best = -3, t1 = 0.0, t2;
  int tau1, tau2;
  int N = Z.size();
  for (int i = 0; i < min_size - 1; ++i)
    AddToHeaps(LeftMin, LeftMax, Z[i]);

  for (tau1 = min_size; tau1 < N - min_size + 1; ++tau1) { // Iterate over breakout locations
    AddToHeaps(LeftMin, LeftMax, Z[tau1 - 1]);
    std::priority_queue<double> RightMax;
    std::priority_queue<double, std::vector<double>, std::greater<double>> RightMin;
    double medL = getMedian(LeftMin, LeftMax);

    // Add first set of elements to the heaps for the right segment
    for (std::vector<double>::const_iterator i = Z.begin() + tau1; i != Z.begin() + tau1 + min_size - 1; ++i)
      AddToHeaps(RightMin, RightMax, *i);

    for (tau2 = tau1 + min_size; tau2 < N + 1; ++tau2) { // Iterate over end of prefix series locations
      AddToHeaps(RightMin, RightMax, Z[tau2 - 1]);
      double medR = getMedian(RightMin, RightMax);

      stat = std::pow(medL - medR, 2);
      stat *= ((double)tau1 * (tau2 - tau1) / tau2);

      if (stat > stat_best) {
        t1 = tau1;
        t2 = tau2;
        stat_best = stat;
      }
    }
  }

  std::vector<int> ret;
  if (stat_best > 0) {
    ret.push_back(t1);
  }
  return ret;
}

// Use 2 heaps to keep track of the median (can also be adjusted for other quantiles). One heap
// for the "larger" and one heap for the "smaller" observations. Simple to update for streaming
// data ( O(log n) ) and find median ( O(1) ).

double getMedian(const std::priority_queue<double, std::vector<double>, std::greater<double>>& m, const std::priority_queue<double>& M) {

  if (m.size() > M.size()) // There are an odd number of observations
    return m.top();
  else if (M.size() > m.size()) // There are an odd number of observations
    return M.top();
  else // There are an even number of obersations
    return (m.top() + M.top()) / 2;
}

void AddToHeaps(std::priority_queue<double, std::vector<double>, std::greater<double>>& m, std::priority_queue<double>& M, double x) {

  // decide on initial heap to place element into
  if (m.empty() || x < m.top())
    M.push(x);
  else
    m.push(x);
  // make sure that heaps are balanced
  if (m.size() > M.size() + 1) {
    M.push(m.top());
    m.pop();
  } else if (M.size() > m.size() + 1) {
    m.push(M.top());
    M.pop();
  }
}
