cmake_minimum_required(VERSION 2.8)

project(Integris_test)
cmake_policy(SET CMP0037 NEW)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TESTS_ENABLED "Use CxxTest" ON)

if (${TESTS_ENABLED})
    ENABLE_TESTING()
    add_subdirectory(./Tests)
endif()

include_directories(.)

set(SOURCES
    ./main.cpp)

add_executable(task_manager ${SOURCES})
target_link_libraries(task_manager -lpthread)