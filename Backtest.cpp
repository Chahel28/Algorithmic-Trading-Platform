#include "MACD, ATR, BB.h"
#include "SMA Crossovers.h"

#include <DataFrame/DataFrame.h>
#include <DataFrame/Utils/DateTime.h>
#include <iostream>

using namespace hmdf;

using StrDataFrame = StdDataFrame<std::string>;
using DTDataFrame = StdDataFrame<DateTime>;

// This is a simple ROI calculator, returns the %ROI of a strategy
// signal > 0 -> long, signal < 0 -> short, signal == 0 -> flat
template <typename T>
double roi(const std::vector<double> &prices, const std::vector<T> &signal) {
    // giving a return of (prices[1] - prices[0]) / prices[0]
    double ret = 100;
    bool position = false;
    for (int i = 0; i < prices.size() - 1; ++i) { // ignore buy signal on final tick
        if (signal[i] > 0 && !position) {
            position = true;
            // std::cout << "Buy: " << prices[i] << '\t';
        } else if (signal[i] < 0 && position) {
            position = false;
            // std::cout << "Sell: " << prices[i] << '\n';
        }

        if (position) {
            ret *= prices[i + 1] / prices[i];
        }
    }

    return ret - 100;
}

int main() {
    StrDataFrame ibm_df;
    ibm_df.read("DataFrame/data/IBM.csv", io_format::csv2);
    auto close = ibm_df.get_column<double>("IBM_Close");
    std::cout << "Bollinger Bands (1 sigma) ROI for IBM: " << roi(close, BollingerBands(close, 20, 1)) << "%\n";
    std::cout << "Bollinger Bands (2 sigma) ROI for IBM: " << roi(close, BollingerBands(close, 20, 2)) << "%\n";
    std::cout << "MACD Crossover ROI for IBM: " << roi(close, MACD(close, 12, 26, 9)) << "%\n";
    std::cout << "SMA-20 SMA-50 Crossover ROI for IBM: " << roi(close, maCrossover(close, 20, 50)) << "%\n\n";

    DTDataFrame dt_aapl;
    dt_aapl.read("data/DT_AAPL.csv", io_format::csv2);
    close = dt_aapl.get_column<double>("AAPL_Close");
    std::cout << "Bollinger Bands (1 sigma) ROI for AAPL: " << roi(close, BollingerBands(close, 20, 1)) << "%\n";
    std::cout << "Bollinger Bands (2 sigma) ROI for AAPL: " << roi(close, BollingerBands(close, 20, 2)) << "%\n";
    std::cout << "MACD Crossover ROI for AAPL: " << roi(close, MACD(close, 12, 26, 9)) << "%\n";
    std::cout << "SMA-20 SMA-50 Crossover ROI for AAPL: " << roi(close, maCrossover(close, 20, 50)) << "%\n\n";

    std::cout << "Returns are large because the data is for 22 years!\n";

    return 0;
}
