cmake_minimum_required(VERSION 3.10)

# Project name
project(sensor-st)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Find required Qt packages
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Include directories
include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Define the executable
add_executable(sensor-st
    main.cpp
    gui.cpp
    temp_logic.cpp
    sensor.cpp
    config.cpp
)

# Link libraries
target_link_libraries(sensor-st
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    cairo
)

# Install targets (optional)
install(TARGETS sensor-st DESTINATION bin)
