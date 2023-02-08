cmake_minimum_required(VERSION 3.5)

project(hook)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(hook SHARED
    hook.h
    hook.cpp
)
target_compile_definitions(hook PUBLIC HOOK_LIBRARY)
target_link_libraries(hook PRIVATE Qt6::Core)
