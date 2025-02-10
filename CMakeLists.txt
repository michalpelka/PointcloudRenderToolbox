cmake_minimum_required(VERSION 3.10)

# Project name
project(ImGuiOpenGLExample)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add your source files
set(SOURCES

    source/PRTWindow.cpp
    source/PRTAuxDraw.cpp
    source/PRTPointcloud.cpp
    source/CompileShader.cpp
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

include(FetchContent)

# Fetch GLEW
FetchContent_Declare(
        GLEW
        GIT_REPOSITORY https://github.com/nigels-com/glew.git
        IT_TAG glew-2.1.0
)

# Fetch GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# Link against OpenGL, GLEW, and GLFW
add_executable(sandbox  main.cpp ${SOURCES})

target_link_libraries(sandbox OpenGL::GL GLEW glfw)


add_executable(helloWindow  examples/helloWindow.cpp ${SOURCES})
target_link_libraries(helloWindow OpenGL::GL GLEW glfw)