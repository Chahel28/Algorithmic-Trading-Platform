add_executable(Exchange Exchange.cpp)

add_executable(Demo Demo.cpp MACD,\ ATR,\ BB.cpp)

add_executable(Test Backtest.cpp MACD,\ ATR,\ BB.cpp SMA\ Crossovers.cpp)
target_link_libraries(Test DataFrame)
