cmake_minimum_required(VERSION 2.8)

project(Integris_test)
cmake_policy(SET CMP0037 NEW)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TESTS_ENABLED "Use CxxTest" ON)

include_directories(.)

set(SOURCES
    ./Commands.cpp
    ./ManagerCore.cpp 
    ./Tasks/TaskData.cpp
    ./Utils.cpp)

set(TESTS
    ./Tests/test_Task.h
    ./Tests/test_TaskData.h
    ./Tests/test_ManagerCore.h)

if (${TESTS_ENABLED})

    find_package(CxxTest)
    find_package(PythonInterp)

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

    ENABLE_TESTING()

    foreach(test_file ${TESTS})
        ADD_CXXTEST(${test_file} ${SOURCES})
        message(STATUS "Test added: ${test_file}")
    endforeach()

endif()

add_executable(task_manager ./main.cpp ${SOURCES})
target_link_libraries(task_manager -lpthread)