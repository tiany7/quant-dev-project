load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "bazel_tools",
    urls = ["https://github.com/bazelbuild/bazel_tools/archive/4.2.1.tar.gz"],
    strip_prefix = "bazel_tools-4.2.1",
)

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "build_bazel_rules_cc",
    remote = "https://github.com/bazelbuild/rules_cc.git",
    commit = "9cf0a9c",
)

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "json_lib",
    remote = "https://github.com/nlohmann/json.git",
    commit = "a3e6e26dc83a726b292f5be0492fcc408663ce55",
)


git_repository(
    name = "curl",
    remote = "https://github.com/curl/curl.git",
    commit = "817204c6e41f66dafbaa704d67f828a4288b3577",
)