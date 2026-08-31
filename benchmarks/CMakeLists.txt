# Benchmark harness for Guillaume.
#
# Built only when BUILD_BENCHMARKS=ON. Uses Google Benchmark to measure the key
# hot paths: layout/entity placement, redraw/render-system pass, event
# dispatch throughput, and memory footprint.

include(FetchContent)
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.1
)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(guillaume_benchmarks
    benchmark_main.cpp
)

target_link_libraries(guillaume_benchmarks PRIVATE
    guillaume
    benchmark::benchmark
)

target_include_directories(guillaume_benchmarks PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
