cmake_minimum_required(VERSION 3.9.0)
project(c-maps VERSION 0.1.0)

include(GoogleTest)
add_executable(Teste ./tests/testTabelaHash.cpp)
gtest_add_tests(TARGET Teste
                TEST_SUFIX .noArgs
                TEST_LIST noArgsTests
)
enable_testing()

add_library(c-maps c-maps.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
