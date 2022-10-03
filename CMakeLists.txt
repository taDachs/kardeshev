cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Kardeshev)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


find_package(Boost REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glm REQUIRED)

include_directories(${Boost_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS})

add_subdirectory(util)
add_subdirectory(lib)
add_subdirectory(ui)
add_subdirectory(generation)
add_subdirectory(kardeshev)

