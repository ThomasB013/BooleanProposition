cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

#Specify the project
project(main.exe)
add_executable(${PROJECT_NAME} main.cpp tokenizer.cpp expression.cpp parse.cpp)
