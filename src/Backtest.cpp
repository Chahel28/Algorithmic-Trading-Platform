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

inline double seconds_to_years(const double seconds) {
    return seconds / (365.24 * 24 * 60 * 60);
}

// This is a simple ROI calculator, returns the %ROI of a strategy
// signal > 0 -> long, signal < 0 -> short, signal == 0 -> flat
template <typename T>
double roi(const std::vector<double> &prices, const std::vector<T> &signal) {
    // giving a return of (prices[1] - prices[0]) / prices[0]
    double ret = 100;
    bool position = false;
    for (int i = 1; i < prices.size() - 1; ++i) { // ignore buy signal on final tick
        if (signal[i - 1] > 0 && !position) {
            position = true;
            // std::cout << "Buy: " << prices[i] << '\t';
        } else if (signal[i - 1] < 0 && position) {
            position = false;
            // std::cout << "Sell: " << prices[i] << '\n';
        }

        if (position) { // buy now if previous signal was a buy
            ret *= prices[i + 1] / prices[i];
        }
    }

    return ret - 100;
}

// This is a simple CAGR calculator, returns the %CAGR of a strategy
// signal > 0 -> long, signal < 0 -> short, signal == 0 -> flat
template <typename T>
double cagr(const std::vector<double> &prices, const std::vector<T> &signal, double years) {
    return 100 * std::pow((roi(prices, signal) + 100) / 100, 1 / years) - 100;
}

int main() {
    DTDataFrame ibm;
    ibm.read("DataFrame/data/DT_IBM.csv", io_format::csv2);
    auto close = ibm.get_column<double>("IBM_Close");
    auto index = ibm.get_index();
    auto years = seconds_to_years(index.back() - index.front());

    std::cout << "Bollinger Bands (1 sigma) CAGR (Rate of Return) for IBM: " << cagr(close, BollingerBands(close, 20, 1), years) << "%\n";
    std::cout << "Bollinger Bands (2 sigma) CAGR (Rate of Return) for IBM: " << cagr(close, BollingerBands(close, 20, 2), years) << "%\n";
    std::cout << "MACD Crossover CAGR (Rate of Return) for IBM: " << cagr(close, MACD(close, 12, 26, 9), years) << "%\n";
    std::cout << "SMA-20 SMA-50 Crossover CAGR (Rate of Return) for IBM: " << cagr(close, maCrossover(close, 20, 50), years) << "%\n\n";

    DTDataFrame aapl;
    aapl.read("DataFrame/data/DT_AAPL.csv", io_format::csv2);
    close = aapl.get_column<double>("AAPL_Close");
    index = aapl.get_index();
    years = (index.back() - index.front()) / (365.24 * 24 * 60 * 60);
    std::cout << "Bollinger Bands (1 sigma) CAGR (Rate of Return) for AAPL: " << cagr(close, BollingerBands(close, 20, 1), years) << "%\n";
    std::cout << "Bollinger Bands (2 sigma) CAGR (Rate of Return) for AAPL: " << cagr(close, BollingerBands(close, 20, 2), years) << "%\n";
    std::cout << "MACD Crossover CAGR (Rate of Return) for AAPL: " << cagr(close, MACD(close, 12, 26, 9), years) << "%\n";
    std::cout << "SMA-20 SMA-50 Crossover CAGR (Rate of Return) for AAPL: " << cagr(close, maCrossover(close, 20, 50), years) << "%\n\n";

    return 0;
}
