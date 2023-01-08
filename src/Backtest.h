#pragma once

#include <vector>

// This is a simple ROI calculator, returns the %ROI of a strategy
// signal > 0 -> long, signal < 0 -> short, signal == 0 -> flat
template <typename T>
double roi(const std::vector<double> &prices, const std::vector<T> &signal);

// This is a simple CAGR calculator, returns the %CAGR of a strategy
// signal > 0 -> long, signal < 0 -> short, signal == 0 -> flat
template <typename T>
double cagr(const std::vector<double> &prices, const std::vector<T> &signal, const double years);
