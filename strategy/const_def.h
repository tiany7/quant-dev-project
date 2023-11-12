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


using kTickerType = std::string;
using kStartDateType = std::string;
using kEndDateType = std::string;
using kPriceType = double;
using kHighType = double;
using kLowType = double;
using kOpenType = double;
using kCloseType = double;
using kVolumeType = double;

#define CSV_ROW_TYPES kVolumeType, kCloseType, kOpenType, kLowType, kHighType, kPriceType, kEndDateType, kStartDateType, kTickerType