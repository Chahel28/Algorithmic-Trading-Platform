#include <cmath>
#include <vector>

// Hammer Candlestick Pattern
// https://github.com/SpiralDevelopment/candlestick-patterns --> [logic] 

std::vector<bool> Hammer(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, std::vector<double> &open, int w1 = 3, int w2 = 0.6) {
    
    std::vector<bool> hammer(high.size(), false);
    bool hm;

    for (int i = 0; i < high.size(); i++) {

        hm = (((high[i] - low[i]) > w1*(open[i] - close[i])) &&
                ((close[i] - low[i]) / (.001 + high[i] - low[i]) > w2) &&
                ((open[i] - low[i]) / (.001 + high[i] - low[i]) > w2));

        hammer[i] = hm;
    }

    return hammer;
}

