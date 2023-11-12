#include "example_strategy.h"
#include <algorithm>



void ExampleStrategy::apply_strategy() {
    // do nothing here
    for(auto data = pipe_in->receive(); !is_err(data); data = pipe_in->receive()){
        auto price = std::get<MarketData>(data).price;
        Signal sig;
        sig.price = price;
        sig.volume = 100;
        sig.type = Signal::SignalType::BUY;
        pipe_out->send(sig);
    }
}