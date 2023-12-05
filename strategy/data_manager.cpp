#include "data_manager.h"

DataManager::DataManager() : reader() {}



// this creates a file reader stream that could asynchronously read the file
DataManager::receiver_type DataManager::read_csv(const std::string& file_path) {
        auto [tx, rx] = utils::mpsc::create_channel<MarketData>(2);
        // std::cerr<<"reading file: "<<file_path<<std::endl;
        join_handles.emplace_back(std::async( [this, file_path](std::shared_ptr<utils::mpsc::Sender<MarketData>>&& tx){
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
                    std::cerr<<data<<std::endl;
                    tx->send(data);
            }
            std::cerr<<"finished reading file: "<<tx.use_count()<<std::endl;
            tx.reset();
        }, std::move(tx)));
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
        return std::move(rx);
}


void DataManager::join() {
        for (auto& handle : join_handles) {
            handle.wait();
        }
}


DataManager::~DataManager() {
        join();
}