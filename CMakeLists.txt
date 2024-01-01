cmake_minimum_required(VERSION 3.16)
project(Iona-desktop)

set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(core_display)

qt6_finalize_project()
