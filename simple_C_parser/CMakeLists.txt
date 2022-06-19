cmake_minimum_required(VERSION 3.13)

add_executable(${PROJECT_NAME} "main.cpp")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)