#pragma once

#include <vector>

std::vector<bool> HangingMan(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, std::vector<double> &open, int w1 = 4, int w2 = 0.75);

std::vector<bool> bearishEngulfing(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, std::vector<double> &open, int w1 = 3, int w2 = 0.6);

std::vector<bool> Hammer(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, std::vector<double> &open, int w1 = 3, int w2 = 0.6);

std::vector<bool> bullishEngulfing(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, std::vector<double> &open, int w1 = 3, int w2 = 0.6); 

