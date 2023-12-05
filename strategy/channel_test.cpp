#include "utils.h"
#include "transformer.hpp"
#include <thread>
#include <iostream>
#include <any>
int main(){
    auto [tx, rx] = utils::mpsc::create_channel<std::any>(10);
    auto empty_tx_vec = std::vector<std::shared_ptr<utils::mpsc::Sender<std::any>>>{};
    auto rx_vec = std::vector<std::unique_ptr<utils::mpsc::Receiver<std::any>>>();
    auto [augment_tx, augment_rx] = utils::mpsc::create_channel<std::any>(10);
    rx_vec.push_back(std::move(rx));
    auto validate_tx_vec = std::vector<std::shared_ptr<utils::mpsc::Sender<std::any>>>{};
    validate_tx_vec.emplace_back(std::move(augment_tx));
    auto validate_rx_vec = std::vector<std::unique_ptr<utils::mpsc::Receiver<std::any>>>();
    validate_rx_vec.push_back(std::move(augment_rx));
    auto [main_tx, main_rx] = utils::mpsc::create_channel<std::any>(10);
    auto tx_vec = std::vector<std::shared_ptr<utils::mpsc::Sender<std::any>>>{};
    tx_vec.emplace_back(std::move(main_tx));
    using sender_t = std::shared_ptr<utils::mpsc::Sender<std::any>>;
    using receiver_t = std::unique_ptr<utils::mpsc::Receiver<std::any>>;
    ValidateTransformer validate_transformer;
    AugmentTransformer augment_transformer;
    std::thread t1([](sender_t &&tx){
        for(int i = 0; i < 10; ++i){
            MarketData data;
            data.price = i;
            data.volume = i;
            tx->send(data);
        }
        
    }, std::move(tx));
    // std::cerr<<"t1 finished "<<tx.use_count()<<std::endl;
    t1.join();
    TransformerManager transformer_mgr;
    transformer_mgr
    .apply(validate_transformer, std::move(rx_vec), std::move(validate_tx_vec))
    .apply(augment_transformer, std::move(validate_rx_vec), std::move(tx_vec));
    for (auto data = main_rx->receive(); !is_err(data); data = main_rx->receive()){
        auto &&item = std::get<std::any>(data);
        auto &&real_item = std::any_cast<MarketData>(item);
        std::cerr<<"price1: "<<real_item.price<<std::endl;
        // check main_tx's ref_count
    }
    return 0;
}