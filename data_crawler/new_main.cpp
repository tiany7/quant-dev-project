#include "data_crawler.h"

int main() {
    std::string apiKey = "3d9494c596msh83d6b3b0f28b7dcp197ec4jsn5ea94db681ff";
    std::string apiHost = "yahoo-finance15.p.rapidapi.com";
    DataCrawler crawler(apiKey, apiHost);

    std::string ticker = "AAPL"; 
    std::string interval = "5m"; 
    std::string outputPath = "output2.csv"; 

    crawler.fetchData(ticker, interval, outputPath);

    return 0;
}