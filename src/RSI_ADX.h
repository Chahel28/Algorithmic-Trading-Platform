#pragma once

#include <vector>

std::vector<double> RSI(double period, std::vector<double> &close, int n);

std::pair<std::vector<double>, std::pair<std::vector<double>, std::vector<double>>> ADX_DI(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, int n);
