#include "data_manager.h"

DataManager::DataManager() : reader() {}



// this creates a file reader stream that could asynchronously read the file
DataManager::receiver_type DataManager::read_csv(const std::string& file_path) {
        auto rx = std::make_shared<MPSCChannel<MarketData>>();
        auto tx = rx; // sender is receiver
        // std::cerr<<"reading file: "<<file_path<<std::endl;
        join_handles.emplace_back(std::async(std::launch::async, [this, tx, file_path](){
            auto rows = this->reader.read_from(file_path);
            if (!rows) {
                std::cerr << "unable to read the file: " << file_path << std::endl;
                return;
            }
            for (const auto& row : *rows) {
                    MarketData data;
                    data.ticker = row.get<kTicker>();
                    data.start_date = row.get<kStartDate>();
                    data.end_date = row.get<kEndDate>();
                    data.price = row.get<kPrice>();
                    data.high = row.get<kHigh>();
                    data.low = row.get<kLow>();

                    data.open = row.get<kOpen>();
                    data.close = row.get<kClose>();
                    data.volume = row.get<kVolume>();
                    // std::cerr<<data<<std::endl;
                    tx->send(data);
            }
            tx->shutdown();
        }));
        // auto rows = this->reader.read_from(file_path);
        // if (!rows) {
        //     std::cerr << "unable to read the file: " << file_path << std::endl;
        //     return nullptr;
        // }
        // for (const auto& row : *rows) {
        //         MarketData data;
        //         data.ticker = row.get<kTicker>();
        //         data.start_date = row.get<kStartDate>();
        //         data.end_date = row.get<kEndDate>();
        //         data.price = row.get<kPrice>();
        //         data.high = row.get<kHigh>();
        //         data.low = row.get<kLow>();
                
        //         data.open = row.get<kOpen>();
        //         data.close = row.get<kClose>();
        //         data.volume = row.get<kVolume>();
        //         // std::cerr<<data<<std::endl;
        //         tx->send(data);
        // }
        return rx;
}


void DataManager::join() {
        for (auto& handle : join_handles) {
            handle.wait();
        }
}


DataManager::~DataManager() {
        join();
}