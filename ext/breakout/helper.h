#pragma once

#include <algorithm>
#include <cmath>
#include <set>

double get_median(const std::multiset<double>&, const std::multiset<double, std::greater<double>>&);
void insert_element(std::multiset<double>&, std::multiset<double, std::greater<double>>&, double);
void remove_element(std::multiset<double>&, std::multiset<double, std::greater<double>>&, double);

extern double Linear(double x);
extern double Const(double x);
extern double Quadratic(double x);
