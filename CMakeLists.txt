cmake_minimum_required(VERSION 3.16)

project(SilksongdleSolver LANGUAGES C CXX)

# options

option(DEBUG_INFO "Enable debug information" OFF)

# language standards

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# debug configuration

if(DEBUG_INFO)
    add_compile_definitions(DEBUG_INFO)

    add_compile_options(
        -g
        -O0
    )
endif()

# imgui

find_package(SDL3 REQUIRED CONFIG)

file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS imgui/*.cpp)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
    imgui
    imgui/backends
)

# GUI

file(GLOB GUI_SOURCES CONFIGUER_DEPENDS
    gui/*.cc
)

add_library(gui STATIC 
    ${GUI_SOURCES}
    imgui/backends/imgui_impl_sdl3.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(gui PUBLIC gui)

target_link_libraries(gui PRIVATE
    imgui
    SDL3::SDL3
)

# core program

file(GLOB CORE_SOURCES CONFIGURE_DEPENDS core/*.c)

add_executable(main ${CORE_SOURCES})

target_link_libraries(main PRIVATE gui)
