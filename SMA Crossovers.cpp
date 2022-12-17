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
template <typename T> T maCrossover(vector<T>  v, int w1, int w2){// `v` vector of prices, `w1` window of larger MA, `w2` window of smaller MA
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
    vector<int> buySell;
    for(int i = 1; i<diff.size(); i++){
        if(diff[i] < 0 && diff[i-1] > 0){ // buy price , golden cross
            buySell.push_back(v[i]);
        }
        else if(diff[i] > 0 && diff[i-1] < 0){ // sell price, death cross
            buySell.push_back(v[i]);
        }
    }
    int profit = 0;
    //  every even index consists of buy prices, every odd index consists of sell prices
    for(int i = 0; i<(int)buySell.size() - 1; i+=2){
        profit += buySell[i]-buySell[i+1];
    }
    // cerr<<profit;
    
return profit;    
}



int32_t main(){
// vector<int> p = {9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,45,45,78,89,45,6,2134,6,789,789,3,12,2,1,2,5,4,8,98,56,4,64,65,46,465,465,45,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,45,45,78,89,45,6,2134,6,789,789,3,12,2,1,2,5,4,8,98,56,4,64,65,46,465,465,45,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,45,45,78,89,45,6,2134,6,789,789,3,12,2,1,2,5,4,8,98,56,4,64,65,46,465,465,45,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,45,45,78,89,45,6,2134,6,789,789,3,12,2,1,2,5,4,8,98,56,4,64,65,46,465,465,45,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,45,45,78,89,45,6,2134,6,789,789,3,12,2,1,2,5,4,8,98,56,4,64,65,46,465,465,45,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,45,45,78,89,45,6,2134,6,789,789,3,12,2,1,2,5,4,8,98,56,4,64,65,46,465,465,45,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,45,45,78,89,45,6,2134,6,789,789,3,12,2,1,2,5,4,8,98,56,4,64,65,46,465,465,45,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,45,45,78,89,45,6,2134,6,789,789,3,12,2,1,2,5,4,8,98,56,4,64,65,46,465,465,45};
// vector<int> p = {1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
// vector<int> p = {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2};
// cout<< maCrossover(p,2,7) ;


return 0;
}