cmake_minimum_required(VERSION 3.18)
project(c-maps VERSION 0.1.0)

include(CTest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_executable(testTabelaHash ./tests/testTabelaHash.cpp)

target_link_libraries(testTabelaHash gtest_main)

add_test(NAME example_test COMMAND testTabelaHash)

add_library(c-maps c-maps.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
