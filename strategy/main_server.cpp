#include <thread>
#include <iostream>
#include <any>

#include "utils.h"
#include "transformer.hpp"
#include "example_strategy.h"
#include "data_manager.h"
#include "CSVReader.h"
#include "const_def.h"
int main(){
    auto [tx, rx] = utils::mpsc::create_channel<std::any>(10);
    auto empty_tx_vec = std::vector<sender_t>{};
    auto rx_vec = std::vector<receiver_t>();
    auto [augment_tx, augment_rx] = utils::mpsc::create_channel<std::any>(10);
    rx_vec.push_back(std::move(rx));
    auto validate_tx_vec = std::vector<sender_t>{};
    validate_tx_vec.emplace_back(std::move(augment_tx));
    auto validate_rx_vec = std::vector<receiver_t>();
    validate_rx_vec.push_back(std::move(augment_rx));
    auto [main_tx, main_rx] = utils::mpsc::create_channel<std::any>(10);
    auto tx_vec = std::vector<sender_t>{};
    tx_vec.emplace_back(std::move(main_tx));
    using sender_t = sender_t;
    using receiver_t = receiver_t;
    ValidateTransformer validate_transformer;
    AugmentTransformer augment_transformer;

    // we get the input stream from the file
    std::thread t1([](sender_t &&tx){
        DataManager data_mgr;
        std::string current_path = std::string(global_path) + "example.csv";
        auto &&rx = data_mgr.read_csv(current_path);
        for (auto data = rx->receive(); !is_err(data); data = rx->receive()){
            // auto &&item = std::get<std::any>(data);
            
            
            auto &&real_item = std::get<MarketData>(data);
            std::cerr<<"send price: "<<real_item.price<<std::endl;
            tx->send(real_item);
        }
        std::cerr<<"t1 finished "<<tx.use_count()<<std::endl;
        
    }, std::move(tx));
    // std::cerr<<"t1 finished "<<tx.use_count()<<std::endl;
    
    PipelineManager pipeline_mgr;
    
    auto [algo_tx, algo_rx] = utils::mpsc::create_channel<std::any>(10);
    std::vector<sender_t> algo_tx_vec;
    std::vector<receiver_t> main_rx_vec;
    main_rx_vec.emplace_back(std::move(main_rx));
    algo_tx_vec.emplace_back(std::move(algo_tx));
    ExampleStrategy example_strategy;
    pipeline_mgr
    .apply(validate_transformer, std::move(rx_vec), std::move(validate_tx_vec))
    .in_span("validate transformer")
    .apply(augment_transformer, std::move(validate_rx_vec), std::move(tx_vec))
    .in_span("augment transformer")
    .apply(example_strategy, std::move(main_rx_vec), std::move(algo_tx_vec))
    .in_span("example strategy");
    t1.join();
    for (auto data = algo_rx->receive(); !is_err(data); data = algo_rx->receive()){
        auto &&item = std::get<std::any>(data);
        auto &&sig = std::any_cast<Signal>(item);
        std::cerr<<(sig.type == Signal::SignalType::BUY ? "BUY" : "SELL")<<std::endl;
        std::cerr<<"price: "<<sig.price<<std::endl;
        std::cerr<<"volume: "<<sig.volume<<std::endl;
        // check main_tx's ref_count
    }
    
    return 0;
}