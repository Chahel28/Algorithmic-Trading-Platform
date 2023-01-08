#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

#include "RSI_ADX.h"

std::vector<double> RSI(double period, std::vector<double> &close, int n) {
    if (period < 1) {
        std::cout << "Please give an integer value for period >=1";
        return std::vector<double>();
    }

    std::vector<double> diffClose(n);
    std::adjacent_difference(close.begin(), close.end(), diffClose.begin());
    diffClose[0] = 0;

    std::vector<double> gain(n), loss(n);
    for (int i = 0; i < n; i++) {
        gain[i] = (diffClose[i] >= 0) ? diffClose[i] : 0;
        loss[i] = (diffClose[i] < 0) ? -diffClose[i] : 0;
    }

    int floor_period = std::floor(period);
    double avg_gain = 0, avg_loss = 0;
    for (int i = 0; i < period; i++) {
        avg_gain += gain[i];
        avg_loss += loss[i];
    }
    avg_gain /= period;
    avg_loss /= period;

    std::vector<double> RSI_val;
    for (int i = floor_period; i < n; i++) {
        int RS = avg_gain / avg_loss;
        RSI_val.push_back(100 - 100 / (1 + RS));

        avg_gain = (avg_gain * (floor_period - 1) + gain[i]) / floor_period;
        avg_gain = (avg_gain * (floor_period - 1) + gain[i]) / floor_period;
    }

    return RSI_val;
}

std::pair<std::vector<double>, std::pair<std::vector<double>, std::vector<double>>> ADX_DI(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, int n) {
    std::vector<double> DM_plus(n), DM_minus(n);
    adjacent_difference(high.begin(), high.end(), DM_plus.begin());
    adjacent_difference(low.begin(), low.end(), DM_minus.begin());

    for (int i = 0; i < n; i++) {
        DM_plus[i] = std::max(DM_plus[i], 0.0);
        DM_minus[i] = std::max(-DM_minus[i], 0.0);
    }

    std::vector<double> TR;
    for (int i = 0; i < n - 1; i++) {
        TR.push_back(std::max(high[i + 1] - low[i + 1],
                              std::max(std::abs(high[i + 1] - close[i]),
                                       std::abs(low[i + 1] - close[i]))));
    }

    std::vector<double> avg_tr{0};
    for (int i = 0; i < 14; i++) {
        avg_tr[0] += TR[i];
    }

    for (int i = 14; i < n - 1; i++) {
        avg_tr.push_back(avg_tr[i - 14] * (13 / 14) + TR[i]);
    }

    std::vector<double> DI_plus{0}, DI_minus{0};

    for (int i = 1; i < 15; i++) {
        DI_plus[0] += DM_plus[i];
        DI_minus[0] += DM_minus[i];
    }

    for (int i = 15; i < n; i++) {
        DI_plus.push_back(DI_plus[i - 15] * (13 / 14) + DM_plus[i]);
        DI_minus.push_back(DI_minus[i - 15] * (13 / 14) + DM_minus[i]);
    }

    std::vector<double> DX;
    for (int i = 0; i < DI_plus.size(); i++) {
        DI_plus[i] = DI_plus[i] * 100 / avg_tr[i];
        DI_minus[i] = DI_minus[i] * 100 / avg_tr[i];
        DX.push_back(std::abs(DI_plus[i] - DI_minus[i]) * 100 / std::abs(DI_plus[i] + DI_minus[i]));
    }

    std::vector<double> ADX{0};
    for (int i = 0; i < 14; i++) {
        ADX[0] += DX[i];
    }

    for (int i = 14; i < DX.size(); i++) {
        ADX.push_back((ADX[i - 14] * 13 + DX[i]) / 14);
    }

    std::pair<std::vector<double>, std::vector<double>> DI(DI_plus, DI_minus);
    std::pair<std::vector<double>, std::pair<std::vector<double>, std::vector<double>>> ADX_DI(ADX, DI);

    return ADX_DI;
}
