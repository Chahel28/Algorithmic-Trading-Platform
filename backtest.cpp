#include "MACD, ATR, BB.h"
#include "SMA Crossovers.h"

#include <DataFrame/DataFrame.h>
#include <DataFrame/Utils/DateTime.h>
#include <iostream>

using namespace hmdf;

using StrDataFrame = StdDataFrame<std::string>;
using DTDataFrame = StdDataFrame<DateTime>;

// This is a simple ROI calculator, returns the %ROI of a strategy
double roi(const std::vector<double> &prices, const std::vector<bool> &position) {
    // eg. if position = 1 only for i = 0, then you buy at prices[0] and sell at prices[1]
    // giving a return of (prices[1] - prices[0]) / prices[0]
    double ret = 100;
    for (size_t i = 0; i < prices.size() - 1; ++i) { // ignore long position on final tick
        if (position[i]) {
            ret *= prices[i + 1] / prices[i];
        }
    }
    return ret - 100;
}

int main() {
    StrDataFrame ibm_df;
    ibm_df.read("DataFrame/data/IBM.csv", io_format::csv2);
    auto &close = ibm_df.get_column<double>("IBM_Close");

    std::vector<bool> position;
    position.reserve(close.size());
    for (double &bb : BollingerBands(close, 20, 1)) {
        position.push_back(bb > 1);
    }
    std::cout << "Bollinger Bands (1 sigma) ROI for IBM: " << roi(close, position) << "%\n";

    DTDataFrame dt_ibm;
    dt_ibm.read("data/DT_IBM.csv", io_format::csv2);
    close = dt_ibm.get_column<double>("IBM_Close");

    position.clear();
    for (double &bb : BollingerBands(close, 20, 2)) {
        position.push_back(bb > 1);
    }
    std::cout << "Bollinger Bands (2 sigma) ROI for IBM: " << roi(close, position) << "%\n";

    return 0;
}
