#include <cmath>
#include <vector>

// Hanging Man Candlestick Pattern
// When a specified security notably moves lower after the open, but continues to rally to close above the intraday low, 
// a Hanging Man candlestick will form. The candlestick will resemble a square, attached to a long stick-like figure. 
// It is referred to as a Hanging Man if the candlestick forms during an advance

// above-average volume, longer lower shadow(at least twice the body), selling on the following day make it more reliable

// https://github.com/SpiralDevelopment/candlestick-patterns --> [logic] 

std::vector<bool> HangingMan(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, std::vector<double> &open, int w1 = 4, int w2 = 0.75) {
    
    std::vector<bool> hangingMan(high.size(), false);
    bool hm;

    for (int i = 2; i < high.size(); i++) {

        hm = (((high[i] - low[i]) > w1*(open[i] - close[i])) && 
        ((close[i] - low[i]) / (.001 + high[i] - low[i]) >= w2) && 
        ((open[i] - low[i]) / (.001 + high[i] - low[i]) >= w2) &&
        (high[i-1] < open[i]) &&
        (high[i-2] < open[i]));

        hangingMan[i] = hm;
    }

    return hangingMan;
}