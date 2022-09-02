cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Kardeshev)

find_package(Boost REQUIRED)
include_directories(include ${Boost_INCLUDE_DIR})
add_library(kardeshev_lib
  src/planets.cpp
  src/id.cpp
  src/util.cpp
  src/planet_generation.cpp
  src/art.cpp
  src/star_generation.cpp
  src/stars.cpp
  src/solar_system_generation.cpp
  src/solar_systems.cpp
  )

add_executable(kardeshev src/main.cpp)

target_link_libraries(kardeshev kardeshev_lib ${Boost_LIBRARIES})
