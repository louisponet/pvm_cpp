load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "com_google_benchmark",
  urls = ["https://github.com/google/benchmark/archive/d572f4777349d43653b21d6c2fc63020ab326db2.zip"],
  strip_prefix = "benchmark-d572f4777349d43653b21d6c2fc63020ab326db2",
)

http_archive(
    name = "com_grail_bazel_compdb",
    strip_prefix = "bazel-compilation-database-0.5.2",
    urls = ["https://github.com/grailbio/bazel-compilation-database/archive/0.5.2.tar.gz"],
)
load("@com_grail_bazel_compdb//:deps.bzl", "bazel_compdb_deps")
bazel_compdb_deps()

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
  remote = "https://github.com/nlohmann/json",
  name = "nlohmann_json",
  branch = "develop"
)

http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/3d787f5a0d58cfc37a0563bb15647a0d8aa2c1bf.zip"],
  strip_prefix = "googletest-3d787f5a0d58cfc37a0563bb15647a0d8aa2c1bf",
)

http_archive(
	name = "crow",
	url = "https://github.com/CrowCpp/Crow/releases/download/v1.0%2B5/crow-v1.0+5.tar.gz",
	build_file = "crow.BUILD",
)

git_repository(
  remote = "https://github.com/skypjack/entt",
  name = "entt",
  branch = "v3.11.1"
)

http_archive(
  name = "httplib",
  url = "https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.11.3.tar.gz",
  build_file = "httplib.BUILD",
)

http_archive(
  name = "fmt",
  url = "https://github.com/fmtlib/fmt/releases/download/9.1.0/fmt-9.1.0.zip",
  build_file = "fmt.BUILD",
  strip_prefix = "fmt-9.1.0",
)

# Boost
# Famous C++ library that has given rise to many new additions to the C++ Standard Library
# Makes @boost available for use: For example, add `@boost//:algorithm` to your deps.
# For more, see https://github.com/nelhage/rules_boost and https://www.boost.org
http_archive(
    name = "com_github_nelhage_rules_boost",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/nelhage/rules_boost/archive/96e9b631f104b43a53c21c87b01ac538ad6f3b48.tar.gz",
    strip_prefix = "rules_boost-96e9b631f104b43a53c21c87b01ac538ad6f3b48",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'com_github_nelhage_rules_boost' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
)
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

git_repository(
  remote = "https://github.com/louisponet/bazel-toolchain",
  name = "com_grail_bazel_toolchain",
  branch = "master"
)

load("@com_grail_bazel_toolchain//toolchain:deps.bzl", "bazel_toolchain_dependencies")

bazel_toolchain_dependencies()

load("@com_grail_bazel_toolchain//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    llvm_version="15.0.6",
    cxx_standard={"":"c++20"},
    stdlib={"":"stdc++"},
	link_flags={"":["-ldl"]}
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()
