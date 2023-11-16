
#include <thread>
#include <chrono>

#include "example_strategy.h"
#include "data_manager.h"
#include "CSVReader.h"
#include "const_def.h"

// int main(){
//     // declare new pipe in and pipeout
//     auto pipe_in_ptr = std::make_shared<MPSCChannel<MarketData>>();
//     auto pipe_out_ptr = std::make_shared<MPSCChannel<Signal>>();
//     ExampleStrategy strategy(pipe_in_ptr, pipe_out_ptr);
//     std::thread t1([&strategy](){
//         strategy.apply_strategy();
//     });
//     std::thread t2([&pipe_in_ptr](){
//         for(int i = 0; i < 12; ++i){
//             MarketData data;
//             data.price = i;
//             data.volume = 100;
//             data.high = 100;
//             pipe_in_ptr->send(data);
//             std::this_thread::sleep_for(std::chrono::milliseconds(100));
//         }
//         pipe_in_ptr->shutdown(); // graceful shutdown
//     });

//     std::thread t3([&pipe_out_ptr](){
//         for(int i = 0; i < 12; ++i){
//             auto data = pipe_out_ptr->receive();
//             std::visit([](auto&& arg) {
//                 if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, Signal>) {
//                     std::cout << arg.price<< " "<<arg.volume << std::endl;
//                 } else {
//                     std::cerr << "Error: " << arg << std::endl;
//                 }
//             }, data);
//         }
//     });

//     t1.join();
//     t2.join();
//     t3.join();

// }

int main() {
    std::string current_path = std::string(global_path) + "example.csv";
    // std::cerr<<"global path: "<<current_path<<std::endl;
    DataManager data_mgr;
    // std::cerr<<"reading file: "<<current_path<<std::endl;
    auto rx = data_mgr.read_csv(current_path);
    // std::cerr<<"reading file done"<<std::endl;
    auto pipe_out = std::make_shared<MPSCChannel<Signal>>();
    ExampleStrategy strategy(rx, pipe_out);
    std::jthread t1([&strategy](){
        strategy.apply_strategy();
    });

    std::jthread t2([&pipe_out](){
        for(int i = 0; i < 3; ++i){
            auto data = pipe_out->receive();
            std::visit([](auto&& arg) {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, Signal>) {
                    std::cout << arg.price<< " "<<arg.volume << std::endl;
                } else {
                    std::cerr << "Error: " << arg << std::endl;
                }
            }, data);
        }
    });
    return 0;
}