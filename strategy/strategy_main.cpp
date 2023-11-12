#include "example_strategy.h"
#include <thread>
#include <chrono>

int main(){
    // declare new pipe in and pipeout
    // auto pipe_in_ptr = std::make_shared<MPSCChannel<MarketData>>(new MPSCChannel<MarketData> ());
    // auto pipe_out_ptr = std::make_shared<MPSCChannel<MarketData>>(new MPSCChannel<MarketData> ());
    // ExampleStrategy strategy(pipe_in_ptr, pipe_out_ptr);
    
    // std::thread t1([&strategy](){
    //     strategy.apply_strategy();
    // });

    MPSCChannel<MarketData> pipe_in, pipe_out;
    std::thread t2([&pipe_out](){
        for(int i = 0; i < 12; ++i){
            MarketData data;
            data.price = 100;
            data.volume = 100;
            data.high = 100;
            pipe_out.send(data);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    std::thread t3([&pipe_out](){
        for(int i = 0; i < 12; ++i){
            auto data = pipe_out.receive();
            std::visit([](auto&& arg) {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, MarketData>) {
                    std::cout << arg.price << std::endl;
                } else {
                    std::cerr << "Error: " << arg << std::endl;
                }
            }, data);
        }
    });

    t2.join();
    t3.join();

}
