#pragma once

#include <vector>

// v-> vector of prices, n-> size of MA window
template <typename T>
std::vector<T> MovingAverage(std::vector<T> &v, int n) {
    std::vector<T> ans;
    T sum = 0;
    for (int i = 0; i < n; i++) {
        sum += v[i];
    }
    ans.push_back(sum / n);

    for (i = n; i < v.size(); i++) {
        sum += v[i] - v[i - n];
        ans.push_back(sum / n);
    }

    return ans;
}

// v-> vector of prices, n-> size of EMA window, k-> smoothing factor = x/(n+1)
template <typename T, typename U>
std::vector<T> ExponentialMovingAverage(std::vector<T> &v, int n, U x) {
    T k = (double)x / (n + 1);
    T ema = v[0];
    std::vector<T> ans;
    ans.push_back(ema);

    for (int i = 1; i < v.size(); i++) {
        ema = (v[i] - ema) * k + ema;
        ans.push_back(ema);
    }

    return ans;
}

// v-> vector of prices, w-> weights for WMA
template <typename T, typename U>
std::vector<T> WeightedMovingAverage(std::vector<T> &v, std::vector<U> &w) {
    std::vector<T> ans;
    T sum = 0;
    for (int i = 0; i < w.size(); i++) {
        sum += w[i];
    }

    for (int i = w.size() - 1; i < v.size(); i++) {
        T temp = 0;
        for (int j = 0; j < w.size(); j++) {
            temp += v[i - j] * w[j];
        }
        ans.push_back(temp / sum);
    }

    return ans;
}

// smoothning factor = x/(n+1) for EMA
// EMA1-> fast EMA, EMA2-> slow EMA
// signal line is MA of MACD (n3), hist is MACD-EMA(signal line), 0 for i<n3-1
template <typename T>
std::vector<T> MACD(std::vector<T> &price, int n1, int n2, int n3, int x) {
    std::vector<T> MACD;
    T ema1 = price[0], ema2 = price[0];
    MACD.push_back(ema1 - ema2);

    for (int i = 1; i < price.size(); i++) {
        ema1 = (price[i] - ema1) * x / (n1 + 1) + ema1;
        ema2 = (price[i] - ema2) * x / (n2 + 1) + ema2;
        MACD.push_back(ema1 - ema2);
    }

    std::vector<T> signal, hist;
    for (int i = 0; i < n3 - 1; i++) {
        signal.push_back(0);
        hist.push_back(0);
    }

    for (int i = n3 - 1; i < MACD.size(); i++) {
        T sum = 0;
        for (int j = i - n3 + 1; j <= i; j++) {
            sum += MACD[j];
        }
        signal.push_back(sum / n3);
        hist.push_back(MACD[i] - signal[i]);
    }

    return hist;
}

// ATR = EMA of TR (n), smoothening factor = x/(n+1)
// TR = max(high-low, abs(high-prev_close), abs(low-prev_close))
template <typename T>
std::vector<T> AverageTrueRange(std::vector<T> &high, std::vector<T> &low, std::vector<T> &close, int n, int x) {
    T tr = high[i] - low[i], atr = tr;
    std::vector<T> ATR = {atr};

    for (int i = 1; i < high.size(); i++) {
        tr = max(max(high[i] - low[i], abs(high[i] - close[i - 1])), abs(low[i] - close[i - 1]));
        atr = (tr - atr) * x / (n + 1) + atr;
        ATR.push_back(atr);
    }

    return ATR;
}

// Bollinger Bands from MA - k*std to MA + k*std, set k = 2 for default
template <typename T, typename U>
std::vector<T> BollingerBands(std::vector<T> &price, int n, U k) {
    std::vector<T> BB, upper, lower;
    T ma, std;
    for (int i = 0; i < n - 1; i++) {
        BB.push_back(0);
        upper.push_back(0);
        lower.push_back(0);
    }

    for (int i = n - 1; i < price.size(); i++) {
        T sum = 0;
        for (int j = i - n + 1; j <= i; j++) {
            sum += price[j];
        }
        ma = sum / n;

        sum = 0;
        for (int j = i - n + 1; j <= i; j++) {
            sum += (price[j] - ma) * (price[j] - ma);
        }

        std = sqrt(sum / n);
        BB.push_back((price[i] - ma) / (k * std));
        upper.push_back(ma + k * std);
        lower.push_back(ma - k * std);
    }

    return BB;
}
