#include "data_crawler.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

DataCrawler::DataCrawler(const std::string& apiKey, const std::string& apiHost)
    : apiKey(apiKey), apiHost(apiHost) {}

size_t DataCrawler::WriteDataCallback(void* buffer, size_t size, size_t nmemb, void* userp) {
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}

void DataCrawler::downloadData(const std::string& url, std::string& readBuffer) {
    CURL* hnd = curl_easy_init();
    if (hnd) {
        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteDataCallback);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("X-RapidAPI-Key: " + apiKey).c_str());
        headers = curl_slist_append(headers, ("X-RapidAPI-Host: " + apiHost).c_str());
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

        CURLcode ret = curl_easy_perform(hnd);
        if (ret != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(ret) << std::endl;
        }

        curl_easy_cleanup(hnd);
        curl_slist_free_all(headers);
    }
}


void DataCrawler::parseAndSaveData(const std::string& readBuffer, const std::string& outputPath, const std::string& ticker) {
    // Parse JSON data
    nlohmann::json data;
    try {
        data = nlohmann::json::parse(readBuffer);
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return;
    }

    // Open CSV file
    std::ofstream csvFile(outputPath, std::ios_base::app);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to open file: " << outputPath << std::endl;
        return;
    }

    // Check if the 'body' key exists in JSON
    if (data.find("body") == data.end()) {
        std::cerr << "JSON does not contain 'body' key" << std::endl;
        return;
    }

    auto item = data["body"];

    // Write data to CSV
    for (auto it = item.begin(); it != item.end(); ++it) {
        std::string date = it.value().at("date");
        double open = it.value().at("open");
        double high = it.value().at("high");
        double low = it.value().at("low");
        double close = it.value().at("close");
        double volume = it.value().at("volume");
        // to match the data input format for later use, due to api limit, use clse as current price
        csvFile << ticker << "," << date << "," << date  << "," << close << ","<< high << "," << low << "," << open << "," << close << "," << volume << "\n";
    }

    // Close the file
    csvFile.close();

    std::cout << "Data written to " << outputPath << std::endl; // Debug information
}

void DataCrawler::fetchData(const std::string& ticker, const std::string& interval, const std::string& outputPath) {
    std::string readBuffer;
    std::string url = "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/history?symbol=" + ticker + "&interval=" + interval + "&diffandsplits=false";
    downloadData(url, readBuffer);
    parseAndSaveData(readBuffer, outputPath, ticker);
}
