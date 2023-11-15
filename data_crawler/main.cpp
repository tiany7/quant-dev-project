#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream> 
#include <string>


// This callback function gets called by libcurl as soon as there is data received that needs to be saved.
size_t WriteDataCallback(void* buffer, size_t size, size_t nmemb, void* userp) {
    // Append the data to the userp buffer
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* hnd = curl_easy_init();
    CURLcode ret = CURLE_OK;
    std::string readBuffer;
    std::vector<std::string> symbolList;
    int flag = 1;

    if (hnd) {
        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(hnd, CURLOPT_URL, "https://apidojo-yahoo-finance-v1.p.rapidapi.com/market/get-trending-tickers?region=US");
        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteDataCallback);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "X-RapidAPI-Key: 3d9494c596msh83d6b3b0f28b7dcp197ec4jsn5ea94db681ff");
        headers = curl_slist_append(headers, "X-RapidAPI-Host: apidojo-yahoo-finance-v1.p.rapidapi.com");
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

        ret = curl_easy_perform(hnd);

        if (ret != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(ret) << std::endl;
        } else {
            // The response is now in the readBuffer - you can process it as needed
            std::cout << "Received data: " << readBuffer << std::endl;
        }

        // std::string readBuffer = R"(readBuffer)";

        // 使用nlohmann/json解析JSON字符串
        auto json = nlohmann::json::parse(readBuffer);

        // 提取股票代码
        std::vector<std::string> equitySymbols;
        for (const auto& quote : json["finance"]["result"][0]["quotes"]) {
            if (quote["quoteType"] == "EQUITY") {
                equitySymbols.push_back(quote["symbol"]);
            }
        }
        std::cout << std::endl;
        

        // 打印股票代码
        for (const auto& symbol : equitySymbols) {
            std::cout << "SYMBOL: " << symbol << std::endl;
            symbolList.push_back(symbol);

        }

        for(auto& i : symbolList){
            std::cout << i << ", " << std::endl;
        }

        // Always cleanup
        curl_easy_cleanup(hnd);
        curl_slist_free_all(headers);
        readBuffer.clear();
    }

    // Open a CSV file.
    std::ofstream csvFile("output.csv");


    for(int k = 0; k < symbolList.size(); ++k) {
        // 初始化新的请求
        hnd = curl_easy_init();

        std::string currentTicker = symbolList[k];
        std::cout << "currentTicker："<< currentTicker  << std::endl;


        if (hnd) {
            std::string targetApiUrl = "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/history?symbol=" + currentTicker + "&interval=5m&diffandsplits=false";
            curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
            curl_easy_setopt(hnd, CURLOPT_URL, targetApiUrl.c_str());
            // std::cout << " targetApiUrl : " << targetApiUrl << std::endl;
            // "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/history?symbol=AAPL&interval=5m&diffandsplits=false"
            curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteDataCallback);
            curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "X-RapidAPI-Key: 3d9494c596msh83d6b3b0f28b7dcp197ec4jsn5ea94db681ff");
            headers = curl_slist_append(headers, "X-RapidAPI-Host: yahoo-finance15.p.rapidapi.com");
            curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

            ret = curl_easy_perform(hnd);

            if (ret != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(ret) << std::endl;
            } else {
                // The response is now in the readBuffer - you can process it as needed
                // std::cout << "Received data: " << readBuffer << std::endl;
            }

            auto data = nlohmann::json::parse(readBuffer);

            

            // Write CSV headers.
            if (flag == 1){
                csvFile << "Symbol,Date,Open,High,Low,Close,Volume\n";
                flag = 0;
            }

            std::ofstream out("data.txt");
            auto item = data["body"];

            // Vector to store values
            std::vector<double> values;

            for (auto it = item.begin(); it != item.end(); ++it) {
                std::string date = it.value().at("date");
                double open = it.value().at("open");
                double high = it.value().at("high");
                double low = it.value().at("low");
                double close = it.value().at("close");
                double volume = it.value().at("volume");
                csvFile << currentTicker << "," << date << "," << open << "," << high << "," << low << "," << close << "," << volume << "\n";
            }

            // Always cleanup
            curl_easy_cleanup(hnd);
            curl_slist_free_all(headers);
            readBuffer.clear();


            
        }

         
    }
    return 0;
}
    


