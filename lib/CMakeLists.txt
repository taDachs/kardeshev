include_directories(${PROJECT_SOURCE_DIR}/lib/include/)
add_library(kardeshev_lib STATIC
  src/buildings.cpp
  src/id.cpp
  src/planets.cpp
  src/population.cpp
  src/solar_systems.cpp
  src/stars.cpp
  src/asteroid.cpp
)
target_link_libraries(kardeshev_lib ${GLM_LIBRARIES} ${Boost_LIBRARIES})
