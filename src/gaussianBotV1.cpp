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
// // long long ask_price, ask_quantity, bid_price, bid_quantity, total_quantity_ask, total_quantity_bid;
// // pending_orders.push(Limit_Order(buy, price, quantity, Get_Time()));
// class tempClass{
//     public:
//   long long ask_price, ask_quantity, bid_price, bid_quantity, total_quantity_ask, total_quantity_bid; 
// };
// tempClass _exchange;

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
double calcMean(deque<double> v){
    double sum = 0;
    for(int i = 0; i<v.size(); i++){
        sum += v[i];
    }
    sum /= v.size();
    return sum;
}
double calcSd(deque<double> v){
    double sd = 0;
    auto mean = calcMean(v);
    for(int i = 0; i<v.size(); i++){
        sd += (v[i] - mean)*(v[i] - mean);
    }
    sd /= v.size();
    sd = sqrtl(sd);
    return sd;
}
void gaussianBot(){
    double prev = -1;
    deque<double> mktPrices(10);
    iota(mktPrices.begin(), mktPrices.end(), 1);
    double mean = calcMean(mktPrices), sd = calcSd(mktPrices);
    while(true){
        if((_exchange.ask_price + _exchange.bid_price)/2.0 != prev){
            auto z = ((_exchange.ask_price + _exchange.bid_price)/2.0 - mean)/sd;
            if(z > 0){z = -z;}
            auto probab = normalCDF(z) + 0.5;
            //now execute something with a certain probability
            auto p = pickDist(rd);
            if(p <= probab){
                pending_orders.push(Limit_Order(buy, _exchange.ask_price, quantityDist(rd), Get_Time()));
                pending_orders.push(Limit_Order(sell, _exchange.bid_price, quantityDist(rd), Get_Time()));
            }
            else{
                //dont match the order, but do add it to the mktPrices
                mktPrices.push_back((_exchange.ask_price + _exchange.bid_price)/2.0);
                mktPrices.pop_front();
            }
            prev = (_exchange.ask_price + _exchange.bid_price)/2.0;
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
