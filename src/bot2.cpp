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


default_random_engine rd(time(0));
// random_device rd;
std::uniform_int_distribution<int> orderType(0,1);
std::uniform_int_distribution<int> buySell(0,1);
std::uniform_int_distribution<int> quantity(1,10);
std::uniform_int_distribution<int> price(100,200);


void randomOrder(){
    cout<<orderType(rd)<<" "<<buySell(rd)<<" "<<price(rd)<<" "<<quantity(rd)<<endl;
}

void order(int x, int y, int z, int d){
    cout<<x<<" "<<y<<" "<<z<<" "<<d<<endl;
}

vector<int> parse(string s){// parses the output string to a vector of parameters
    vector<int> ans;
    string temp = "";
    vector<string> sts;
    for(int i = 0; i<s.size(); i++){
        if(s[i] != ' '){
            temp.push_back(s[i]);
        }
        else{
            sts.push_back(temp);
            temp = "";
        }
    }
    for(auto thing : temp){
        ans.push_back(atof(temp.c_str()));
    }

    
    return ans;
}



void refresh(){
    freopen("output.txt", "a",stdout); 
}
deque<int> mktPrices = {1};
int main(){

    // freopen("output.txt","w",stdout);

    while(true){
        string s; 
        getline(cin, s);
        if(s.size() == 0) continue;
        auto v = parse(s);
        mktPrices.push_back(v[5]);
        if(mktPrices.size() > 10){mktPrices.pop_front();}
        if((v[5] - mktPrices.back())/mktPrices.back() >= 1){ // if the stock price doubles, the bot will support that rise
                order(1, 1, 0, 1);    
        }
        if((v[5] - mktPrices.back())/mktPrices.back() <= -1){ // if the stock price halves, the bot will support that fall
                order(1, 0, 0, 1); 
        }
        
        
        
        
        
        
        // cout<<"HI"<<endl;
        // refresh(); //optional, prolly comment out
    }


return 0;
}