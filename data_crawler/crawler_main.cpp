#include "data_crawler.h"
#include <unistd.h>
#include <thread>
#include <iostream>
#include <any>
#include <string>

int main(int argc, char** argv) {
    std::string apiKey = "3d9494c596msh83d6b3b0f28b7dcp197ec4jsn5ea94db681ff";
    std::string apiHost = "yahoo-finance15.p.rapidapi.com";
    DataCrawler crawler(apiKey, apiHost);

    std::string ticker;
    std::string interval;
    std::string outputPath;
    int opt;

    while ((opt = getopt(argc, argv, "t:i:o:")) != -1) {
        switch (opt) {
        case 't':
            ticker = optarg;
            break;
        case 'i':
            interval = optarg;
            break;
        case 'o':
            outputPath = optarg;
            break; 
        default: 
            fprintf(stderr, "Usage: %s [-t ticker] [-i interval] [-o outputfile]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    crawler.fetchData(ticker, interval, outputPath);

    return 0;
}