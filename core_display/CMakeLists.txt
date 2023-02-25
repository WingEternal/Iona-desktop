cmake_minimum_required(VERSION 3.16)

project(core_display VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia OpenGL OpenGLWidgets Core5Compat)
find_package(OpenGL REQUIRED)

qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

###########################
# Set up cubism framework #
###########################
set(THIRD_PARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
set(CUBISM_PATH ${THIRD_PARTY_PATH}/cubism)
set(CUBISM_CORE_PATH ${CUBISM_PATH}/core)
set(GL_HANDLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/iona_gl)
set(STB_PATH ${THIRD_PARTY_PATH}/stb)

set(CMAKE_SUPPRESS_REGENERATION ON)

# Determine win / linux
add_library(Live2DCubismCore STATIC IMPORTED)
if(CMAKE_HOST_WIN32 AND MSVC_VERSION GREATER_EQUAL 1920)
    set_target_properties(Live2DCubismCore
    PROPERTIES 
        IMPORTED_LOCATION_DEBUG ${CUBISM_CORE_PATH}/lib/windows/Live2DCubismCore_MDd.lib
        IMPORTED_LOCATION_RELEASE ${CUBISM_CORE_PATH}/lib/windows/Live2DCubismCore_MD.lib
        INTERFACE_INCLUDE_DIRECTORIES ${CUBISM_CORE_PATH}/include
    )
elseif(CMAKE_HOST_UNIX)
    set_target_properties(Live2DCubismCore
    PROPERTIES 
        IMPORTED_LOCATION ${CUBISM_CORE_PATH}/lib/linux/libLive2DCubismCore.a
        INTERFACE_INCLUDE_DIRECTORIES ${CUBISM_CORE_PATH}/include
    )
endif()

set(FRAMEWORK_SOURCE OpenGL)
add_subdirectory(${CUBISM_PATH} ${CMAKE_CURRENT_BINARY_DIR}/Framework)
target_include_directories(Framework PUBLIC ${GL_HANDLE_PATH})
target_link_libraries(Framework Live2DCubismCore Qt6::OpenGL ${OPENGL_LIBRARIES})

######################
# Build Core-display #
######################
set(APP_NAME core_display)
if(CMAKE_HOST_WIN32)
    set(APP_WINRC app_win32.rc)
    set_property(SOURCE app_win32.rc APPEND PROPERTY
        OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/icon.ico
    )
    add_executable(${APP_NAME} ${APP_WINRC})
elseif(CMAKE_HOST_UNIX)
    add_executable(${APP_NAME})
endif()
add_subdirectory(app)
add_subdirectory(asm)
add_subdirectory(iona_gl)
add_subdirectory(move_ctrl)
add_subdirectory(quick_cmds)
add_subdirectory(tray)
add_subdirectory(voice)

set(RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
target_sources(${APP_NAME} PRIVATE ${RESOURCE_PATH}/resources.qrc)

target_include_directories(${APP_NAME} 
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CUBISM_CORE_PATH}/include
        ${CUBISM_PATH}/framework
        ${CMAKE_CURRENT_SOURCE_DIR}/../hook
        ${STB_PATH})

add_dependencies(${APP_NAME} Framework)

target_link_libraries(${APP_NAME} 
PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Core5Compat
    hook
    Framework
    ${OPENGL_LIBRARIES}
)

set_target_properties(${APP_NAME} 
PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${APP_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_finalize_executable(${APP_NAME})
