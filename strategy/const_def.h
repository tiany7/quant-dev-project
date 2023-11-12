#pragma once
#include <string>

// define enum row type
using RowType = enum {
    kTicker = 0,
    kStartDate,
    kEndDate,
    kPrice,
    kHigh,
    kLow,
    kOpen,
    kClose,
    kVolume,
};

constexpr std::string_view global_path = "/home/tiany7/quant-dev-project/strategy/";