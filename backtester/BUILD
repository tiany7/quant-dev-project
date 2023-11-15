cc_library(
    name = "backtest",
    srcs = ["backtester.cpp"],
    hdrs = ["backtester.h"],
    deps = ["//strategy:csv_reader", 
            "//strategy:example_strategy", 
            "//strategy:utils",],
    visibility = ["//visibility:public"],
    copts = ["--std=c++20"],
)