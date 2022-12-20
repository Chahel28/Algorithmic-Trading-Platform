#include "SMA Crossovers.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

template <typename T>
void disp(std::vector<T> v) { // just to display a vector
    for (auto &thing : v) {
        std::cout << thing << " ";
    }
    std::cout << '\n';
}

std::vector<double> simpleMovingAverage(std::vector<double> &v, int ws) { // `v` array of prices ; `ws` window size
    std::vector<double> ans(v.size());
    double sum = 0;
    for (int i = 0; i < ws; i++) {
        sum += v[i];
        ans[i] = sum / (i + 1);
    }

    for (int i = ws; i < v.size(); i++) {
        sum += v[i] - v[i - ws];
        ans[i] = sum / ws;
    }

    return ans;
}

std::vector<int> maCrossover(std::vector<double> &v, int w1, int w2) { // `v` vector of prices, `w1` window of larger MA, `w2` window of smaller MA
    if (w1 > w2) {
        std::swap(w1, w2);
    }

    std::vector<double> smallMA, bigMA; // big and small window moving average vectors
    for (int i = 0; i < w1; i++) {
        smallMA.push_back(v[i]);
    }
    for (int i = 0; i < w1; i++) {
        bigMA.push_back(v[i]);
    }

    for (auto &i : simpleMovingAverage(v, w1)) {
        smallMA.push_back(i);
    }
    for (auto &i : simpleMovingAverage(v, w2)) {
        bigMA.push_back(i);
    }
    // Trades will be possible from index `w2` to `v.size() - 1` on smallMA and bigMA

    std::vector<int> diff(v.size()); // vector of price differences between bigMA and smallMA
    for (int i = 0; i < v.size(); i++) {
        diff[i] = bigMA[i] - smallMA[i];
    }

    std::vector<int> buySell(diff.size());
    for (int i = 1; i < diff.size(); i++) {
        if (diff[i] < 0 && diff[i - 1] > 0) {        // buy price , golden cross
            buySell[i] = 1;                          // true implies its a buy signal at price of `v[i]`
        } else if (diff[i] > 0 && diff[i - 1] < 0) { // sell price, death cross
            buySell[i] = -1;                         // false implies its a sell signal at price of `v[i]`
        }                                            // else buySell[i] = 0;
    }

    return buySell;
}
