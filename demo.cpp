#include "MACD, ATR, BB.h"

int main(){
    //n -> total number of days
    int n;
    cin >> n;

    //for Bolinger Bands and MACD
    vector<double> price(n);
    for(int i = 0; i < n; i++){
        cin >> price[i];
    }

    //for ATR
    vector<double> high(n), low(n), close(n);
    for(int i = 0; i < n; i++){
        cin >> high[i] >> low[i] >> close[i];
    }

    vector<double> w = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; //sample weights for WMA

    vector<double> ans = MovingAverage(price, 10);
    for(int i = 0; i < ans.size(); i++){
        cout << ans[i] << " ";
    }
    cout << endl;

    ans = ExponentialMovingAverage(price, 10, 2);
    for(int i = 0; i < ans.size(); i++){
        cout << ans[i] << " ";
    }
    cout << endl;

    ans = WeightedMovingAverage(price, w);
    for(int i = 0; i < ans.size(); i++){
        cout << ans[i] << " ";
    }
    cout << endl;


    ans = MACD(price, 12, 26, 9, 2);
    for(int i = 0; i < ans.size(); i++){
        cout << ans[i] << " ";
    }
    cout << endl;

    
    ans = AverageTrueRange(high, low, close, 14, 2);
    for(int i = 0; i < ans.size(); i++){
        cout << ans[i] << " ";
    }
    cout << endl;

    ans = BollingerBands(price, 20, 2);
    for(int i = 0; i < ans.size(); i++){
        cout << ans[i] << " ";
    }
    cout << endl;
    return 0;
}
