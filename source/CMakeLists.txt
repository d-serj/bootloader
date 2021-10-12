cmake_minimum_required(VERSION 3.10)

project("application" C ASM)

add_executable(${TARGET_NAME} "")

target_sources(
  ${TARGET_NAME} PRIVATE
  system/assert.c
  system/system.c
  utilities/ringbuffer.c
  utilities/crc32.c
  bootloader.c
  delay.c
  main.c
  image.c
  storage/storage.c
  storage/storage_internal.c
  storage/storage_sim800.c
  drivers/usart_driver.c
  drivers/flash.c
  drivers/bkp.c
  drivers/timer.c
  drivers/led.c
  modem/sim800/sim800.c
  comhdlc/com.c
  comhdlc/tinyframe/TinyFrame.c
)

target_include_directories(
  ${TARGET_NAME}
  PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/libopencm3/include
)

target_link_libraries(
  ${TARGET_NAME}
  PRIVATE
  libopencm3
)

set_target_properties(
  ${TARGET_NAME} PROPERTIES
  LINK_DEPENDS "${LINKER_SCRIPT}"
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
  COMMAND ${CMAKE_OBJCOPY} --pad-to=0x4000 --gap-fill=0xFF -v -O binary
  "${TARGET_NAME}${CMAKE_EXECUTABLE_SUFFIX}" "${BINARY_NAME}"
  COMMENT "\nGenerating binary file"
  WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
