cmake_minimum_required(VERSION 3.5)

project(Iona-desktop)

add_subdirectory(hook)
add_subdirectory(core_display)

add_dependencies(core_display hook)

qt6_finalize_project()