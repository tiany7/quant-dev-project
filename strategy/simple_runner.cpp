#include <thread>
#include <iostream>
#include <any>

#include <unistd.h>

#include "utils.h"
#include "transformer.hpp"
#include "example_strategy.h"
#include "data_manager.h"
#include "CSVReader.h"
#include "const_def.h"

int main(int argc, char** argv){
    std::string current_path;
    int opt;
    int balance;
    int initial_balance;
    int current_holdings = 0;
    int total_loss = 0;  
    int total_earnings = 0;

    while ((opt = getopt(argc, argv, "i:b:")) != -1) {
        switch (opt) {
        case 'i':
            current_path = optarg;
            break;
        case 'b':
            balance = atoi(optarg);
            initial_balance = balance;
            break;
        default: 
            fprintf(stderr, "Usage: %s [-i input_file] [-b init_balance]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }


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

    // we get the input stream from the file
    std::thread t1([current_path](sender_t &&tx){
        DataManager data_mgr;
        // std::string current_path = std::string(global_path) + output_dir;
        auto &&rx = data_mgr.read_csv(current_path);
        for (auto data = rx->receive(); !is_err(data); data = rx->receive()){
            // auto &&item = std::get<std::any>(data);
            
            
            auto &&real_item = std::get<MarketData>(data);
            // std::cerr<<"price: "<<real_item.price<<std::endl;
            tx->send(real_item);
        }
        // std::cerr<<"t1 finished "<<tx.use_count()<<std::endl;
        
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
        int transaction_cost = sig.price * sig.volume;

        if(sig.type == Signal::SignalType::BUY){
            balance -= transaction_cost;
            current_holdings += sig.volume;
        }
        else{
            balance += transaction_cost;
            current_holdings -= sig.volume;
            total_earnings += transaction_cost - (sig.price * sig.volume);
        }

        std::cerr << (sig.type == Signal::SignalType::BUY ? "BUY" : "SELL") << std::endl;
        std::cerr << "price: " << sig.price << std::endl;
        std::cerr << "volume: " << sig.volume << std::endl;
        std::cerr << "current holdings: " << current_holdings << std::endl;
        std::cerr << "current balance: " << balance << std::endl;
    }
    double loss_percentage = (static_cast<double>(total_earnings) / initial_balance) * 100.0;

    std::cerr << "finish running, the initial balance was: " << initial_balance << std::endl;
    std::cerr << "final balance is: " << balance << std::endl;
    std::cerr << "total loss: " << loss_percentage << "%"<< std::endl;
    return 0;
}