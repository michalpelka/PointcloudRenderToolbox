cmake_minimum_required(VERSION 3.10)


# Project name
project(ImGuiOpenGLExample)

get_filename_component(REPOSITORY_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
set(EXTERNAL_LIBRARIES_DIRECTORY ${REPOSITORY_DIRECTORY}/3rd)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(${EXTERNAL_LIBRARIES_DIRECTORY}/glfw)

set(BUILD_UTILS OFF CACHE BOOL "" FORCE)

include_directories(${EXTERNAL_LIBRARIES_DIRECTORY}/glm/)

find_package(OpenGL REQUIRED)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add your source files
set(SOURCES
        source/PRTWindow.cpp
        source/PRTAuxDraw.cpp
        source/PRTPointcloud.cpp
        source/CompileShader.cpp
        ${EXTERNAL_LIBRARIES_DIRECTORY}/imgui/imgui.cpp
        ${EXTERNAL_LIBRARIES_DIRECTORY}/imgui/imgui_demo.cpp
        ${EXTERNAL_LIBRARIES_DIRECTORY}/imgui/imgui_draw.cpp
        ${EXTERNAL_LIBRARIES_DIRECTORY}/imgui/imgui_widgets.cpp
        ${EXTERNAL_LIBRARIES_DIRECTORY}/imgui/imgui_tables.cpp
        ${EXTERNAL_LIBRARIES_DIRECTORY}/imgui/backends/imgui_impl_glfw.cpp
        ${EXTERNAL_LIBRARIES_DIRECTORY}/imgui/backends/imgui_impl_opengl3.cpp
        ${EXTERNAL_LIBRARIES_DIRECTORY}/glew-2.1.0/src/glew.c

)

# Add the include directories for ImGui
include_directories(
        ${EXTERNAL_LIBRARIES_DIRECTORY}/imgui
        ${EXTERNAL_LIBRARIES_DIRECTORY}/imgui/backends
        ${GLFW_INCLUDE_DIRS}
        ${EXTERNAL_LIBRARIES_DIRECTORY}/glew-2.1.0/include
        ${EXTERNAL_LIBRARIES_DIRECTORY}
        /usr/include
        include
)

# Link against OpenGL, GLEW, and GLFW
add_executable(sandbox  main.cpp ${SOURCES})

target_link_libraries(sandbox OpenGL::GL glfw)


add_executable(helloWindow  examples/helloWindow.cpp ${SOURCES})
target_link_libraries(helloWindow OpenGL::GL glfw)

add_executable(plyViewer  examples/ply_viewer.cpp ${SOURCES})
target_link_libraries(plyViewer OpenGL::GL glfw)