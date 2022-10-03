include_directories(${PROJECT_SOURCE_DIR}/generation/include/ ${PROJECT_SOURCE_DIR}/util/include ${PROJECT_SOURCE_DIR}/lib/include)
add_library(kardeshev_generation
  src/galaxy_generation.cpp
  src/planet_generation.cpp
  src/solar_system_generation.cpp
  src/star_generation.cpp
  src/name_generation.cpp
)

add_dependencies(kardeshev_generation kardeshev_lib kardeshev_util)
target_link_libraries(kardeshev_generation kardeshev_lib kardeshev_util)
