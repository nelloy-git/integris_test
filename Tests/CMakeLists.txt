cmake_minimum_required(VERSION 2.8)

project(Integris_test)
cmake_policy(SET CMP0037 NEW)

set(TESTS
    ./test_TaskEmpty.h)

find_package(CxxTest)
find_package(PythonInterp)

MACRO(ADD_CXXTEST NAME)
    IF(PYTHONINTERP_FOUND)
        get_filename_component(OUTDIR ${NAME} DIRECTORY [CACHE])
        set(OUTDIR ${OUTDIR}/TestsCpp)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${OUTDIR})
        
        ADD_CUSTOM_COMMAND(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${OUTDIR}/${NAME}.cpp
            COMMAND
            ${PYTHON_EXECUTABLE} ${CXXTEST_PYTHON_TESTGEN_EXECUTABLE}
            --runner=ErrorPrinter
            -o ${CMAKE_CURRENT_BINARY_DIR}/${OUTDIR}/${NAME}.cpp ${NAME} ${ARGN}
            DEPENDS ${ARGN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ENDIF(PYTHONINTERP_FOUND)

    get_filename_component(TEST_NAME ${NAME} NAME_WE [CACHE])
    add_executable(${TEST_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${OUTDIR}/${NAME}.cpp ${ARGN})

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
ENDMACRO(ADD_CXXTEST)

foreach(test_file ${TESTS})
    message(STATUS "Test added: ${test_file}")
    ADD_CXXTEST(${test_file})
endforeach()

#ADD_CXXTEST(${TESTS})