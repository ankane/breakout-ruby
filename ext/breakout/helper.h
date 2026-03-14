#pragma once

#include <cmath>
#include <functional>
#include <set>

double get_median(const std::multiset<double>&, const std::multiset<double, std::greater<>>&);
void insert_element(std::multiset<double>&, std::multiset<double, std::greater<>>&, double);
void remove_element(std::multiset<double>&, std::multiset<double, std::greater<>>&, double);

extern double Linear(double x);
extern double Const(double x);
extern double Quadratic(double x);
