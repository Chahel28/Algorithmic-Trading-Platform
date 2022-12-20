#pragma once

#include <vector>

// just to display a vector
template <typename T>
void disp(std::vector<T> v);

// `v` array of prices ; `ws` window size
std::vector<double> simpleMovingAverage(std::vector<double> &v, int ws);

// `v` vector of prices, `w1` window of larger MA, `w2` window of smaller MA
std::vector<int> maCrossover(std::vector<double> &v, int w1, int w2);
