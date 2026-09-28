cmake_minimum_required(VERSION 3.16)
project(llm_inference_performance_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(inference_core
    src/scheduler.cpp
    src/kv_cache.cpp
    src/metrics.cpp
    src/simulator.cpp
)
target_include_directories(inference_core PUBLIC include)

add_executable(inference_benchmark src/main.cpp)
target_link_libraries(inference_benchmark PRIVATE inference_core)

enable_testing()

add_executable(scheduler_test tests/scheduler_test.cpp)
target_link_libraries(scheduler_test PRIVATE inference_core)
add_test(NAME scheduler_test COMMAND scheduler_test)

add_executable(kv_cache_test tests/kv_cache_test.cpp)
target_link_libraries(kv_cache_test PRIVATE inference_core)
add_test(NAME kv_cache_test COMMAND kv_cache_test)
