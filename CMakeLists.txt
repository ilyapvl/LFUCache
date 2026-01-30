cmake_minimum_required(VERSION 3.14)
project(CacheSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    include
    include/exceptions
)

add_executable(main
    src/main.cpp
)

target_include_directories(main PRIVATE src)

find_package(GTest REQUIRED)
enable_testing()

add_executable(test_lfu 
    test/test_lfu.cpp 
)
add_executable(test_optimal 
    test/test_optimal.cpp 
)

target_link_libraries(test_lfu GTest::gtest GTest::gtest_main)
target_link_libraries(test_optimal GTest::gtest GTest::gtest_main)

target_include_directories(test_lfu PRIVATE src)
target_include_directories(test_optimal PRIVATE src)

add_test(NAME LFUCacheTest COMMAND test_lfu)
add_test(NAME OptimalCacheTest COMMAND test_optimal)
