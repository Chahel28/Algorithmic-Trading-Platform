#include <chrono>
#include <ctime>
#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <queue>
#include <thread>
#include <random>
#include <mutex>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

std::mutex m;

using namespace __gnu_pbds;
using namespace std;

typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

//#define double long 
#define ENDTIME 20000

int gr(int r){
   uniform_int_distribution<int> uid(0,r-1);
   int val = uid(rng);
   return val+1;
}

int glr(int l, int r){
    int dif = r-l+1;
    int x = gr(dif) + l-1;
    return x;
}

/*
FIFO Exchange

Order Format:
0 X P Q
1 X Q

0 -> Limit Order
1 -> Market Order
X = 0/1
0 -> Sell
1 -> Buy
P -> Price
Q -> Quantity


Broadcast Format:
T AP AQ BP BQ FP FQ

T -> Time in milliseconds from the start of the _exchange
AP -> Ask Price
AQ -> Ask Quantity
BP -> Bid Price
BQ -> Bid Quantity
FP -> Fill Price
FQ -> Fill Quantity
(If no fill, FP and FQ are -1 and -1)
(If multiple fills, multiple lines are printed)

If market order is not filled, print "Market order quantity is more than available quantity" and trade with available quantity

Input -1 to end the _exchange
*/

long long time_of_start_of_exchange = 0;
bool bot1_flag = true;
// default_random_engine rd0(time(0));
// std::uniform_int_distribution<int> orderType(0,1);
// std::uniform_int_distribution<int> buySell(0,1);
// std::uniform_int_distribution<int> quantity(1,10);
// std::uniform_int_distribution<int> price(100,200);

using namespace std::chrono;

