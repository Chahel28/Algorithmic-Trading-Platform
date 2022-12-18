#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
using namespace std;
#define int long long

template <typename T> void disp(vector<T> v){ // just to display a vector
    for(auto thing : v){
        cout<<thing<<" ";
    }
    cout<<endl;
}

template <typename T> vector<T> simpleMovingAverage(vector<T> v, int ws){// `v` array of prices ; `ws` window size
    vector<T> ans;
    T sum = 0;
    for(int i  = 0; i<ws; i++){
        sum += v[i];
    }
    ans.push_back(sum/ws);
    int i = 0;
    for(int j  = ws; j<v.size(); j++){
        sum -= v[i]; sum += v[j];
        ans.push_back(sum/ws);
        i++;
    }    
    return ans;
    
    
}
template <typename T> vector<pair<T,bool>> maCrossover(vector<T>  v, int w1, int w2){// `v` vector of prices, `w1` window of larger MA, `w2` window of smaller MA
    vector<T> smallMA, bigMA; //big and small window moving average vectors
    for(int i = 0; i<min(w1,w2); i++){ 
        smallMA.push_back(v[i]);
    }
    for(int i  = 0; i<max(w1,w2); i++){
        bigMA.push_back(v[i]);
    }
    
    vector<T> tempSA  = simpleMovingAverage(v, min(w1, w2));
    vector<T> tempBA =  simpleMovingAverage(v, max(w1, w2));

    for(auto thing : tempSA){smallMA.push_back(thing);}
    for(auto thing : tempBA){bigMA.push_back(thing);}    
    //Trades will be possible from index `max(w1,w2)` to `v.size() - 1` on smallMA and bigMA

    vector<int> diff; // vector of price differences between bigMA and smallMA
    for(int i = 0; i<v.size(); i++){
        diff.push_back(bigMA[i] - smallMA[i]);
    }
    vector<pair<T, bool>> buySell;
    for(int i = 1; i<diff.size(); i++){
        if(diff[i] < 0 && diff[i-1] > 0){ // buy price , golden cross
            buySell.push_back({v[i], true}); // true implies its a buy signal at price of `v[i]`
        }
        else if(diff[i] > 0 && diff[i-1] < 0){ // sell price, death cross
            buySell.push_back({v[i], false});// false implies its a sell signal at price of `v[i]`
        }
    }
    
return buySell;    
}

