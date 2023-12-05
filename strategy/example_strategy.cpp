#include "example_strategy.h"
#include <algorithm>



void ExampleStrategy::apply_strategy(std::vector<receiver_t> &&pipe_in, std::vector<sender_t> &&pipe_out) {
    // do nothing here
    std::cerr<<"example strategy called!!"<<std::endl;
    for(auto data = pipe_in[0]->receive(); !is_err(data); data = pipe_in[0]->receive()){

        auto &&real_item = std::get<std::any>(data);
        auto &&item = std::any_cast<MarketData>(real_item);
        Signal sig;
        // implement a trend following strategy
        if(item.close > item.open){
            sig.type = Signal::SignalType::BUY;
        }else{
            sig.type = Signal::SignalType::SELL;
        }

        sig.price = item.close - item.open * 0.01 * (rand() % 10) * (item.high - item.low) / 100.0 + item.open;
        sig.volume = item.volume * 0.01 * (rand() % 10) * 100;
        
        pipe_out[0]->send(sig);
    }
}

void ExampleStrategy::operator()(std::vector<receiver_t> &&_pipe_in, std::vector<sender_t> &&_pipe_out){
    apply_strategy(std::move(_pipe_in), std::move(_pipe_out));
}