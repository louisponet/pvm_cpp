load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

local_repository(
	name = "com_github_google_benchmark",
	path = "/home/ponet/Software/benchmark"
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
    name = "gtest",
    url = "https://github.com/google/googletest/archive/release-1.10.0.zip",
    sha256 = "94c634d499558a76fa649edb13721dce6e98fb1e7018dfaeba3cd7a083945e91",
    strip_prefix = "googletest-release-1.10.0",
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
  build_file = "fmt.BUILD"
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

