cmake_minimum_required(VERSION 2.8)

project(Integris_test)
cmake_policy(SET CMP0037 NEW)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CxxTest)
find_package(PythonInterp)

option(TESTS_ENABLED "Use CxxTest" ON)

macro(ADD_CXXTEST NAME SOURCES)
    get_filename_component(TEST_NAME ${NAME} NAME_WE [CACHE])
    get_filename_component(OUTDIR ${NAME} DIRECTORY [CACHE])

    if(PYTHONINTERP_FOUND)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${OUTDIR})
        
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.cpp
            COMMAND
            ${PYTHON_EXECUTABLE} ${CXXTEST_PYTHON_TESTGEN_EXECUTABLE}
            --runner=ErrorPrinter
            -o ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.cpp ${NAME} ${ARGN}
            DEPENDS ${ARGN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif(PYTHONINTERP_FOUND)

    set(TEST_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.cpp ${SOURCES})
    add_executable(${TEST_NAME} ${TEST_SOURCES} ${ARGN})
    target_link_libraries(${TEST_NAME} -lpthread)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endmacro(ADD_CXXTEST)

include_directories(.)
set(SOURCES
    ./Tasks/TaskData.cpp)

if (${TESTS_ENABLED})
    ENABLE_TESTING()
    #add_subdirectory(./Tests)

    set(TESTS
        ./Tests/test_Task.h)

    foreach(test_file ${TESTS})
        ADD_CXXTEST(${test_file} ${SOURCES})
        message(STATUS "Test added: ${test_file}")
    endforeach()

endif()

add_executable(task_manager ./main.cpp ${SOURCES})
target_link_libraries(task_manager -lpthread)