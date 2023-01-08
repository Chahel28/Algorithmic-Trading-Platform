#include <algorithm>
#include <cmath>

#include "MACD, ATR, BB.h"

// v-> vector of prices, n-> size of MA window
std::vector<double> MovingAverage(std::vector<double> &v, int n) {
    std::vector<double> ans(v.size());
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += v[i];
        ans[i] = sum / (i + 1);
    }

    for (int i = n; i < v.size(); i++) {
        sum += v[i] - v[i - n];
        ans[i] = sum / n;
    }

    return ans;
}

// v-> vector of prices, n-> size of EMA window, k-> smoothing factor = x/(n+1)
std::vector<double> ExponentialMovingAverage(std::vector<double> &v, int n, double x) {
    double k = x / (n + 1), ema = v[0];
    std::vector<double> ans(v.size());
    ans[0] = ema;

    for (int i = 1; i < v.size(); i++) {
        ema = (v[i] - ema) * k + ema;
        ans[i] = ema;
    }

    return ans;
}

// v-> vector of prices, w-> weights for WMA
std::vector<double> WeightedMovingAverage(std::vector<double> &v, std::vector<double> &w) {
    std::vector<double> ans(v.size());
    double sum = 0;
    for (int i = 0; i < w.size(); i++) {
        sum += w[i];
        ans[i] = std::nan(""); // not a number
    }

    for (int i = w.size() - 1; i < v.size(); i++) {
        double temp = 0;
        for (int j = 0; j < w.size(); j++) {
            temp += v[i - j] * w[j];
        }
        ans[i] = temp / sum;
    }

    return ans;
}

// smoothning factor = x/(n+1) for EMA
// EMA1-> fast EMA, EMA2-> slow EMA
// signal line is MA of MACD (n3), hist is MACD-EMA(signal line), 0 for i<n3-1
// hist > 0 -> bullish, hist < 0 -> bearish
std::vector<double> MACD(std::vector<double> &price, int n1, int n2, int n3, double x) {
    double ema1 = price[0], ema2 = price[0];
    std::vector<double> MACD(price.size());
    MACD[0] = ema1 - ema2;

    for (int i = 1; i < price.size(); i++) {
        ema1 = (price[i] - ema1) * x / (n1 + 1) + ema1;
        ema2 = (price[i] - ema2) * x / (n2 + 1) + ema2;
        MACD[i] = ema1 - ema2;
    }

    std::vector<double> signal(price.size()), hist(price.size());
    for (int i = n3 - 1; i < MACD.size(); i++) {
        double sum = 0;
        for (int j = i - n3 + 1; j <= i; j++) {
            sum += MACD[j];
        }
        signal[i] = sum / n3;
        hist[i] = MACD[i] - signal[i];
    }

    return hist;
}

// ATR = EMA of TR (n), smoothening factor = x/(n+1)
// TR = max(high-low, abs(high-prev_close), abs(low-prev_close))
std::vector<double> AverageTrueRange(std::vector<double> &high, std::vector<double> &low, std::vector<double> &close, int n, double x) {
    double tr = high[0] - low[0], atr = tr;
    std::vector<double> ATR(high.size());
    ATR[0] = atr;

    for (int i = 1; i < high.size(); i++) {
        tr = std::max(std::max(high[i] - low[i], std::abs(high[i] - close[i - 1])), std::abs(low[i] - close[i - 1]));
        atr = (tr - atr) * x / (n + 1) + atr;
        ATR[i] = atr;
    }

    return ATR;
}

// Bollinger Bands from MA - k*std to MA + k*std
std::vector<int> BollingerBands(std::vector<double> &price, int n, double k) {
    std::vector<int> BB(price.size());
    double ma, std;

    for (int i = n - 1; i < price.size(); i++) {
        double sum = 0;
        for (int j = i - n + 1; j <= i; j++) {
            sum += price[j];
        }
        ma = sum / n;

        sum = 0;
        for (int j = i - n + 1; j <= i; j++) {
            sum += (price[j] - ma) * (price[j] - ma);
        }
        std = std::sqrt(sum / n);

        if (price[i] > ma + k * std) {
            BB[i] = 1;
        } else if (price[i] < ma - k * std) {
            BB[i] = -1;
        } // else BB[i] = 0;
    }

    return BB;
}
