cmake_minimum_required(VERSION 3.10)

# Project name
project(ImGuiOpenGLExample)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add your source files
set(SOURCES
    main.cpp
    source/window.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# Add the include directories for ImGui
include_directories(
    imgui
    imgui/backends
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    /usr/include
    include
)

# Find and link GLEW and GLFW
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Link against OpenGL, GLEW, and GLFW
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::GLEW glfw)
