cmake_minimum_required(VERSION 3.20)

project(MyMC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/GamePlay)
include_directories(${PROJECT_SOURCE_DIR}/include/Header)
include_directories(${PROJECT_SOURCE_DIR}/include/UI)
include_directories(${PROJECT_SOURCE_DIR}/include/Render)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE TEST_FILES ${PROJECT_SOURCE_DIR}/test/main.cc)

add_executable(MyMC ${SRC_FILES} ${TEST_FILES})

target_link_libraries(MyMC PRIVATE glfw)
target_link_libraries(MyMC PRIVATE OpenGL::GL)
target_link_libraries(MyMC PRIVATE GLEW::GLEW)
target_link_libraries(MyMC PRIVATE glm::glm)
target_link_libraries(MyMC PRIVATE imgui::imgui)
target_link_directories(MyMC PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(MyMC PRIVATE assimp::assimp)
target_link_libraries(MyMC PRIVATE spdlog::spdlog)
