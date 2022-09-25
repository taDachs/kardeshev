include_directories(${PROJECT_SOURCE_DIR}/ui/include/ ${PROJECT_SOURCE_DIR}/lib/include/ ${PROJECT_SOURCE_DIR}/util/include/)

add_library(kardeshev_ui
  src/artists.cpp
  src/render.cpp
  src/ui_elements.cpp
  src/window.cpp
)
add_dependencies(kardeshev_ui kardeshev_lib kardeshev_util)
target_link_libraries(kardeshev_ui kardeshev_lib kardeshev_util SDL2::Main SDL2::TTF ${GLM_LIBRARIES})

