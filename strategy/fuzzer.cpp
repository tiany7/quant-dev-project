#include "fuzzer.h"


Fuzzer& Fuzzer::output_to(const std::string &fp){
    file_path = fp;
    return *this;
}
void Fuzzer::prepare(int num_rows, int year_from, int year_to){
    std::ofstream oss(file_path);
    
    // generate start to end date
    int previousYear = year_from;
    int previousMonth = 1;
    int previousDay = 1;
    oss<<std::fixed<<std::setprecision(0);
    for (int _ = 1; _ <= num_rows; ++_){
        std::string ticker = generateRandomString(5);
        std::string start_date = generateRandomDate(previousYear, previousMonth, previousDay);
        std::string end_date = generateRandomDate(previousYear, previousMonth, previousDay);
        double price = generateRandomDouble(0, 100);
        double high = generateRandomDouble(0, 100);
        double low = generateRandomDouble(0, 100);
        double open = generateRandomDouble(0, 100);
        double close = generateRandomDouble(0, 100);
        double volume = generateRandomDouble(0, 100);
        oss << ticker << "," << start_date << "," << end_date << "," << price << "," << high << "," << low << "," << open << "," << close << "," << volume << ','<<'\n';
        previousMonth = std::stoi(end_date.substr(5, 2));
        previousDay = std::stoi(end_date.substr(8, 2));
        previousYear = std::stoi(end_date.substr(0, 4));
    }

}