cmake_minimum_required(VERSION 2.8.7)
project(test CXX)
include_directories(include)
ADD_DEFINITIONS(
    -std=c++11 
)
#file(GLOB SOURCES "src/*.c++")
set(SOURCES3 src/test.c++)

add_executable(test ${SOURCES3})
target_link_libraries(test boost_program_options stdc++)
