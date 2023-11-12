#include "example_strategy.h"
#include <algorithm>



void ExampleStrategy::apply_strategy() {
    // do nothing here
    for(auto data = pipe_in->receive(); !is_err(data); pipe_in->receive()){
        std::cout<<std::get<MarketData>(data).price<<std::endl;
        pipe_out->send(std::get<MarketData>(data));
    }
}