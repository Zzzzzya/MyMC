cmake_minimum_required(VERSION 3.20)

project(MyExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(fmt CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE TEST_FILES ${PROJECT_SOURCE_DIR}/test/main.cc)

add_executable(MyExample ${SRC_FILES} ${TEST_FILES})
target_link_libraries(MyExample PRIVATE fmt::fmt)
