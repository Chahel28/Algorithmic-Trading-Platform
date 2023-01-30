#include <cmath>
#include <vector>

// Bullish Engulfing Candlestick Pattern
// https://github.com/SpiralDevelopment/candlestick-patterns --> [logic] 

std::vector<bool> bullishEngulfing(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, std::vector<double> &open, int w1 = 3, int w2 = 0.6) {
    
    std::vector<bool> bullEngulf(high.size(), false);
    bool be;

    for (int i = 1; i < high.size(); i++) {

        be = (  (close[i] >= open[i-1]) &&
                (open[i-1] > close[i-1]) &&
                (close[i] > open[i]) &&
                (close[i-1] >= open[i]) &&
                ((close[i] - open[i]) > (open[i-1] - close[i-1]))
             );

        // # return (prev_close < prev_open and
        // #         0.3 > abs(prev_close - prev_open) / (prev_high - prev_low) >= 0.1 and
        // #         close > open and
        // #         abs(close - open) / (high - low) >= 0.7 and
        // #         prev_high < close and
        // #         prev_low > open)

        bullEngulf[i] = be;
    }

    return bullEngulf;
}

