#include "utils.h"
#include <thread>
#include <iostream>

int main(){
    auto [sender, receiver] = utils::mpsc::create_channel<int>(10);

    std::thread t1([](std::shared_ptr<utils::mpsc::Sender<int>> sender){
        // std::this_thread::sleep_for(std::chrono::seconds(3));
        for(int i = 0; i < 10; ++i){
            sender->send(i);
        }
    }, sender);

    std::thread t2([](std::unique_ptr<utils::mpsc::Receiver<int>> &&receiver){
        for(int i = 0; i < 10; ++i){
            auto data = receiver->receive();
            std::visit([](auto&& arg) {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, int>) {
                    std::cout << arg << std::endl;
                } else {
                    std::cerr << "Error: " << arg << std::endl;
                }
            }, data);
        }
    }, std::move(receiver));

    t1.join();
    t2.join();

    return 0;
}