#include "example_strategy.h"
#include <algorithm>



void ExampleStrategy::apply_strategy() {
    // do nothing here
    for(auto data = pipe_in->receive(); !is_err(data); data = pipe_in->receive()){
        auto &&item = std::get<MarketData>(data);
        Signal sig;
        std::cerr<<"signal: "<<item.price<<" "<<item.volume<<std::endl;
        sig.price = item.price;
        sig.volume = item.volume;
        sig.type = Signal::SignalType::BUY;
        
        pipe_out->send(sig);
    }
    pipe_out->shutdown();
    // std::cerr<<"apply strategy done"<<std::endl;
}