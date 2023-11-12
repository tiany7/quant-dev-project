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
