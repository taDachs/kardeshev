cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Kardeshev)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


find_package(Boost REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glm REQUIRED)
include_directories(include ${Boost_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS})
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
  src/population.cpp
  src/buildings.cpp
  src/render.cpp
  src/galaxy_generation.cpp
  )

add_executable(kardeshev src/main.cpp)

target_link_libraries(kardeshev kardeshev_lib ${Boost_LIBRARIES} SDL2::Main SDL2::TTF ${GLM_LIBRARIES})