void Start_Exchange() {
    time_of_start_of_exchange = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

long long Get_Time() {
    auto time = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    return (time - time_of_start_of_exchange);
}

struct Limit_Order {
    bool buy; // true -> buy, false -> sell
    long double price;
    long long quantity, time;
    Limit_Order(bool buy, long double price, long long quantity, long long time) {
        this->buy = buy;
        this->price = price;
        this->quantity = quantity;
        this->time = time;
    }

    // FIFO:
    bool operator<(const Limit_Order &other) const {
        if (buy){
            if (price != other.price) return price > other.price;
            else return time < other.time;
        }
        else{
            if (price != other.price) return price < other.price;
            else return time < other.time;
        }
    }

    /*
    Pro-rata:
    bool operator < (const Limit_Order &other) const{
        if(price != other.price){
            if(buy) return price > other.price;
            else return price < other.price;
        }
        if (quantity != other.quantity) return quantity < other.quantity;
        return time < other.time;
    }
    */
};

struct Broad {
    long double ask_price, bid_price, fill_price;
    long long ask_quantity, bid_quantity, fill_quantity;
    Broad(long double ap, long long aq, long double bp, long long bq, long double fp, long long fq) {
        ask_price = ap;
        ask_quantity = aq;
        bid_price = bp;
        bid_quantity = bq;
        fill_price = fp;
        fill_quantity = fq;
    }
};



std::queue<Limit_Order> pending_orders;
std::queue<Broad> broadcast_queue;


class Exchange {
private:
    std::set<Limit_Order> buy_orders, sell_orders;
    std::map<long double, long long> ask_prices, bid_prices;


    void Update_Market_Values() {
        if (buy_orders.empty()) {
            bid_price = -1;
            bid_quantity = 0;
            total_quantity_bid = 0;
        } else {
            bid_price = buy_orders.begin()->price;
            bid_quantity = bid_prices[bid_price];
        }

        if (sell_orders.empty()) {
            ask_price = 1e15;
            ask_quantity = 0;
            total_quantity_ask = 0;
        } else {
            ask_price = sell_orders.begin()->price;
            ask_quantity = ask_prices[ask_price];
        }
    }

public:
    long double bid_price, ask_price;
    long long ask_quantity, bid_quantity, total_quantity_ask, total_quantity_bid;

    void Broadcast(Broad b) {
        //std::cout << Get_Time() << "\t" << b.ask_price << "\t" << b.ask_quantity << "\t" << b.bid_price << "\t" << b.bid_quantity << "\t" << b.fill_price << "\t" << b.fill_quantity << "\n";
        //std::cout<<Get_Time()<<"\t"<<(b.ask_price + b.bid_price)/2<<"\t"<<(b.ask_price - b.bid_price)<<"\t"<<total_quantity_ask<<"\t"<<total_quantity_bid<<"\n";
        std::ofstream fout("Broadcasts.txt", std::ios::app);
        fout << Get_Time() << "," << b.ask_price << "," << b.ask_quantity << "," << b.bid_price << "," << b.bid_quantity << "," << b.fill_price << "," << b.fill_quantity << "\n";
        fout.close();
    }
    
    void Fill_Market_Order(bool buy, long long quantity) {
        long double fill_price = -1;
        long long fill_quantity = 0, temp_quantity, current_quantity, temp_time;
        if (buy) {
            if (quantity > total_quantity_ask) {
                quantity = total_quantity_ask;
                std::cout << "Market order quantity is more than available quantity\n";
            }

            while (!sell_orders.empty() && quantity > 0) {
                auto it = sell_orders.begin();
                fill_price = it->price;
                current_quantity = it->quantity;
                temp_time = it->time;
                temp_quantity = std::min(current_quantity, quantity);
                quantity -= temp_quantity;
                total_quantity_ask -= temp_quantity;
                current_quantity -= temp_quantity;
                fill_quantity += temp_quantity;
                if (current_quantity == 0) {
                    sell_orders.erase(it);
                    ask_prices[fill_price] -= temp_quantity;
                    Update_Market_Values();
                    if (ask_prices[fill_price] == 0) {
                        ask_prices.erase(fill_price);
                        //Broadcast(fill_price, fill_quantity);
                        broadcast_queue.push(Broad(ask_price, ask_quantity, bid_price, bid_quantity, fill_price, fill_quantity));
                        fill_quantity = 0;
                    }
                } else {
                    sell_orders.erase(it);
                    sell_orders.insert(Limit_Order(false, fill_price, current_quantity, temp_time));
                    ask_prices[fill_price] -= temp_quantity;
                    Update_Market_Values();
                }
            }
            if (fill_quantity > 0) //Broadcast(fill_price, fill_quantity);
                broadcast_queue.push(Broad(ask_price, ask_quantity, bid_price, bid_quantity, fill_price, fill_quantity));
        } else {
            if (quantity > total_quantity_bid) {
                quantity = total_quantity_bid;
                std::cout << "Market order quantity is more than available quantity\n";
            }

            while (!buy_orders.empty() && quantity > 0) {
                auto it = buy_orders.begin();
                fill_price = it->price;
                current_quantity = it->quantity;
                temp_time = it->time;
                temp_quantity = std::min(current_quantity, quantity);
                quantity -= temp_quantity;
                total_quantity_bid -= temp_quantity;
                current_quantity -= temp_quantity;
                fill_quantity += temp_quantity;
                if (current_quantity == 0) {
                    buy_orders.erase(it);
                    bid_prices[fill_price] -= temp_quantity;
                    Update_Market_Values();
                    if (bid_prices[fill_price] == 0) {
                        bid_prices.erase(fill_price);
                        //Broadcast(fill_price, fill_quantity);
                        broadcast_queue.push(Broad(ask_price, ask_quantity, bid_price, bid_quantity, fill_price, fill_quantity));
                        fill_quantity = 0;
                    }
                } else {
                    buy_orders.erase(it);
                    buy_orders.insert(Limit_Order(true, fill_price, current_quantity, temp_time));
                    bid_prices[fill_price] -= temp_quantity;
                    Update_Market_Values();
                }
            }
            if (fill_quantity > 0) //Broadcast(fill_price, fill_quantity);
                broadcast_queue.push(Broad(ask_price, ask_quantity, bid_price, bid_quantity, fill_price, fill_quantity));
        }
    }

    void Add_Limit_Order(bool buy, long double price, long long quantity, long long time) {
        bool broadcast = false;
        if (buy) {
            if (price >= ask_price) {
                Match(buy, price, quantity, time);
                return;
            }
            buy_orders.insert(Limit_Order(buy, price, quantity, time));
            bid_prices[price] += quantity;
            total_quantity_bid += quantity;
            if (price >= bid_price) broadcast = true;
        } else {
            if (price <= bid_price) {
                Match(buy, price, quantity, time);
                return;
            }
            sell_orders.insert(Limit_Order(buy, price, quantity, time));
            ask_prices[price] += quantity;
            total_quantity_ask += quantity;
            if (price <= ask_price) broadcast = true;
        }
        if (broadcast) {
            Update_Market_Values();
            // Broadcast();
            broadcast_queue.push(Broad(ask_price, ask_quantity, bid_price, bid_quantity, -1, -1));
        }
    }

    void Match(bool buy, long double price, long long quantity, long long time) {
        long double fill_price = -1;
        long long fill_quantity = 0, temp_quantity, current_quantity, temp_time;

        if (buy) {
            while (!sell_orders.empty() && quantity > 0) {
                auto it = sell_orders.begin();
                if (it->price > price) break;
                fill_price = it->price;
                temp_time = it->time;
                current_quantity = it->quantity;
                temp_quantity = std::min(current_quantity, quantity);
                quantity -= temp_quantity;
                total_quantity_ask -= temp_quantity;
                current_quantity -= temp_quantity;
                fill_quantity = temp_quantity;
                if (current_quantity <= 0) {
                    sell_orders.erase(it);
                    ask_prices[fill_price] -= fill_quantity;
                    Update_Market_Values();
                    if (ask_prices[fill_price] <= 0) {
                        ask_prices.erase(fill_price);
                        //Broadcast(fill_price, fill_quantity);
                        Update_Market_Values();
                        broadcast_queue.push(Broad(ask_price, ask_quantity, bid_price, bid_quantity, fill_price, fill_quantity));
                        fill_quantity = 0;
                    }
                } else {
                    sell_orders.erase(it);
                    sell_orders.insert(Limit_Order(false, fill_price, current_quantity, temp_time));
                    ask_prices[fill_price] -= fill_quantity;
                    Update_Market_Values();
                }
            }
            if (quantity > 0)
                Add_Limit_Order(buy, price, quantity, time);
            else if (fill_quantity > 0) //Broadcast(fill_price, fill_quantity);
                broadcast_queue.push(Broad(ask_price, ask_quantity, bid_price, bid_quantity, fill_price, fill_quantity));
        } else {
            while (!buy_orders.empty() && quantity > 0) {
                auto it = buy_orders.begin();
                if (it->price < price) break;
                fill_price = it->price;
                temp_time = it->time;
                current_quantity = it->quantity;
                temp_quantity = std::min(current_quantity, quantity);
                quantity -= temp_quantity;
                total_quantity_bid -= temp_quantity;
                current_quantity -= temp_quantity;
                fill_quantity = temp_quantity;
                if (current_quantity <= 0) {
                    buy_orders.erase(it);
                    bid_prices[fill_price] -= fill_quantity;
                    Update_Market_Values();
                    if (bid_prices[fill_price] <= 0) {
                        bid_prices.erase(fill_price);
                        //Broadcast(fill_price, fill_quantity);
                        Update_Market_Values();
                        broadcast_queue.push(Broad(ask_price, ask_quantity, bid_price, bid_quantity, fill_price, fill_quantity));
                        fill_quantity = 0;
                    }
                } else {
                    buy_orders.erase(it);
                    buy_orders.insert(Limit_Order(true, fill_price, current_quantity, temp_time));
                    bid_prices[fill_price] -= fill_quantity;
                    Update_Market_Values();
                }
            }
            if (quantity > 0)
                Add_Limit_Order(buy, price, quantity, time);
            else if (fill_quantity > 0) //Broadcast(fill_price, fill_quantity);
                broadcast_queue.push(Broad(ask_price, ask_quantity, bid_price, bid_quantity, fill_price, fill_quantity));
        }
    }

    Exchange() {
        ask_price = 1e15;
        bid_price = -1;
        total_quantity_ask = total_quantity_bid = 0;
        ask_quantity = bid_quantity = 0;
        ask_prices.clear();
        bid_prices.clear();
        buy_orders.clear();
        sell_orders.clear();
        // std::ifstream fin("Pending_Orders.txt");
        // while(fin){
        //     bool buy;
        //     long double price;
        //     long long quantity, time;
        //     fin >> buy >> price >> quantity >> time;
        //     if(fin) Add_Limit_Order(buy, price, quantity, time);
        // }
        // fin.close();
    }

    // ~Exchange(){
    //     std::cout << "Exchange is Closing; All Pending Orders are being saved to Pending_Orders.txt" << '\n';
    //     std::cout << "Last Broadcasted Values: " << bid_price << " " << bid_quantity << " " << ask_price << " " << ask_quantity << '\n';
    //     std::ofstream fout("Pending_Orders.txt");
    //     for(auto it:buy_orders) fout << it.buy << "\t" << it.price << "\t" << it.quantity << "\t" << it.time << '\n';
    //     for(auto it:sell_orders) fout << it.buy << "\t" << it.price << "\t" << it.quantity << "\t" << it.time << '\n';
    //     fout.close();
    //     buy_orders.clear();
    //     sell_orders.clear();
    //     ask_prices.clear();
    //     bid_prices.clear();
    // }
};

Exchange _exchange;

void Book_Updater () {
    while (true) {
        if (m.try_lock()){
            if (!pending_orders.empty()) {
                auto order = pending_orders.front();
                pending_orders.pop();
                bool buy = order.buy; 
                long double price = order.price;
                long long quantity = order.quantity, time = order.time;
                if (order.price == -2) _exchange.Fill_Market_Order(buy, quantity);
                else _exchange.Add_Limit_Order(buy, price, quantity, time);
            }
            m.unlock();
        }
        //this_thread::sleep_for(chrono::milliseconds(1));
        this_thread::sleep_for(chrono::microseconds(50));
    }
}

void Broadcaster () {
    while (true) {
        if (!broadcast_queue.empty()) {
            auto broad = broadcast_queue.front();
            broadcast_queue.pop();
            _exchange.Broadcast(broad);
        }
    }
}

void bot1(){
    while(bot1_flag){
        if (m.try_lock()){
            long double price;
            if (_exchange.bid_price == -1) price = min((long double)glr(3000,6500), _exchange.ask_price - glr(1,100));
            else price = glr(1,301) + _exchange.bid_price - 151 + glr(1,Get_Time()/6 + 1);
            long long quantity = gr(100);
            std::ofstream fout("Rand_orders.txt", std::ios_base::app);
            fout<<Get_Time()<<","<<1<<","<<price<<","<<quantity<<"\n";
            //fout.close();
            //cout<<1<<" "<<price<<" "<<quantity<<endl;
            pending_orders.push(Limit_Order(true, price, quantity, Get_Time()));

            if (_exchange.ask_price == 1e15) price = max((long double)glr(3000,6500 + Get_Time()/2), _exchange.bid_price + glr(1,100));
            else price = _exchange.ask_price + 151 - glr(1,301) + glr(1,Get_Time()/6 + 1);
            quantity = gr(100);
            //std::ofstream fout("Bot1_orders.txt", std::ios_base::app);
            fout<<Get_Time()<<","<<0<<","<<price<<","<<quantity<<"\n";
            fout.close();
            //cout<<0<<" "<<price<<" "<<quantity<<endl;
            pending_orders.push(Limit_Order(false, price, quantity, Get_Time()));
            m.unlock();
        }
        this_thread::sleep_for(chrono::microseconds(3000));

        // if (m.try_lock()){
        //     long double price;
        //     if (_exchange.ask_price == 1e15) price = max((long double)glr(120,170), _exchange.bid_price + 2);
        //     else price = glr(1,21) + _exchange.ask_price - 11;
        //     long long quantity = gr(1000);
        //     std::ofstream fout("Bot1_orders.txt", std::ios_base::app);
        //     fout<<Get_Time()<<","<<0<<","<<price<<","<<quantity<<"\n";
        //     fout.close();
        //     //cout<<0<<" "<<price<<" "<<quantity<<endl;
        //     pending_orders.push(Limit_Order(false, price, quantity, Get_Time()));
        //     m.unlock();
        // }

        //this_thread::sleep_for(chrono::milliseconds(1));
        if (Get_Time() > ENDTIME) bot1_flag = false;
    }
}

default_random_engine rd(time(0));
std::uniform_int_distribution<int> quantityDist(1,10);
std::uniform_real_distribution<float> pickDist(0,1);

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

void gaussianBot(){// n is the size of the window 
    double prevBids = -1, prevAsks = -1;
    int n = 5;
    deque<double> mktBids(n);
    deque<double> mktAsks(n);
    iota(mktAsks.begin(), mktAsks.end(), 100);
    iota(mktBids.begin(), mktBids.end(), 100);
    double meanBids = calcMean(mktBids), sdBids = calcSd(mktBids);
    double meanAsks = calcMean(mktAsks), sdAsks = calcSd(mktAsks);
    while(Get_Time() < ENDTIME){
        if(_exchange.ask_price != prevAsks){
            auto z = (_exchange.ask_price - calcMean(mktAsks))/calcSd(mktAsks);
            if(z < 0){z = -z;}
            auto acceptProbability = normalCDF(z) + 0.5;
            auto p = pickDist(rd);
            if(p <= acceptProbability){// accept the ask
                auto price = _exchange.ask_price;
                if (price == 1e15) { // I do not know why this is necessary
                    price /= 1.000000000001;
                }
                if (m.try_lock()) {
                    std::ofstream fout("Gauss_orders.txt", std::ios_base::app);
                    fout<<Get_Time()<<","<<1<<","<<price<<","<<quantityDist(rd)<<"\n";
                    pending_orders.push(Limit_Order(true, price, quantityDist(rd), Get_Time()));
                    fout.close();
                    m.unlock();
                }
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
                auto price = _exchange.ask_price;
                if (price == 1e15) { // I do not know why this is necessary
                    price /= 8e12;
                }
                if (m.try_lock()) {
                    std::ofstream fout("Gauss_orders.txt", std::ios_base::app);
                    pending_orders.push(Limit_Order(false, price, quantityDist(rd), Get_Time()));
                    fout<<Get_Time()<<","<<0<<","<<price<<","<<quantityDist(rd)<<"\n";
                    fout.close();
                    m.unlock();
                }
            } 
            prevBids = _exchange.bid_price;
            mktBids.push_back(_exchange.bid_price);
            mktBids.pop_front();            
        }
    }
}

void smaBot(){// n is the size of the window 
    double prevBids = -1, prevAsks = -1;
    deque<double> mktAsks20(20), mktAsks5(5);
    iota(mktAsks20.begin(), mktAsks20.end(), 100);
    iota(mktAsks5.begin(), mktAsks5.end(), 100);

    while(Get_Time() < ENDTIME){
        if(_exchange.ask_price != prevAsks){
            mktAsks20.push_back(_exchange.ask_price);
            mktAsks20.pop_front();
            mktAsks5.push_back(_exchange.ask_price);
            mktAsks5.pop_front();
            prevAsks = _exchange.ask_price;
            double meanAsks20 = calcMean(mktAsks20), meanAsks5 = calcMean(mktAsks5);

            if(meanAsks5 > meanAsks20){// accept the ask
                auto price = _exchange.ask_price;
                if (price == 1e15) { // I do not know why this is necessary
                    price /= 1.000000000001;
                }
                if (m.try_lock()) {
                    std::ofstream fout("SMA_orders.txt", std::ios_base::app);
                    pending_orders.push(Limit_Order(true, price, quantityDist(rd), Get_Time()));
                    fout<<Get_Time()<<","<<1<<","<<price<<","<<quantityDist(rd)<<"\n";
                    fout.close();
                    m.unlock();
                }
            }

            if(meanAsks5 < meanAsks20){// accept the bid
                auto price = _exchange.ask_price;
                if (price == 1e15) { // I do not know why this is necessary
                    price /= 8e12;
                }
                if (m.try_lock()) {
                    std::ofstream fout("SMA_orders.txt", std::ios_base::app);
                    pending_orders.push(Limit_Order(false, price, quantityDist(rd), Get_Time()));
                    fout<<Get_Time()<<","<<0<<","<<price<<","<<quantityDist(rd)<<"\n";
                    fout.close();
                    m.unlock();
                }
            } 
        }
    }
}

int main() {
    Start_Exchange();

    std::thread t1(Book_Updater);
    t1.detach();

    std::thread t2(Broadcaster);
    t2.detach();

    std::thread t3(bot1);
    t3.detach();

    std::thread t4(gaussianBot);
    t4.detach();

    std::thread t5(smaBot);
    t5.detach();
    
    queue<Limit_Order> Manual_orders;

    while (true) {
        short int order_type;
        std::cin >> order_type;
        bool buy;
        long double price;
        long long quantity;
        if (order_type == 0) {
            std::cin >> buy >> price >> quantity;
            // _exchange.Add_Limit_Order(buy, price, quantity, Get_Time());
            if (m.try_lock()) {
                std::ofstream fout("Manual_orders.txt", std::ios_base::app);
                while (!Manual_orders.empty()) {
                    Limit_Order temp = Manual_orders.front();
                    pending_orders.push(Manual_orders.front());
                    Manual_orders.pop();
                    fout<<temp.time<<","<<temp.buy<<","<<temp.price<<","<<temp.quantity<<"\n";
                }
                pending_orders.push(Limit_Order(buy, price, quantity, Get_Time()));
                fout<<Get_Time()<<","<<buy<<","<<price<<","<<quantity<<"\n";
                fout.close();
                m.unlock();
            }
            else Manual_orders.push(Limit_Order(buy, price, quantity, Get_Time()));
            //pending_orders.push(Limit_Order(buy, price, quantity, Get_Time()));
        } else if (order_type == 1) {
            std::cin >> buy >> quantity;
            //_exchange.Fill_Market_Order(buy, quantity);
            if (m.try_lock()) {
                std::ofstream fout("Manual_orders.txt", std::ios_base::app);
                while (!Manual_orders.empty()) {
                    Limit_Order temp = Manual_orders.front();
                    pending_orders.push(Manual_orders.front());
                    Manual_orders.pop();
                    fout<<temp.time<<","<<temp.buy<<","<<temp.price<<","<<temp.quantity<<"\n";
                }
                pending_orders.push(Limit_Order(buy, -2, quantity, Get_Time()));
                fout<<Get_Time()<<","<<buy<<","<<-2<<","<<quantity<<"\n";
                fout.close();
                m.unlock();
            }
            else Manual_orders.push(Limit_Order(buy, -2, quantity, Get_Time()));
            //pending_orders.push(Limit_Order(buy, -1, quantity, Get_Time()));
        }
        else break;
    }
    //wait for book updater to finish
    while(bot1_flag){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    while(!pending_orders.empty()){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    while (!broadcast_queue.empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}