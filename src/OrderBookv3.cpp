#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <iomanip>
#include <fstream>
using namespace std;
// #define int long long

template <typename T> void disp(vector<T> v){ // just to display a vector
    for(auto thing : v){
        cout<<thing<<" ";
    }
    cout<<endl;
}
template <typename T, typename U> void disp(vector<pair<T,U>> v){ // just to display a vector
    for(auto thing : v){
        cout<<thing.first<<" "<<thing.second<<endl;
    }
    cout<<endl;
}


void clear(){
    freopen("output.txt", "w",stdout); // output.txt is the output file where all the stuff happens
}

int timeNumber = 0; // my own time, created by me. It increases by 1 after every bid or ask

class buyOrder{ // buyOrder class. Object of this class represents a buy order
    public:
    // string orderID;  
    int orderQuantity; 
    int price;
    int timeNumber;
};

class sellOrder{ // sellOrder class. Object of this class represents a sell order
    public: 
    // string orderID;
    int orderQuantity;
    int price;
    int timeNumber;
};

class fifoBuyComparator{ // comparator function for bids priority queue
    public:
    bool operator()(buyOrder A, buyOrder B){
    if(A.price != B.price){
        if(A.price < B.price){ //greater price, greater priority
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(A.timeNumber > B.timeNumber){ //earlier, more priority
            return true;
        }
        else{
            return false;
        }
    }
}    
};

class fifoSellComparator{ // comparator funciton for asks priority queue
    public:
    bool operator()(sellOrder A, sellOrder B){
    if(A.price != B.price){
        if(A.price > B.price){ // lesser price, higher priority
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(A.timeNumber > B.timeNumber){ // earlier, higher priority
            return true; 
        }
        else{
            return false;
        }
    }
}
};

void showpq(priority_queue<buyOrder, vector<buyOrder>, fifoBuyComparator> p){// function to display bids priority queue
    while(p.size()){
        cout<< p.top().price <<" "<<p.top().orderQuantity<<" "<<p.top().timeNumber<<endl;
        p.pop();
    }
    
}
void showpq(priority_queue<sellOrder, vector<sellOrder>, fifoSellComparator> p){// overloaded function to display asks priority queue
    while(p.size()){
        cout<<p.top().price<<" "<<p.top().orderQuantity<<" "<<p.top().timeNumber<<endl;
        p.pop();
    }
}

priority_queue<buyOrder, vector<buyOrder>, fifoBuyComparator> bids; // declaration of the bids priority queue
priority_queue<sellOrder, vector<sellOrder>, fifoSellComparator> asks; // declaration of the asks priority queue

vector<pair<int,int>> matchedOrders; // stores the prices at which orders happened

void showOrderBook(){ // displays currently active bids and ask prices, along with matched orders
    cout<<"Bids :"<<endl;
    if(bids.size()){showpq(bids);}
    cout<<endl;
    cout<<"Asks :"<<endl;
    if(asks.size()){showpq(asks);}
    cout<<endl;
    cout<<"Number of Matched Orders: "<<matchedOrders.size()<<endl;
    cout<<"Matched Order Prices & Quantities: "<<endl;
    if(matchedOrders.size()){disp(matchedOrders);}
}


void matchFifo(){ // calling this function runs the Fifo matching algorithm. This function is "automatically" run when a buy order/sell order is placed
    clear();
    showOrderBook();
    if(bids.size() == 0 || asks.size() == 0){return;} // so you wont have some access error
    // The below Matching Code is for FIFO
    if(bids.top().price == asks.top().price && bids.size() != 0 && asks.size() != 0){ // checks if any order can happen, i.e. if the price of the best buy order and best sell order match
        int quantityMatched = 0;
        int priceMatched = asks.top().price;
        if(asks.top().orderQuantity > bids.top().orderQuantity){
            while(asks.top().orderQuantity > bids.top().orderQuantity && asks.top().price == bids.top().price){
                auto temp = asks.top();
                temp.orderQuantity -= bids.top().orderQuantity; quantityMatched += bids.top().orderQuantity;
                asks.pop();
                bids.pop();
                asks.push(temp);
                if(bids.size() == 0 || asks.size() == 0){break;}
            }
            if(bids.size() != 0 && asks.size() != 0){
                if(bids.top().orderQuantity > asks.top().orderQuantity && asks.top().price == bids.top().price){
                    auto temp = bids.top();
                    temp.orderQuantity -= asks.top().orderQuantity; quantityMatched += asks.top().orderQuantity;
                    asks.pop();
                    bids.pop();
                    bids.push(temp);
                }
                else if(asks.top().price == bids.top().price){
                    quantityMatched += bids.top().orderQuantity;
                    bids.pop();
                    asks.pop();
                }
            }
        }
        else if(bids.top().orderQuantity > asks.top().orderQuantity){
            while(bids.top().orderQuantity > asks.top().orderQuantity){
                auto temp = bids.top();
                temp.orderQuantity -= asks.top().orderQuantity; quantityMatched += asks.top().orderQuantity;
                asks.pop();
                bids.pop();
                bids.push(temp);
                if(bids.size() == 0 || asks.size() == 0){break;}
            }
            if(bids.size() != 0 && asks.size() != 0){
                if(bids.top().orderQuantity < asks.top().orderQuantity && asks.top().price == bids.top().price){
                    auto temp = asks.top();
                    temp.orderQuantity -= bids.top().orderQuantity; quantityMatched += bids.top().orderQuantity;
                    asks.pop();
                    bids.pop();
                    asks.push(temp);
                }
                else if(asks.top().price == bids.top().price){
                    quantityMatched += bids.top().orderQuantity;
                    bids.pop();
                    asks.pop();
                }
            }
        }
        else{
            quantityMatched = asks.top().orderQuantity;
            asks.pop();
            bids.pop();
        }
        
        matchedOrders.push_back({priceMatched, quantityMatched});
        // // cout<<"Trade Ahoy! : "<<asks.top().price<<endl;                                                                                
        // matchedOrders.push_back(bids.top().price); // then the order is registered to the list of matched orders
        // bids.pop();// the bid price and ask price at which the trade happened, 
        // asks.pop();// are popped form their priority queues  
    }
    clear();
    showOrderBook();
}

void makeBuyOrder(int buyingprice, int orderquantity){ // function that `places` a buy order. It pushes the buy order object into the bids priority queue
    timeNumber++; // this is so that time increases after every order
    buyOrder orderA; //buyOrder object `orderA` created for this order
    orderA.price = buyingprice; // assigning the value of buying price to the object
    orderA.timeNumber = timeNumber; // assigning the value of time to this object
    orderA.orderQuantity = orderquantity;
    bids.push(orderA); // pushing this object into the bids priority queue
    matchFifo();
}

void makeSellOrder(int sellingprice, int orderquantity){ // similar to the makeSellOrder function
    timeNumber++;
    sellOrder orderA;
    orderA.price = sellingprice;
    orderA.timeNumber = timeNumber;
    orderA.orderQuantity = orderquantity;
    asks.push(orderA);
    matchFifo();
}


void parse(string s){ // parses the input from user, which is a string
    string stNum = ""; // string number
    string stQuantity = "";
    int n = s.length(); int indx = -1;
    for(int i = 1; i<n; i++){
        if(tolower(s[i]) != 'q'){
            stNum.push_back(s[i]);
        }
        else{
            indx = i; break;
        }
    }
    double price = atof(stNum.c_str());
    if(indx == -1){
        stQuantity = "1";
    }
    else{
        for(int i = indx+1; i<n; i++){
            stQuantity.push_back(s[i]);
        }
    }
    int quantity = atoi(stQuantity.c_str());
    if(tolower(s[0]) == 'b'){
        makeBuyOrder(price, quantity);
    }
    else if(tolower(s[0]) == 's'){
        makeSellOrder(price, quantity);
    }
}


#define hi cout<<"HI"<<endl

int32_t main(){
//--------------------
// freopen("input.txt","r",stdin);
freopen("output.txt","w",stdout);
//-------------------
// hi;
// for (int i = 1; i<10; i++){
//     makeBuyOrder(i*i, i);
// }
// makeSellOrder(300,1);
// makeSellOrder(100,20);
// // makeSellOrder(81, 9);
// // makeBuyOrder(100, 10);
// makeSellOrder(81, 2);
// makeBuyOrder(100,11);
// makeBuyOrder(100, 10);
// makeSellOrder(100, 12);

// makeBuyOrder(100,30);
// makeSellOrder(100,10);


// showOrderBook();

while(true){
    string s; cin>>s;
    if(s == "stop"){
       clear();
       cout<<"Stopped"; 
       break;
    }
    else{
        if(tolower(s[0]) == 'b'){
            parse(s);
        }
        else if(tolower(s[0]) == 's'){
            parse(s);
        }
        else{
            clear();
            cout<<"Invalid input, try again\n";
        }
    }
}



return 0;
}
