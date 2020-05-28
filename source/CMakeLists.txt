cmake_minimum_required(VERSION 3.10)

project("application" C ASM)

add_executable(${PROJECT_NAME} "")

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  main.c
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  ${CMAKE_SOURCE_DIR}/libopencm3/include
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  libopencm3
)  
