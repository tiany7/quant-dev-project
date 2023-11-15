
cc_binary(
    name = "my_app",
    srcs = ["main.cpp"],
    copts = ["-std=c++17"],
    deps = [
        "@json_lib//:json",
        # ... any other dependencies
    ],
    linkopts = ["-lcurl"],
)

