#pragma once

#include <vector>

// v-> vector of prices, n-> size of MA window
std::vector<double> MovingAverage(std::vector<double> &v, int n);

// v-> vector of prices, n-> size of EMA window, k-> smoothing factor = x/(n+1)
std::vector<double> ExponentialMovingAverage(std::vector<double> &v, int n, double x = 2);

// v-> vector of prices, w-> weights for WMA
std::vector<double> WeightedMovingAverage(std::vector<double> &v, std::vector<double> &w);

// smoothning factor = x/(n+1) for EMA
// EMA1-> fast EMA, EMA2-> slow EMA
// signal line is MA of MACD (n3), hist is MACD-EMA(signal line), 0 for i<n3-1
// hist > 0 -> bullish, hist < 0 -> bearish
std::vector<double> MACD(std::vector<double> &price, int n1 = 12, int n2 = 26, int n3 = 9, double x = 2);

// ATR = EMA of TR (n), smoothening factor = x/(n+1)
// TR = max(high-low, abs(high-prev_close), abs(low-prev_close))
std::vector<double> AverageTrueRange(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, int n, double x = 2);

// Bollinger Bands from MA - k*std to MA + k*std
std::vector<int> BollingerBands(std::vector<double> &price, int n, double k = 2);
