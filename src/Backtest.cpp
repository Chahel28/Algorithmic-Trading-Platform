#include <DataFrame/DataFrame.h>
#include <DataFrame/Utils/DateTime.h>
#include <iostream>

#include "Backtest.h"
#include "MACD, ATR, BB.h"
#include "RSI_ADX.h"
#include "SMA Crossovers.h"

using namespace hmdf;

using StrDataFrame = StdDataFrame<std::string>;
using DTDataFrame = StdDataFrame<DateTime>;

inline double seconds_to_years(const double seconds) {
    return seconds / (365.24 * 24 * 60 * 60);
}

double mean(const std::vector<double> &values) {
    double sum = 0;
    for (auto &val : values) {
        sum += val;
    }
    return sum / values.size();
}

double stdev(const std::vector<double> &values, const double mean) {
    double sum = 0;
    for (auto &val : values) {
        sum += std::pow(val - mean, 2);
    }
    return std::sqrt(sum / values.size());
}

double stdev(const std::vector<double> &values) {
    return stdev(values, mean(values));
}

// This is a simple ROI calculator, returns the %ROI of a strategy
// signal > 0 -> long, signal < 0 -> short, signal == 0 -> flat
template <typename T>
std::vector<double> returns(const std::vector<double> &prices, const std::vector<T> &signal) {
    std::vector<double> roi(prices.size());

    bool position = false;
    for (int i = 1; i < prices.size() - 1; ++i) { // ignore buy signal on final tick
        if (signal[i - 1] > 0 && !position) {
            position = true;
            // std::cout << "Buy: " << prices[i] << '\t';
        } else if (signal[i - 1] < 0 && position) {
            position = false;
            // std::cout << "Sell: " << prices[i] << '\n';
        }

        // buy now if previous signal was a buy
        roi[i] = position ? (100 + roi[i - 1]) * prices[i + 1] / prices[i] - 100
                          : roi[i - 1];
    }
    roi[roi.size() - 1] = roi[roi.size() - 2];

    return roi;
}

// This is a simple CAGR calculator, returns the %CAGR of a strategy
// signal > 0 -> long, signal < 0 -> short, signal == 0 -> flat
double cagr(const std::vector<double> &returns, double years) {
    return 100 * std::pow(mean(returns) / 100 + 1, 1 / years) - 100;
}

std::vector<double> risk_free_returns(const std::vector<double> &returns, const double risk_free_rate) {
    std::vector<double> risk_free_returns(returns);
    for (auto &i : risk_free_returns) {
        i -= risk_free_rate;
    }
    return risk_free_returns;
}

double sharpe_ratio(const std::vector<double> &returns, const double risk_free_rate) {
    auto risk_free = risk_free_returns(returns, risk_free_rate);
    return mean(risk_free) / stdev(risk_free);
}

double max_drawdown(const std::vector<double> &returns) {
    double peak = returns[0];
    double max_drawdown = 0.0;
    for(int i = 1; i < returns.size(); i++) {
        double drawdown = (peak - returns[i]) / peak;
        max_drawdown = std::max(max_drawdown, drawdown);
        peak = std::max(peak, returns[i]);
    }
    return max_drawdown;
}

double hit_ratio(const std::vector<double>& returns) {
    int total_trades = returns.size();
    int profitable_trades = 0;
    for(const auto &return_val : returns) {
        if (return_val > 0)
            profitable_trades++;
    }
    return (double)profitable_trades / total_trades;
}

int main() {
    for (std::string ticker : {"IBM", "AAPL"}) {
        DTDataFrame df;
        df.read(("DataFrame/data/DT_" + ticker + ".csv").c_str(), io_format::csv2);

        auto close = df.get_column<double>((ticker + "_Close").c_str());
        auto index = df.get_index();
        auto years = seconds_to_years(index.back() - index.front());

        std::cout << ticker << " metrics\n";

        auto rets = returns(close, BollingerBands(close, 20, 1));
        double cagr_val = cagr(rets, years), sharpe = sharpe_ratio(rets), max_drawdown_val = max_drawdown(rets), hit_ratio_val = hit_ratio(rets);
        std::cout << "  Bollinger Bands (1 sigma) : CAGR = " << cagr_val << "%, Sharpe Ratio = " << sharpe << " Max Drawdown = " << max_drawdown_val << " Hit Ratio = " << hit_ratio_val << "\n";

        rets = returns(close, MACD(close, 12, 26, 9));
        cagr_val = cagr(rets, years), sharpe = sharpe_ratio(rets), max_drawdown_val = max_drawdown(rets), hit_ratio_val = hit_ratio(rets);
        std::cout << "  MACD Crossover            : CAGR = " << cagr_val << "%, Sharpe Ratio = " << sharpe << " Max Drawdown = " << max_drawdown_val << " Hit Ratio = " << hit_ratio_val << "\n";

        rets = returns(close, maCrossover(close, 50, 200));
        cagr_val = cagr(rets, years), sharpe = sharpe_ratio(rets), max_drawdown_val = max_drawdown(rets), hit_ratio_val = hit_ratio(rets);
        std::cout << "  SMA-20 SMA-50 Crossover   : CAGR = " << cagr_val << "%, Sharpe Ratio = " << sharpe << " Max Drawdown = " << max_drawdown_val << " Hit Ratio = " << hit_ratio_val << "\n\n";
    }

    return 0;
}
