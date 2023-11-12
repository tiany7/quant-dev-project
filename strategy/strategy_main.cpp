#include "example_strategy.h"
#include <thread>
#include <chrono>

int main(){
    // declare new pipe in and pipeout
    auto pipe_in_ptr = std::make_shared<MPSCChannel<MarketData>>();
    auto pipe_out_ptr = std::make_shared<MPSCChannel<MarketData>>();
    ExampleStrategy strategy(pipe_in_ptr, pipe_out_ptr);
    std::thread t1([&strategy](){
        strategy.apply_strategy();
    });
    std::thread t2([&pipe_in_ptr](){
        for(int i = 0; i < 12; ++i){
            MarketData data;
            data.price = 100;
            data.volume = 100;
            data.high = 100;
            pipe_in_ptr->send(data);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        pipe_in_ptr->shutdown(); // graceful shutdown
    });

    std::thread t3([&pipe_out_ptr](){
        for(int i = 0; i < 12; ++i){
            auto data = pipe_out_ptr->receive();
            std::visit([](auto&& arg) {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, MarketData>) {
                    std::cout << arg.price << std::endl;
                } else {
                    std::cerr << "Error: " << arg << std::endl;
                }
            }, data);
        }
    });

    t1.join();
    t2.join();
    t3.join();

}
