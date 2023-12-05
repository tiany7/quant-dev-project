#pragma once
#include "base_strategy.h"

class ExampleStrategy : public BaseStrategy {

public:
    ExampleStrategy() : BaseStrategy() {}   

    virtual void apply_strategy(std::vector<receiver_t> &&pipe_in, std::vector<sender_t> &&pipe_out) override;

    virtual ~ExampleStrategy() = default;

    void operator()(std::vector<receiver_t> &&_pipe_in, std::vector<sender_t> &&_pipe_out);
};