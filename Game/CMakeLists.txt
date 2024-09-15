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
find_package(unofficial-noise CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/GamePlay)
include_directories(${PROJECT_SOURCE_DIR}/include/Header)
include_directories(${PROJECT_SOURCE_DIR}/include/UI)
include_directories(${PROJECT_SOURCE_DIR}/include/Render)
include_directories(${PROJECT_SOURCE_DIR}/include/Map)
include_directories(${PROJECT_SOURCE_DIR}/include/Physics)
include_directories(${PROJECT_SOURCE_DIR}/include/Culling)

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
target_link_libraries(MyMC PRIVATE unofficial::noise::noise)

# 设置项目的版本号 (用于生成安装包)
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

# 定义安装包的名字和类型
set(CPACK_PACKAGE_NAME "MyMC")

# 生成安装包的类型 (ZIP, TGZ, DEB, RPM 等)
set(CPACK_GENERATOR "ZIP")

# 安装规则
# 安装可执行文件到 bin 目录
install(TARGETS MyMC DESTINATION MC)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/res DESTINATION .)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/shaders DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/build/fmtd.dll DESTINATION MC)
install(FILES ${PROJECT_SOURCE_DIR}/build/glew32d.dll DESTINATION MC)
install(FILES ${PROJECT_SOURCE_DIR}/build/glfw3.dll DESTINATION MC)
install(FILES ${PROJECT_SOURCE_DIR}/build/noise.dll DESTINATION MC)
install(FILES ${PROJECT_SOURCE_DIR}/build/spdlogd.dll DESTINATION MC)
install(FILES ${PROJECT_SOURCE_DIR}/build/imgui.ini DESTINATION MC)
install(FILES ${PROJECT_SOURCE_DIR}/build/MyMC.pdb DESTINATION MC)

install(FILES ${PROJECT_SOURCE_DIR}/docs/refs/readme.md DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/docs/refs/versions.md DESTINATION .)

# install(FILES ${PROJECT_SOURCE_DIR}/build/spdlog.dll DESTINATION bin)
# install(FILES ${PROJECT_SOURCE_DIR}/build/fmt.dll DESTINATION bin)
# install(FILES ${PROJECT_SOURCE_DIR}/build/glew32.dll DESTINATION bin)

# 打包配置
include(CPack)
