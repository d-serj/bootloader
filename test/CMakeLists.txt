##################################
#          Unit tests
##################################

cmake_minimum_required(VERSION 3.15)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_BUILD_PARALLEL_LEVEL 8)

if(SYSTEM_WINDOWS)
  set(CMAKE_EXECUTABLE_SUFFIX ".exe")
elseif(SYSTEM_UNIX)
  set(CMAKE_EXECUTABLE_SUFFIX "")
endif()

project(bootloader_unit_tests C)

include(CTest)
include(GNUInstallDirs)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

# specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Unity ${EXECUTABLE_OUTPUT_PATH}/Unity)

find_package(Ruby)

if(NOT Ruby_FOUND)
  message(WARNING "Ruby is not installed. Please, download Ruby from https://rubyinstaller.org/downloads/ and install it!\n")
endif(NOT Ruby_FOUND)
# TODO skip farther build process


set(MAIN_PROJ_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../source)

set(MAIN_PROJ_SOURCES
  ${MAIN_PROJ_PATH}/image.c
  ${MAIN_PROJ_PATH}/crc32.c
  ${MAIN_PROJ_PATH}/firmware.c
  ${MAIN_PROJ_PATH}/storage.c
)
  
set(UNIT_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/TestImage.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/TestStorage.c
)

set(INCLUDES_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/../source
  ${CMAKE_CURRENT_SOURCE_DIR}/fff
  ${CMAKE_CURRENT_SOURCE_DIR}/Unity
)

macro(add_test_executable FILENAME)
  add_executable(${FILENAME})

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tests/${FILENAME}_Runner.c
    PRE_BUILD
    COMMAND ${Ruby_EXECUTABLE} generate_test_runner.rb ${CMAKE_CURRENT_SOURCE_DIR}/tests/${FILENAME}.c
    COMMENT "Generating test runner for ${FILENAME}.c ..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Unity/auto
  )
  
  target_sources(
    ${FILENAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/${FILENAME}.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/${FILENAME}_Runner.c
    ${MAIN_PROJ_SOURCES}
  )
  target_link_libraries(${FILENAME} PRIVATE unity)
  target_include_directories(${FILENAME} PRIVATE ${INCLUDES_PATH})
  target_compile_options(${FILENAME} PRIVATE -O0 -g3)
  add_test(NAME ${FILENAME} COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/${FILENAME})

  add_custom_command(
    TARGET ${FILENAME}
    POST_BUILD
    COMMAND ${FILENAME}${CMAKE_EXECUTABLE_SUFFIX}
    COMMENT "Running ${FILENAME} unit tests"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/
  )
endmacro(add_test_executable)

foreach(TEST_SRC ${UNIT_TEST_SOURCES})
  GET_FILENAME_COMPONENT(UT_FILENAME ${TEST_SRC} NAME_WE)
  add_test_executable(${UT_FILENAME})
endforeach(TEST_SRC ${UNIT_TEST_SOURCES})

enable_testing()
