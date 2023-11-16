cc_library(
    name = "data_crawler",
    srcs = ["data_crawler.cpp"],
    hdrs = ["data_crawler.h"],
    deps = [
        "@json_lib//:json",
        #"@curl//:curl",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "crawler_main",
    srcs = ["new_main.cpp"],
    copts = ["-std=c++17"],
    deps = [
        "@json_lib//:json",
        ":data_crawler",
    ],
    linkopts = ["-L./curl", "-lcurl"], 
)