#pragma once
#include <string>
#include <vector>

class DataCrawler {
public:
    DataCrawler(const std::string& apiKey, const std::string& apiHost);
    void fetchData(const std::string& ticker, const std::string& interval, const std::string& outputPath);

private:
    std::string apiKey;
    std::string apiHost;
    static size_t WriteDataCallback(void* buffer, size_t size, size_t nmemb, void* userp);
    void downloadData(const std::string& url, std::string& readBuffer);
    void parseAndSaveData(const std::string& readBuffer, const std::string& outputPath, const std::string& ticker);
};