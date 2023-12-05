#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <random>

class Fuzzer
{
private:
    std::string file_path;
    std::string generateRandomString(int length)
    {
        static const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);

        std::string result;
        result.reserve(length);

        for (int i = 0; i < length; ++i)
        {
            result += charset[dis(gen)];
        }

        return result;
    }
    std::string generateRandomDate(int previousYear, int previousMonth, int previousDay)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> yearDis(previousYear, previousYear + 1);
        std::uniform_int_distribution<> monthDis(previousMonth, previousMonth + 1);
        std::uniform_int_distribution<> dayDis(previousDay, previousDay + 1);

        int year = yearDis(gen);
        int month = monthDis(gen);
        int day = dayDis(gen);

        return std::to_string(year) + "-" + (month < 10 ? "0" : "") + std::to_string(month) +
               "-" + (day < 10 ? "0" : "") + std::to_string(day);
    }

    // 生成随机浮点数
    double generateRandomDouble(double min, double max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }
public:
    Fuzzer() = default;
    ~Fuzzer() = default;
    Fuzzer& output_to(const std::string &fp);
    void prepare(int num_rows, int year_from, int year_to);
};