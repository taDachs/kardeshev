include_directories(${PROJECT_SOURCE_DIR}/ui/include/ ${PROJECT_SOURCE_DIR}/lib/include/ ${PROJECT_SOURCE_DIR}/generation/include/ ${PROJECT_SOURCE_DIR}/util/include/)

add_library(kardeshev_ui
  src/render.cpp
  src/ui_elements.cpp
  src/window.cpp
  src/assets.cpp
  src/entities.cpp
  src/view.cpp
  src/ui_state.cpp
)
add_dependencies(kardeshev_ui kardeshev_lib kardeshev_util kardeshev_generation)
target_link_libraries(kardeshev_ui kardeshev_lib kardeshev_util kardeshev_generation SDL2::Main SDL2::TTF SDL2::Image ${GLM_LIBRARIES})

