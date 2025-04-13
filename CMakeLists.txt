cmake_minimum_required(VERSION 3.10)
project(sensor-st LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(CAIROMM REQUIRED cairomm-1.0)

# JSON library (header-only)
find_path(NLOHMANN_JSON_INCLUDE_DIR nlohmann/json.hpp)

# Include directories
include_directories(
    ${GTKMM_INCLUDE_DIRS}
    ${CAIROMM_INCLUDE_DIRS}
    ${NLOHMANN_JSON_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

link_directories(
    ${GTKMM_LIBRARY_DIRS}
    ${CAIROMM_LIBRARY_DIRS}
)

# Sources
set(SOURCES
    main.cpp
    config.cpp
    sensor.cpp
    temp_logic.cpp
    gui.cpp
)

set(HEADERS
    config.hpp
    sensor.hpp
    temp_logic.hpp
    gui.hpp
)

# Executable
add_executable(sensor-st ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(sensor-st
    ${GTKMM_LIBRARIES}
    ${CAIROMM_LIBRARIES}
)

# For Yocto to avoid rpath issues
set_target_properties(sensor-st PROPERTIES INSTALL_RPATH "")
