cmake_minimum_required(VERSION 3.0)

project(Collision VERSION 1.0 LANGUAGES C CXX)

add_executable(ex ./src/main.cpp ./src/shader.cpp)
target_sources(ex PRIVATE ./lib/glad.c)

target_link_libraries(ex glfw dl)
