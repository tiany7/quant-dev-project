

# load("@bazel_tools//tools/cpp:cc_rules.bzl", "cc_library")

# cc_library(
#     name = "strategy",
#     srcs = ["base_strategy.cpp"],
#     hdrs = ["base_strategy.h"],
#     visibility = ["//visibility:public"],
# )

cc_library(
    name = "csv_reader",
    hdrs = ["CSVReader.h"],
    visibility = ["//visibility:public"],
    copts = ["--std=c++20"],
)

cc_library(
    name = "const_def",
    hdrs = ["const_def.h"],
    copts = ["--std=c++20"],
    visibility = ["//visibility:public"],
)


cc_library(
    name = "utils",
    hdrs = ["utils.h"],
    copts = ["--std=c++20"],
    visibility = ["//visibility:public"],
)
cc_binary(
    name = "main",
    srcs = ["server_main.cpp"],
    deps = [":csv_reader", ":const_def"],
    copts = ["--std=c++20"],
)

