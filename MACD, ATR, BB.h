#include<bits/stdc++.h>

using namespace std;

//v-> vector of prices, n-> size of MA window
template <typename T> vector<T> MovingAverage(vector<T> &v, int n) {
    vector<T> ans;
    int i = 0, j = 0;
    T sum = 0;
    for (; j+1<n; j++) sum+=v[j];
    
    for (j = n-1; j<v.size(); j++){
        sum+=v[j];
        ans.push_back(sum / n);
        sum -= v[i];
        i++;
    }
    return ans;
}

//v-> vector of prices, n-> size of EMA window, k-> smoothing factor = x/(n+1)
template <typename T, typename U> vector<T> ExponentialMovingAverage(vector<T> &v, int n, U x) {
    vector<T> ans;
    T k = (double) x / (n + 1);
    T ema = v[0];
    ans.push_back(ema);
    for (int i = 1; i < v.size(); i++) {
        ema = (v[i] - ema) * k + ema;
        ans.push_back(ema);
    }
    return ans;
}

//v-> vector of prices, w-> weights for WMA
template <typename T, typename U> vector<T> WeightedMovingAverage(vector<T> &v, vector<U> &w) {
    vector<T> ans;
    T sum = 0;
    for (int i = 0; i < w.size(); i++) {
        sum += w[i];
    }
    for (int i = w.size()-1; i < v.size(); i++) {
        T temp = 0;
        for (int j = 0; j < w.size(); j++) {
            temp += v[i - j] * w[j];
        }
        ans.push_back(temp / sum);
    }
    return ans;
}

//smoothning factor = x/(n+1) for EMA
//EMA1-> fast EMA, EMA2-> slow EMA
//signal line is MA of MACD (n3), hist is MACD-EMA(signal line), 0 for i<n3-1
template <typename T> vector<T> MACD(vector<T> &price, int n1, int n2, int n3, int x){
    vector<T> EMA1, EMA2, MACD, signal, hist;
    T ema1, ema2;

    for(int i = 0; i < price.size(); i++){
        if(i == 0){
            ema1 = price[i];
            ema2 = price[i];
        }
        else{
            ema1 = (price[i] - ema1) * x / (n1 + 1) + ema1;
            ema2 = (price[i] - ema2) * x / (n2 + 1) + ema2;
        }
        EMA1.push_back(ema1);
        EMA2.push_back(ema2);
        MACD.push_back(ema1 - ema2);
    }

    for(int i = 0; i < MACD.size(); i++){
        if(i < n3 - 1){
            signal.push_back(0);
            hist.push_back(0);
        }
        else{
            T sum = 0;
            for(int j = i - n3 + 1; j <= i; j++){
                sum += MACD[j];
            }
            signal.push_back(sum / n3);
            hist.push_back(MACD[i] - signal[i]);
        }
    }
    return hist;
}

//ATR = EMA of TR (n), smoothening factor = x/(n+1)
//TR = max(high-low, abs(high-prev_close), abs(low-prev_close))
template <typename T> vector<T> AverageTrueRange(vector<T> &high, vector<T> &low, vector<T> &close, int n, int x){
    vector<T> ATR;
    T tr, atr;
    for(int i = 0; i < high.size(); i++){
        if(i == 0){
            tr = high[i] - low[i];
            atr = tr;
        }
        else{
            tr = max(max(high[i] - low[i], abs(high[i] - close[i - 1])), abs(low[i] - close[i - 1]));
            atr = (tr - atr) * x / (n + 1) + atr;
        }
        ATR.push_back(atr);
    }
    return ATR;
}

//Bollinger Bands from MA - k*std to MA + k*std, set k = 2 for default
template <typename T, typename U> vector<T> BollingerBands(vector<T> &price, int n, U k){
    vector<T> BB, upper, lower;
    T ma, std;
    for(int i = 0; i < price.size(); i++){
        if(i < n - 1){
            BB.push_back(0);
            upper.push_back(0);
            lower.push_back(0);
        }
        else{
            T sum = 0;
            for(int j = i - n + 1; j <= i; j++){
                sum += price[j];
            }
            ma = sum / n;
            sum = 0;
            for(int j = i - n + 1; j <= i; j++){
                sum += (price[j] - ma) * (price[j] - ma);
            }
            std = sqrt(sum / n);
            BB.push_back((price[i] - ma) / (k * std));
            upper.push_back(ma + k * std);
            lower.push_back(ma - k * std);
        }
    }
    return BB;
}
