#include<bits/stdc++.h>
using namespace std;

template <typename T> vector<T> RSI(T period, vector<T> &close, int n) {
    if(period < 1) {
        cout << "Please give an integer value for period >=1";
        return vector<T>();
    }
    vector<T>diffClose(n);
    adjacent_difference(close.begin(), close.end(), diffClose);
    diffClose[0] = 0;
    vector<T>gain(n), loss(n);
    for(int i=0; i<n; i++) {
        gain[i] = (diffClose[i] >= 0) ? diffClose[i] : 0;
        loss[i] = (diffClose[i] < 0) ? abs(diffClose[i]) : 0;
    }
    int floor_period = floor(period);
    T avg_gain = 0, avg_loss = 0;
    for(int i=0; i<period; i++) {
        avg_gain += gain[i];
        avg_loss += loss[i];
    }
    avg_gain /= period;
    avg_loss /= period;

    vector<T>RSI_val;
    int RS;
    for(int i=floor_period; i<n; i++){
        RS = avg_gain/avg_loss;
        RSI_val.push_back(100 - 100/(1+RS));

        avg_gain = (avg_gain*(floor_period-1) + gain[i])/floor_period;
        avg_gain = (avg_gain*(floor_period-1) + gain[i])/floor_period;
    }
    return RSI_val;
}

template <typename T> pair<vector<T>, pair<vector<T>, vector<T>>> ADX_DI(vector<T> &high, vector<T> &low, vector<T> &close, int n) {
    vector<T>DM_plus(n), DM_minus(n);
    adjacent_difference(high.begin(), high.end(), DM_plus);
    adjacent_difference(low.begin(), low.end(), DM_minus);
    for(int i=0; i<n; i++) {
        DM_plus[i] = max(DM_plus[i], 0);
        DM_minus[i] = max(-1*DM_minus[i], 0);
    }

    vector<T>TR;
    for(int i=0; i<n-1; i++){
        TR.push_back(max(high[i+1]-low[i+1], max(abs(high[i+1]- close[i]), abs(low[i+1]- close[i]))));
    }

    vector<T> avg_tr;
    avg_tr.push_back(0);
    for(int i=0; i<14; i++){
        avg_tr[0] += TR[i];
    }
    
    int k=0;
    for(int i=14; i<n-1; i++){
        avg_tr.push_back(avg_tr[k]*(13/14)+TR[i]);
        k++;
    }

    vector<T>DI_plus, DI_minus;

    DI_plus.push_back(0);
    DI_minus.push_back(0);
    for(int i=1; i<15; i++){
        DI_plus[0]+=DM_plus[i];
        DI_minus[0]+=DM_minus[i];
    }

    k=0;
    for(int i=15; i<n; i++){
        DI_plus.push_back(DI_plus[k]*(13/14)+DM_plus[i]);
        DI_minus.push_back(DI_minus[k]*(13/14)+DM_minus[i]);
        k++;
    }

    vector<T>DX;
    for(int i=0; i<DI_plus.size(); i++){
        DI_plus[i] = DI_plus[i]*100/avg_tr[i];
        DI_minus[i] = DI_minus[i]*100/avg_tr[i];
        DX.push_back(abs(DI_plus[i]-DI_minus[i])*100/abs(DI_plus[i]+DI_minus[i]));
    }

    vector<T> ADX;
    ADX.push_back(0);
    for(int i=0; i<14; i++){
        ADX[0]+= DX[i];
    }
    k=0;
    for(int i=14; i<DX.size(); i++){
        ADX.push_back((ADX[k]*(13)+DX[i])/14);
        k++;
    }

    pair<vector<T>, vector<T>> DI(DI_plus, DI_minus);
    pair<vector<T>, pair<vector<T>, vector<T>>> ADX_DI(ADX, DI);

    return ADX_DI;

}

int main() {
    return 0;
}