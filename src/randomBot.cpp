#include <chrono>
#include <ctime>
#include <iostream>
#include <map>
#include <set>
#include <windows.h>
#include <thread>
#include <random>
using namespace std;

vector<int> parse(){
    vector<int> ans;
    // parses the output string to a vector of quantities
    return ans;
}

default_random_engine rd(time(0));
// random_device rd;
std::uniform_int_distribution<int> orderType(0,1);
std::uniform_int_distribution<int> buySell(0,1);
std::uniform_int_distribution<int> quantity(1,10);
std::uniform_int_distribution<int> price(100,200);
void randomOrder(){
    cout<<orderType(rd)<<" "<<buySell(rd)<<" "<<price(rd)<<" "<<quantity(rd)<<endl;
}
int main(){
// freopen("output.txt","w",stdout);
int periodInSeconds = 5;
while(true){
    randomOrder();
    Sleep(periodInSeconds*1000);
}




return 0;
}