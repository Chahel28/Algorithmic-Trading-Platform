#include <chrono>
#include <ctime>
#include <iostream>
#include <map>
#include <set>
#include <windows.h>
#include <thread>
#include <deque>
#include <string>
#include <random>
using namespace std;
// void refresh(){
//     freopen("output.txt", "a",stdout); 
// }
// long long ask_price, ask_quantity, bid_price, bid_quantity, total_quantity_ask, total_quantity_bid;
// pending_orders.push(Limit_Order(buy, price, quantity, Get_Time()));
class tempClass{
    public:
  long long ask_price, ask_quantity, bid_price, bid_quantity, total_quantity_ask, total_quantity_bid; 
};
tempClass _exchange;

//-------------------COPY THE CODE FROM HERE-----------------------------------------------------------------------------------------------------------------
default_random_engine rd(time(0));
std::uniform_int_distribution<int> orderTypeDist(0,1);
std::uniform_int_distribution<int> buySellDist(0,1);
std::uniform_int_distribution<int> quantityDist(1,10);
std::uniform_int_distribution<int> priceDist(100,200);
std::uniform_real_distribution<float> pickDist(0,1);
void randomOrder(){
    cout<<orderTypeDist(rd)<<" "<<buySellDist(rd)<<" "<<priceDist(rd)<<" "<<quantityDist(rd)<<endl;
}

void order(int x, int y, int z, int d){
    cout<<x<<" "<<y<<" "<<z<<" "<<d<<endl;
}
double normalCDF(double value){
    return 0.5*erfc(-value*sqrtl(0.5));
}
double calcMean(deque<double> v){ // returns the mean of a deque of numbers
    double sum = 0;
    for(int i = 0; i<v.size(); i++){
        sum += v[i];
    }
    sum /= v.size();
    return sum;
}
double calcSd(deque<double> v){ // returns the standard deviation of a deque of numbers
    double sd = 0;
    auto mean = calcMean(v);
    for(int i = 0; i<v.size(); i++){
        sd += (v[i] - mean)*(v[i] - mean);
    }
    sd /= v.size();
    sd = sqrtl(sd);
    return sd;
}
void gaussianBot(int n = 5){// n is the size of the window 
    double prevBids = -1, prevAsks = -1;
    deque<double> mktBids(n);
    deque<double> mktAsks(n);
    iota(mktAsks.begin(), mktAsks.end(), 100);
    iota(mktBids.begin(), mktBids.end(), 100);
    double meanBids = calcMean(mktBids), sdBids = calcSd(mktBids);
    double meanAsks = calcMean(mktAsks), sdAsks = calcSd(mktAsks);
    while(true){
        if(_exchange.ask_price != prevAsks){
            auto z = (_exchange.ask_price - calcMean(mktAsks))/calcSd(mktAsks);
            if(z < 0){z = -z;}
            auto acceptProbability = normalCDF(z) + 0.5;
            auto p = pickDist(rd);
            if(p <= acceptProbability){// accept the ask
                pending_orders.push(Limit_Order(buy, _exchange.ask_price, quantityDist(rd), Get_Time()));
            }
            prevAsks = _exchange.ask_price;
            mktAsks.push_back(_exchange.ask_price);
            mktAsks.pop_front();
        }
        if(_exchange.bid_price != prevBids){
            auto z = (_exchange.bid_price - calcMean(mktBids))/calcSd(mktBids);
            if(z < 0){z = -z;}
            auto acceptProbability = normalCDF(z) + 0.5;
            auto p = pickDist(rd);
            if(p <= acceptProbability){// accept the bid
                pending_orders.push(Limit_Order(sell, _exchange.bid_price, quantityDist(rd), Get_Time()));
            } 
            prevBids = _exchange.bid_price;
            mktBids.push_back(_exchange.bid_price);
            mktBids.pop_front();            
        }


    }
}


//---------------END HERE--------------------------------------------------------------------------------------------------------------------------

int main(){
    freopen("output.txt","w",stdout);
return 0;
}
/*
Broadcast Format:
T AP AQ BP BQ FP FQ
T -> Time in milliseconds from the start of the exchange
AP -> Ask Price
AQ -> Ask Quantity
BP -> Bid Price
BQ -> Bid Quantity
FP -> Fill Price
FQ -> Fill Quantity
(If no fill, FP and FQ are not present)
(If multiple fills, multiple lines are printed)
*/
