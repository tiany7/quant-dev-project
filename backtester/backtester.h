// backtester.h
#pragma once

#include "strategy/CSVReader.h"
#include "strategy/base_strategy.h"
#include "strategy/example_strategy.h"
#include <memory>
#include <vector>

class Backtester {
public:
    Backtester(const std::string& csvFilePath, std::shared_ptr<ExampleStrategy> strategy, double initialCapital);
    void run();
    void receiveSignals();
    void processSignal(const Signal& signal);

private:
    std::string csvFilePath;
    std::shared_ptr<ExampleStrategy> strategy;
    std::shared_ptr<MPSCChannel<Signal>> pipe_out;
    double capital;
};