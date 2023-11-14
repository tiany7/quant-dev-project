#include "backtester.h"

Backtester::Backtester(const std::string& csvFilePath, std::shared_ptr<ExampleStrategy> strategy, double initialCapital)
    : csvFilePath(csvFilePath), strategy(strategy), capital(initialCapital) {
    // TODO: finish connect the channel
    pipe_out = strategy->getOutputChannel();
}

void Backtester::run() {
    CSVReader<int, double, std::string> reader(csvFilePath);
    auto rows = reader.read();

    // TODO: make this work.
    strategy->apply_strategy(rows);

    receiveSignals();
}

void Backtester::receiveSignals() {
   while (true) {
        auto signal = pipe_out->receive();
        if (!signal) {
            break;  // Break the loop if the channel is closed
        }

        // Process the signal
        processSignal(*signal);
    }
}


void Backtester::processSignal(const Signal& signal) {
    if (signal.type ==  Signal::SignalType::BUY) {
        // Update capital for a buy signal
        capital -= signal.price * signal.volume;
    } else if (signal.type == Signal::SignalType::SELL) {
        // Update capital for a sell signal
        capital += signal.price * signal.volume;
    }
}