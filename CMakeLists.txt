cmake_minimum_required(VERSION 3.0.0)
project(sudokuSolver VERSION 0.1.0)

file(GLOB SOURCES "src/*.cpp")
add_executable(sudokuSolver main.cpp ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(sudokuSolver PRIVATE Threads::Threads)

target_include_directories(sudokuSolver PRIVATE includes)
target_include_directories(sudokuSolver PRIVATE .)

include(CPack)
