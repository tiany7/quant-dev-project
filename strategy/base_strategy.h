#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <variant>

#include "const_def.h"
#include "utils.h"




struct MarketData {
    std::string ticker;
    std::string start_date;
    std::string end_date;
    double price;
    double high;
    double low;
    double open;
    double close;
    double volume;
};


class BaseStrategy {
protected:
    std::shared_ptr<MPSCChannel<MarketData>> pipe_in, pipe_out; // these are the pipes for the strategy to communicate with the server
public:
    BaseStrategy(std::shared_ptr<MPSCChannel<MarketData>> _pipe_in, std::shared_ptr<MPSCChannel<MarketData>> _pipe_out) : pipe_in(_pipe_in), pipe_out(_pipe_out) {}
    virtual ~BaseStrategy() = default;

    virtual void apply_strategy() = 0; // declare it as pure virtual function
};