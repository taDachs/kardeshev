include_directories(${PROJECT_SOURCE_DIR}/kardeshev/include/ ${PROJECT_SOURCE_DIR}/generation/include/ ${PROJECT_SOURCE_DIR}/lib/include/ ${PROJECT_SOURCE_DIR}/util/include/ ${PROJECT_SOURCE_DIR}/ui/include/)

add_executable(kardeshev src/main.cpp)

target_link_libraries(kardeshev kardeshev_lib kardeshev_generation kardeshev_util kardeshev_ui)
