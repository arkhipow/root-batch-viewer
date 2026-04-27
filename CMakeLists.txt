cmake_minimum_required(VERSION 4.3.0)

project(RootBatchViewer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glad)

add_library(glad STATIC
    ${GLAD_DIR}/src/glad.c
)

target_include_directories(glad PUBLIC
    ${GLAD_DIR}/include
)

# GLFW
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "Build the GLFW test programs"    FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "Build the GLFW documentation"    FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "Generate installation target"    FORCE)

add_subdirectory(${GLFW_DIR})

# IMGUI
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GLFW_DIR}/include
)

# IMPlot
set(IMPLOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/implot)

add_library(implot STATIC
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

target_include_directories(implot PUBLIC
    ${IMPLOT_DIR}
)

target_link_libraries(implot PUBLIC
    imgui
)

# RootBatchViewer
set(ROOT_BATCH_VIEWER_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(root_batch_viewer
    ${ROOT_BATCH_VIEWER_DIR}/src/application.cpp
    ${ROOT_BATCH_VIEWER_DIR}/src/main.cpp
)

target_include_directories(root_batch_viewer PRIVATE
    ${ROOT_BATCH_VIEWER_DIR}/include
)

target_link_libraries(root_batch_viewer PRIVATE
    ${OPENGL_LIBRARIES}
    glad
    glfw
    imgui
    implot
)