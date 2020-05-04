#
# SmoLight main CMake file
#
  
cmake_minimum_required (VERSION 3.10)

include(ProcessorCount)
ProcessorCount(PROCESSORS_NUM)

# Toolchain must come before first project
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain/arm-gcc-toolchain.cmake)

set(TARGET_NAME "bootloader")

project(${TARGET_NAME} C ASM)

# Make sure that the default is a Release or Debug
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: Debug Release"
      FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# Cmake GUI settings
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug)   # Add selectable options to choose build type from CMake GUI
mark_as_advanced(FORCE CMAKE_ECLIPSE_GENERATE_LINKED_RESOURCES)

# Build type
if(CMAKE_BUILD_TYPE MATCHES Release)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Release)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Release/libs)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Debug)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Debug/libs)
endif()

set(DEVICE stm32f103c8t6)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld) # Must be set inside the libopencm3.cmake

# TODO libopencm3


# TODO move -g -gdwarf under the DEBUG checkif
set(TARGET_COMPILE_OPTIONS "-mcpu=cortex-m3 -Og -mthumb -Wall -msoft-float -fdata-sections -ffunction-sections -g -gdwarf-4")
add_definitions(-DSTM32F103xB)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections -specs=nosys.specs -specs=nano.specs -T ${LINKER_SCRIPT}")

message(STATUS "BUILD_TYPE:               " ${CMAKE_BUILD_TYPE})
message(STATUS "TARGET_COMPILE_OPTIONS    " ${TARGET_COMPILE_OPTIONS})
message(STATUS "TARGET_DEFINITION_OPRIONS " ${TARGET_DEFINITION_OPRIONS})
message(STATUS "TARGET_LINK_OPTIONS       " ${TARGET_LINK_OPTIONS})

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(BINARY_NAME ${TARGET_NAME}.bin)

add_library(libopencm3 STATIC IMPORTED)
set_property(TARGET libopencm3 PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libopencm3/lib/libopencm3_stm32f1.a)

# Build main project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source ${EXECUTABLE_OUTPUT_PATH}/)

include(ExternalProject)

ExternalProject_Add(
  proj_libopencm3
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/libopencm3
  BUILD_IN_SOURCE   TRUE
  CONFIGURE_COMMAND ""
  INSTALL_COMMAND   ""
  BUILD_COMMAND     make TARGETS=stm32/f1 -j${PROCESSORS_NUM}
)
