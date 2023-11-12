#include "base_strategy.h"

class ExampleStrategy : public BaseStrategy {

public:
    ExampleStrategy(const std::shared_ptr<MPSCChannel<MarketData>> &_pipe_in, const std::shared_ptr<MPSCChannel<MarketData>> &_pipe_out) : BaseStrategy(_pipe_in, _pipe_out) {}   

    virtual void apply_strategy() override;

    virtual ~ExampleStrategy() = default;
};