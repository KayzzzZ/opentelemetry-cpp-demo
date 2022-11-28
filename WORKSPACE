workspace(name = "ot-demo")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "io_opentelemetry_cpp",
    sha256 = "2ad0911cdc94fe84a93334773bef4789a38bd1f01e39560cabd4a5c267e823c3",
    strip_prefix = "opentelemetry-cpp-1.7.0",
    urls = [
        "https://github.com/open-telemetry/opentelemetry-cpp/archive/refs/tags/v1.7.0.tar.gz"
    ],
)

# Load OpenTelemetry dependencies after load.
load("@io_opentelemetry_cpp//bazel:repository.bzl", "opentelemetry_cpp_deps")

opentelemetry_cpp_deps()

# Load gRPC dependencies after load.
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

# Load extra gRPC dependencies due to https://github.com/grpc/grpc/issues/20511
load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")

grpc_extra_deps()


## foreign_cc & cmake etc
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Rule repository, note that it's recommended to use a pinned commit to a released version of the rules
http_archive(
   name = "rules_foreign_cc",
   sha256 = "5303e3363fe22cbd265c91fce228f84cf698ab0f98358ccf1d95fba227b308f6",
   strip_prefix = "rules_foreign_cc-0.9.0",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.9.0.zip",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://github.com/bazelbuild/rules_foreign_cc/tree/main/docs#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

_ALL_CONTENT = """\
filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)
"""

## nacos
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
new_git_repository(
    remote = "https://github.com/nacos-group/nacos-sdk-cpp.git",
    commit = "60dc0fb86c5c93ff9c62a637175226f9f2f3860f",
    init_submodules = True,
    recursive_init_submodules = True,
    name = "nacos_sdk_cpp",
    build_file_content = _ALL_CONTENT,
)

#
#load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
#
## pcre source code repository
#new_git_repository(
#    remote = "https://github.com/open-telemetry/opentelemetry-cpp.git",
#    commit = "8bfb9a3f2ded4c8fcbb6a5ba786eebfd20452734",
#    init_submodules = True,
#    recursive_init_submodules = True,
#    name = "opentelemetry_cpp_sdk",
#    build_file_content = _ALL_CONTENT,
#    strip_prefix = "opentelemetry-cpp",
##    strip_prefix = "opentelemetry-cpp-1.7.0",
##    urls = [
##        "https://github.com/open-telemetry/opentelemetry-cpp/archive/refs/tags/v1.7.0.tar.gz"
##    ],
##    sha256 = "2ad0911cdc94fe84a93334773bef4789a38bd1f01e39560cabd4a5c267e823c3",
#)