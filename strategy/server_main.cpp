#include <iostream>


#include "CSVReader.h"
#include "const_def.h"

int main(){
    std::string file = std::string(global_path) + "example.csv";
    CSVReader<int, double, std::string> reader(file);
    auto rows = reader.read();
    if (rows) {
        for (const auto& row : *rows) {
            std::cout << row.get<0>() << ", " << row.get<1>() << ", " << row.get<2>() << std::endl;
        }
    }
    return 0;
}