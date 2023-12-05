#pragma once

#include <future>

#include "utils.h"
#include "async_utils.hpp"
#include "CSVReader.h"
#include "const_def.h"


// raw data from the market
struct MarketData {
    std::string ticker;
    std::string start_date;
    std::string end_date;
    double price;
    double high;
    double low;
    double open;
    double close;
    double volume;
    friend std::ostream& operator<<(std::ostream& os, const MarketData& data) {
        os<<"{"
          << data.ticker 
          << " | " << data.start_date 
          << " | " << data.end_date 
          << " | " << data.price 
          << " | " << data.high 
          << " | " << data.low 
          << " | " << data.open 
          << " | " << data.close 
          << " | " << data.volume
          <<"}";
        return os;
    }
};

class DataManager {
private:
    CSVReader<CSV_ROW_TYPES> reader;
    std::vector<std::future<void>> join_handles;
    using receiver_type = std::unique_ptr<utils::mpsc::Receiver<MarketData>>;
public:
    
    DataManager();


    receiver_type read_csv(const std::string& file_path);

    

    void join();

    ~DataManager();

};