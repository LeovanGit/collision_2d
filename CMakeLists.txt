cmake_minimum_required(VERSION 3.0)

set(PROJECT collision)
set(INC inc)
set(SRC src)
set(LIB lib)

project(${PROJECT} VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT} ${SRC}/main.cpp ${SRC}/shader.cpp ${SRC}/physics.cpp ${SRC}/vector2.cpp)
target_sources(${PROJECT} PRIVATE ${LIB}/glad.c)
target_link_libraries(${PROJECT} glfw dl)
target_include_directories(${PROJECT} PRIVATE ${INC})
target_include_directories(${PROJECT} PRIVATE ${LIB})
