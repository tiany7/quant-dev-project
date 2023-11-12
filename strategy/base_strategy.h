#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>


namespace fs = std::filesystem;


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
};


class BaseStrategy {
    
}