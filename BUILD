cc_library(
    name = "filter_lib",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    strip_include_prefix = "include",
)

cc_test(
    name = "filter_test",
    srcs = glob(["test/**/*.cpp"]),
    deps = [
        ":filter_lib",
        "@googletest//:gtest_main",
    ],
)
