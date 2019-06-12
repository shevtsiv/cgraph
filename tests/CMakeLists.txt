cmake_minimum_required(VERSION 3.13)
project(cgraph)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(RunTests tests.cpp)
target_link_libraries(RunTests ${GTEST_LIBRARIES} pthread)
