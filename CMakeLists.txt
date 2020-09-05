cmake_minimum_required(VERSION 3.0.0)
project(sudokuSolver VERSION 0.1.0)

file(GLOB SOURCES "src/*.cpp")
add_executable(sudokuSolver main.cpp ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_include_directories(sudokuSolver PRIVATE includes)

include(CPack)
