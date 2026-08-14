cmake_minimum_required(VERSION 3.16)

project(SilksongdleSolver LANGUAGES C CXX)

# options
option(BUILD_GUI "Build the GUI application" OFF)

# language standards

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (BUILD_GUI)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_options(-g -O0) # debug flags
# add_compile_options(-O3) # optimization flags


# core program

file(GLOB CORE_SOURCES CONFIGURE_DEPENDS
    core/*.c
)

add_library(core STATIC
    ${CORE_SOURCES}
)

target_include_directories(core PUBLIC
    core
)

target_link_libraries(core PUBLIC m)

# CLI
if (NOT BUILD_GUI)
    add_executable(main cli/main.c)
    target_link_libraries(main PRIVATE core)
endif()

# imgui
if (BUILD_GUI)
    find_package(SDL3 REQUIRED CONFIG)

    file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS imgui/*.cpp)

    add_library(imgui STATIC ${IMGUI_SOURCES})

    target_include_directories(imgui PUBLIC
        imgui
        imgui/backends
    )

endif()

# GUI
if (BUILD_GUI)
    find_package(OpenGL REQUIRED)

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
        OpenGL::GL
    )

    add_executable(main app/main.c)

    target_link_libraries(main PRIVATE
        core
        gui
    )

endif()

