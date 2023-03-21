cmake_minimum_required(VERSION 3.14)

# Define project name and version
project("segundo" VERSION 1.0)

# Define C++ standard to use
set(CMAKE_CXX_STANDARD 11)

# Find SFML library
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Define the path where the ImGui library will be cloned
set(IMGUI_PATH ${CMAKE_CURRENT_BINARY_DIR}/imgui)

# Clone the ImGui library from its GitHub repository
IF(NOT EXISTS "${IMGUI_PATH}")
    execute_process(
        COMMAND git clone https://github.com/ocornut/imgui.git ${IMGUI_PATH}
    )
ELSE()
    execute_process(
        COMMAND pushd ${IMGUI_PATH} && git pull && popd
    )
ENDIF()

# Clone the ImGui-SFML library from its GitHub repository
IF(NOT EXISTS "${IMGUI_PATH}/imgui-sfml")
    execute_process(
        COMMAND git clone https://github.com/eliasdaler/imgui-sfml.git "${IMGUI_PATH}/imgui-sfml"
    )
ELSE()
    execute_process(
        COMMAND pushd "${IMGUI_PATH}/imgui-sfml" && git pull && popd
    )
ENDIF()

# Add the ImGui and ImGui-SFML libraries to the project
add_library(imgui STATIC
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
)

add_library(imgui-sfml STATIC
    ${IMGUI_PATH}/imgui-sfml/imgui-SFML.cpp
)

# Add include directories for the ImGui and SFML libraries
target_include_directories(imgui PUBLIC ${IMGUI_PATH})
target_include_directories(imgui-sfml PUBLIC ${IMGUI_PATH}/imgui-sfml)
target_include_directories(imgui-sfml PUBLIC ${IMGUI_PATH})
 
include_directories(include)
include_directories("dependencies/Box2D/Include")
add_library(box2d STATIC IMPORTED)

add_executable(segundo src/main.cpp src/toolbox.cpp src/button_sprite.cpp)

target_include_directories("segundo" PUBLIC ${SFML_INCLUDE_DIRS})
target_link_libraries("segundo" imgui imgui-sfml sfml-graphics sfml-audio GL)

