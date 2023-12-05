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
#include "data_manager.h"

struct Signal
{
    double price;
    double volume;
    enum class SignalType
    {
        BUY,
        SELL
    } type;
};

class BaseStrategy
{
protected:
public:
    BaseStrategy() = default;
    virtual ~BaseStrategy() = default;

    virtual void apply_strategy(std::vector<receiver_t> &&pipe_in, std::vector<sender_t> &&pipe_out) = 0; // declare it as pure virtual function

    void operator()(std::vector<receiver_t> &&_pipe_in, std::vector<sender_t> &&_pipe_out);
};