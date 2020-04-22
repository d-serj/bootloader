#
# SmoLight main CMake file
#
  
cmake_minimum_required (VERSION 3.10)

include(ProcessorCount)
ProcessorCount(PROCESSORS_NUM)

# Toolchain must come before first project
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-gcc-toolchain.cmake)

set(CMAKE_ECLIPSE_MAKE_ARGUMENTS ${CMAKE_ECLIPSE_MAKE_ARGUMENTS} "--no-print-directory -j${PROCESSORS_NUM} -r -s" CACHE STRING "Make parameters")

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

message(STATUS "BUILD_TYPE:                 ${CMAKE_BUILD_TYPE}/${OPTIONS_UI_TYPE}")
message(STATUS "TARGET_COMPILE_OPTIONS    " ${TARGET_COMPILE_OPTIONS})
message(STATUS "TARGET_DEFINITION_OPRIONS " ${TARGET_DEFINITION_OPRIONS})
message(STATUS "TARGET_LINK_OPTIONS       " ${TARGET_LINK_OPTIONS})

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# Configure binary name
if (NOT OPTIONS_DEMO)
  set(BINARY_NAME_SUFFIX "${OPTIONS_UI_TYPE}-${CMAKE_BUILD_TYPE}-${BUILD_NUMBER}")
elseif(OPTIONS_DEMO)
  set(BINARY_NAME_SUFFIX "${OPTIONS_UI_TYPE}-DEMO-${BUILD_NUMBER}")
endif()

set(BINARY_NAME ${TARGET_NAME}_${BINARY_NAME_SUFFIX}.bin)

set(DEVICE_TYPE "SMO" CACHE INTERNAL "Type of the device")

# Build main project
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source ${EXECUTABLE_OUTPUT_PATH})

include(ExternalProject)

ExternalProject_Add(
  libopencm3
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/libopencm3
  BUILD_IN_SOURCE   TRUE
  CONFIGURE_COMMAND ""
  INSTALL_COMMAND   ""
  BUILD_COMMAND     make TARGETS=stm32/f1 -j${PROCESSORS_NUM}
)
