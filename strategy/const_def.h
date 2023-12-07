#pragma once
#include <string>

// define enum row type
using RowType = enum {
    kVolume = 0,
    kClose,
    kOpen,
    kLow,
    kHigh,
    kPrice,
    kEndDate,
    kStartDate,
    kTicker,
};


// constexpr std::string_view global_path = "/home/tiany7/quant-dev-project/strategy/";
constexpr std::string_view global_path = "/Users/zzhu/Documents/Columbia/COMS4995/proj/quant-dev-project/strategy/";


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