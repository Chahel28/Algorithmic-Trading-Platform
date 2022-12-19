#include "MACD, ATR, BB.h"

#include <iostream>

int main() {
    // n -> total number of days
    int n;
    std::cin >> n;

    // for Bolinger Bands and MACD
    std::vector<double> price(n);
    for (int i = 0; i < n; i++) {
        std::cin >> price[i];
    }

    // for ATR
    std::vector<double> high(n), low(n), close(n);
    for (int i = 0; i < n; i++) {
        std::cin >> high[i] >> low[i] >> close[i];
    }

    std::vector<double> w = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // sample weights for WMA

    std::vector<double> ans = MovingAverage(price, 10);
    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i] << " ";
    }
    std::cout << '\n';

    ans = ExponentialMovingAverage(price, 10, 2);
    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i] << " ";
    }
    std::cout << '\n';

    ans = WeightedMovingAverage(price, w);
    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i] << " ";
    }
    std::cout << '\n';

    ans = MACD(price, 12, 26, 9, 2);
    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i] << " ";
    }
    std::cout << '\n';

    ans = AverageTrueRange(high, low, close, 14, 2);
    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i] << " ";
    }
    std::cout << '\n';

    ans = BollingerBands(price, 20, 2);
    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i] << " ";
    }
    std::cout << '\n';

    return 0;
}
