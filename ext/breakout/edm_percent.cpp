/*
  Penalizes based on percent change in the statistic value.
  Linear penalty means that each new breakout must result in an at least X% increase
  Quadratic penalty means that each new breakout must result in at least an (X*k)% increase for k breakouts
*/

#include <algorithm>
#include <cmath>
#include <functional>
#include <set>
#include <vector>

#include "helper.h"

std::vector<int> EDM_percent(const std::vector<double>& Z, int min_size = 24, double percent = 0, int degree = 0) {
  // Z: time series
  // min_size: minimum segment size
  // beta: penalization term for the addition of a change point

  // identify which type of penalization to use
  double (*G)(double);
  switch (degree) {
  case 1:
    G = Linear;
    break;
  case 2:
    G = Quadratic;
    break;
  default:
    G = Const;
    break;
  }

  // TODO check cast
  int n = static_cast<int>(Z.size());

  std::vector<int> prev(n + 1, 0);   // store optimal location of previous change point
  std::vector<int> number(n + 1, 0); // store the number of change points in optimal segmentation
  std::vector<double> F(n + 1, 0);   // store optimal statistic value
  // F[s] is calculated using observations { Z[0], Z[1], ..., Z[s-1] }

  // trees used to store the "upper half" of the considered observations
  std::multiset<double> right_min;
  std::multiset<double> left_min;
  // trees used to store the "lower half" of the considered observations
  std::multiset<double, std::greater<>> right_max;
  std::multiset<double, std::greater<>> left_max;

  // Iterate over possible locations for the last change
  for (int s = 2 * min_size; s < n + 1; ++s) {
    right_max.clear();
    right_min.clear(); // clear right trees
    left_max.clear();
    left_min.clear(); // clear left trees

    // initialize left and right trees to account for minimum segment size
    for (int i = prev.at(min_size - 1); i < min_size - 1; ++i) {
      insert_element(left_min, left_max, Z.at(i));
    }
    for (int i = min_size - 1; i < s; ++i) {
      insert_element(right_min, right_max, Z.at(i));
    }

    // Iterate over possible locations for the penultimate change
    for (int t = min_size; t < s - min_size + 1; ++t) { // modify limits to deal with min_size
      insert_element(left_min, left_max, Z.at(t - 1));     // insert element into left tree
      remove_element(right_min, right_max, Z.at(t - 1));   // remove element from right tree
      // left tree now has { Z[prev[t-1]], ..., Z[t-1] }
      // right tree now has { Z[t], ..., Z[s-1] }

      // check to see if optimal position of previous change point has changed
      // if so update the left tree
      if (prev.at(t) > prev.at(t - 1)) {
        for (int i = prev.at(t - 1); i < prev.at(t); ++i) {
          remove_element(left_min, left_max, Z.at(i));
        }
      } else if (prev.at(t) < prev.at(t - 1)) {
        for (int i = prev.at(t); i < prev.at(t - 1); ++i) {
          insert_element(left_min, left_max, Z.at(i));
        }
      }

      // calculate statistic value
      double left_median = get_median(left_min, left_max);
      double right_median = get_median(right_min, right_max);
      double normalize = ((t - prev.at(t)) * (s - t)) / (std::pow(static_cast<double>(s - prev.at(t)), 2));
      double tmp = F.at(t) + normalize * std::pow(left_median - right_median, 2);
      // Find best location for change point. check % condition later
      if (tmp > F.at(s)) {
        number.at(s) = number.at(t) + 1;
        F.at(s) = tmp;
        prev.at(s) = t;
      }
    }
    // check to make sure we meet the percent change requirement
    if (prev.at(s) != 0) {
      if (F.at(s) - F.at(prev.at(s)) < percent * G(number.at(prev.at(s))) * F.at(prev.at(s))) {
        number.at(s) = number.at(prev.at(s));
        F.at(s) = F.at(prev.at(s));
        prev.at(s) = prev.at(prev.at(s));
      }
    }
  }

  // obtain list of optimal change point estimates
  std::vector<int> ret;
  int at = n;
  while (at != 0) {
    if (prev.at(at) != 0) { // don't insert 0 as a change point estimate
      ret.push_back(prev.at(at));
    }
    at = prev.at(at);
  }
  sort(ret.begin(), ret.end());
  return ret;
}
