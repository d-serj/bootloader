#
# bootloader main CMake file
#
  
cmake_minimum_required (VERSION 3.14)

# Toolchain must come before first project
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain/arm-gcc-toolchain.cmake
  CACHE FILEPATH "CMake ARM GCC toolchain file" FORCE)

include(ProcessorCount)
ProcessorCount(PROCESSORS_NUM)
set(CMAKE_BUILD_PARALLEL_LEVEL ${PROCESSORS_NUM})

set(TARGET_NAME "bootloader")
set(DEVICE stm32f105rct)

project(${TARGET_NAME} C ASM)

# Make sure that the default is a Release or Debug
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: Debug Release"
      FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# Build type
if(CMAKE_BUILD_TYPE MATCHES Release)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/Release)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/Release/libs)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/Debug)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/Debug/libs)
endif()

# Clean entire build dir with global "clean" command
set_directory_properties(
  PROPERTIES
  ADDITIONAL_CLEAN_FILES ${EXECUTABLE_OUTPUT_PATH}
)

# Cmake GUI settings
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug)   # Add selectable options to choose build type from CMake GUI

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED YES)

# Linker script and .map file
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F1_FLASH.ld)
add_link_options(
  -L ${CMAKE_SOURCE_DIR}/libopencm3/lib
  -Wl,-Map,${EXECUTABLE_OUTPUT_PATH}/${TARGET_NAME}.map
  -T ${LINKER_SCRIPT}
)

message(STATUS "BUILD_TYPE:               :" ${CMAKE_BUILD_TYPE})
message(STATUS "TARGET_COMPILE_OPTIONS    :" ${TARGET_COMPILE_OPTIONS})
message(STATUS "TARGET_DEFINITION_OPTIONS :" ${TARGET_DEFINITION_OPTIONS})
message(STATUS "TARGET_LINK_OPTIONS       :" ${TARGET_LINK_OPTIONS})

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(BINARY_NAME ${TARGET_NAME}.bin)

#
# libopencm3
#
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libopencm3)
  message(FATAL_ERROR "Please, init libopencm3 submodule")
endif()

execute_process(
  COMMAND           make TARGETS=stm32/f1 FP_FLAGS="-mfloat-abi=soft" CFLAGS="-O1"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libopencm3
)

add_library(libopencm3 STATIC IMPORTED)
set_property(TARGET libopencm3 PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libopencm3/lib/libopencm3_stm32f1.a)

# Build main project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source ${EXECUTABLE_OUTPUT_PATH}/source)

include(ExternalProject)

ExternalProject_Add(
  unit_tests
  PREFIX            ${CMAKE_SOURCE_DIR}/test
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/test/build
  BINARY_DIR        ${CMAKE_SOURCE_DIR}/test/build
  STAMP_DIR         ${CMAKE_SOURCE_DIR}/test/build
  TMP_DIR           ${CMAKE_SOURCE_DIR}/test/build
  DOWNLOAD_COMMAND  ""
  PATCH_COMMAND     ""
  INSTALL_COMMAND   ""
  UPDATE_COMMAND    ""
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S ${CMAKE_SOURCE_DIR}/test -G ${CMAKE_GENERATOR}
  BUILD_ALWAYS      TRUE
  ALWAYS            TRUE
  DEPENDS           ${TARGET_NAME}
)

