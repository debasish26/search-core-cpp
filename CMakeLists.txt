cmake_minimum_required(VERSION 3.23)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(app
    src/main.cpp
    src/tokenizer.cpp
)

target_include_directories(app PRIVATE include)