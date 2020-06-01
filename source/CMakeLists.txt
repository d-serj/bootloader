cmake_minimum_required(VERSION 3.10)

project("application" C ASM)

add_executable(${TARGET_NAME} "")

target_sources(
  ${TARGET_NAME}
  PRIVATE
  main.c
)

target_include_directories(
  ${TARGET_NAME}
  PRIVATE
  ${CMAKE_SOURCE_DIR}/libopencm3/include
)

target_link_libraries(
  ${TARGET_NAME}
  PRIVATE
  libopencm3
)

# Linker script and .map file
target_link_options(
  ${TARGET_NAME} PRIVATE
  -Wl,-Map,${EXECUTABLE_OUTPUT_PATH}/${TARGET_NAME}.map
  -T ${CMAKE_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld
)

# Show size of the output binary
add_custom_command(
  TARGET ${TARGET_NAME} 
  POST_BUILD
  COMMAND ${CMAKE_SIZE_UTIL} -B
  "${TARGET_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# Generate binary file
add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -v -O binary
  "${TARGET_NAME}${CMAKE_EXECUTABLE_SUFFIX}" "${BINARY_NAME}"
  COMMENT "\nGenerating binary file"
  WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
