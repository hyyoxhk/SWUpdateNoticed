cmake_minimum_required(VERSION 3.14)

project(SWUpdateNoticed VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(Qt5 COMPONENTS Core Quick Qml LinguistTools REQUIRED)
find_library(SWUPDATE_LIBRARY NAMES swupdate)

add_subdirectory(src)
