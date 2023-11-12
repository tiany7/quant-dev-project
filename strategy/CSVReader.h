#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include <chrono>
#include <optional>
#include <tuple>
#include <shared_mutex>


namespace fs = std::filesystem;


template <typename... Ts>
class CSVRow {
public:
    CSVRow(Ts... data) : data_{data...} {}

    // constructor using tuple
    CSVRow(const std::tuple<Ts...>& data) : data_(data) {}
    
    constexpr std::size_t size() const {
        return sizeof...(Ts);
    }

    template <std::size_t Index>
    constexpr auto get() const {
        static_assert(Index < sizeof...(Ts), "Index out of range");
        return std::get<Index>(data_);
    }

private:
    std::tuple<Ts...> data_;

};


template <typename... Ts>
class CSVReader {
public:
    CSVReader(const std::filesystem::path& file_path) : file_path_(file_path) {}

    // 读取整个 CSV 文件
    std::optional<std::vector<CSVRow<Ts...>>> read() const {
        std::ifstream file(file_path_);
        if (!file.is_open()) {
            std::cerr << "unable to open the file: " << file_path_ << std::endl;
            return std::nullopt;
        }

        std::vector<CSVRow<Ts...>> rows;    

        std::string line;
        while (std::getline(file, line)) {
            rows.emplace_back(parseLine(line, std::index_sequence_for<Ts...>{}));
        }

        return rows;
    }

private:
    template <std::size_t... Is>
    static auto parseLine(const std::string& line, std::index_sequence<Is...>) {
        std::istringstream ss(line);
        return std::make_tuple(parseField<Ts>(ss)...);
    }

    template <typename T>
    static T parseField(std::istringstream& ss) {
        T field;
        ss >> field;
        if (ss.peek() == ',') {
            ss.ignore();
        }
        return field;
    }

    std::filesystem::path file_path_;

};

