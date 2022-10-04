include_directories(${PROJECT_SOURCE_DIR}/util/include/)
add_library(kardeshev_util STATIC
  src/util.cpp
  src/logger.cpp
  src/random_generators.cpp
)
