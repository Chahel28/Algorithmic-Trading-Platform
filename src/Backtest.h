#pragma once

#include <vector>

#include <DataFrame/DataFrame.h>
#include <DataFrame/Utils/DateTime.h>
#include <iostream>

#include "Backtest.h"
#include "MACD, ATR, BB.h"
#include "RSI_ADX.h"
#include "SMA Crossovers.h"

using namespace hmdf;

using StrDataFrame = StdDataFrame<std::string>;
using DTDataFrame = StdDataFrame<DateTime>;

inline double seconds_to_years(const double seconds);

double mean(const std::vector<double> &values);
double stdev(const std::vector<double> &values, const double mean);
double stdev(const std::vector<double> &values);

// This is a simple ROI calculator, returns the %ROI of a strategy
// signal > 0 -> long, signal < 0 -> short, signal == 0 -> flat
template <typename T>
std::vector<double> returns(const std::vector<double> &prices, const std::vector<T> &signal);

// This is a simple CAGR calculator, returns the %CAGR of a strategy
// signal > 0 -> long, signal < 0 -> short, signal == 0 -> flat
double cagr(const std::vector<double> &returns, double years);

std::vector<double> risk_free_returns(const std::vector<double> &returns, const double risk_free_rate = 7.4);

double sharpe_ratio(const std::vector<double> &returns, const double risk_free_rate = 7.4);
