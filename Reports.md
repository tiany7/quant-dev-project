# Mid-Project Report

## Overview
Our project is a stock market analysis tool, designed with distinct modules for downloading financial data, executing trading strategies, and performing backtesting. It integrates real-time data from sources like Yahoo Finance and applies user-defined trading strategies to assess performance across multiple stock tickers. The system's modular design, coupled with modern C++ features, ensures flexibility, scalability, and efficiency, catering to both individual investors and financial analysts.

## Github link
https://github.com/tiany7/quant-dev-project

## Environment
The project is built upon C++20 with Bazel.

## Modules
Currently, the project is divided into three modules to make the MVP (Minimum Viable Product) pipeline work.

### Data_crawler
The data_crawler is responsible for fetching financial data from external sources. Our current data for each stock retrieved within a given timeframe will contain each stock's daily fundamentals, including Ticker, Date, High, Low, Open, Close, and Volume.

### Data Processor
The data processor is responsible for pulling the data from local filesystem and dispatch the data to the strategy component via channels. The data processor will read the data asynchronously and serves as the in-memory buffer for the strategy component.
### Strategy
The strategy component contains the logic for making trading decisions based on the input data. Each strategy will be able to process a set of data and determine buy/sell signals. The signals will include the buy/sell quantity and price.

### Backtester
The backtester is responsible for running simulations of trading strategies using historical data. Its interface should allow for running these simulations and retrieving performance metrics to assess how well the strategy performs on the given historical data.

## Future and TODO
Our current implementation can only handle a single ticker with a basic mathematical strategy hard-coded in the system. We aim to provide a more robust user interface in the next few weeks and support analysis on multiple tickers simultaneously (which constitutes a portfolio).

## C++ Features Used
```text
- std::thread / std::mutex / std::condition_variable
- std::filesystem
- std::chrono
- folding expressions
- meta-programming
- std::variant
- std::optional
- std::string_view
- more to be added
```
