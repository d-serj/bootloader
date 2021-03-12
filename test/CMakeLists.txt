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

find_package(Ruby 2.7 QUIET COMPONENTS Ruby_EXECUTABLE)

if(NOT Ruby_FOUND)
  message(WARNING "Ruby is not installed. Please, download Ruby from https://rubyinstaller.org/downloads/ and install it!\n")
endif(NOT Ruby_FOUND)
# TODO skip farther build process


set(MAIN_PROJ_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../source)

set(MAIN_PROJ_SOURCES
  ${MAIN_PROJ_PATH}/utilities/ringbuffer.c
  ${MAIN_PROJ_PATH}/modem/sim800/sim800.c
  ${MAIN_PROJ_PATH}/crc32.c  
)

set(UNIT_TEST_STUBS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/stubs/libopencm3/cm3/nvic.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/stubs/libopencm3/stm32/usart.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/stubs/libopencm3/stm32/gpio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/stubs/libopencm3/stm32/rcc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/stubs/delay.c
)

set(INCLUDES_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/../source
  ${CMAKE_CURRENT_SOURCE_DIR}/../source/drivers
  ${CMAKE_CURRENT_SOURCE_DIR}/fff
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/stubs
  ${CMAKE_CURRENT_SOURCE_DIR}/Unity
)

macro(add_test_executable TEST_NAME TEST_SOURCES)
  add_executable(${TEST_NAME})

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/${TEST_NAME}_Runner.c
    PRE_BUILD
    COMMAND ${Ruby_EXECUTABLE} generate_test_runner.rb ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/${TEST_NAME}.c
    COMMENT "Generating test runner for ${TEST_NAME}.c ..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Unity/auto
  )

  target_sources(
    ${TEST_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/${TEST_NAME}_Runner.c
    ${TEST_SOURCES}
    ${MAIN_PROJ_SOURCES}
    ${UNIT_TEST_STUBS}
  )

  target_link_libraries(${TEST_NAME} PRIVATE unity)
  target_include_directories(${TEST_NAME} PRIVATE ${INCLUDES_PATH})
  target_compile_definitions(${TEST_NAME} PRIVATE UTEST)
  target_compile_options(${TEST_NAME} PRIVATE -O0 -g3)
  add_test(NAME ${TEST_NAME} COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/${TEST_NAME})

  add_custom_command(
    TARGET ${TEST_NAME}
    POST_BUILD
    COMMAND ${TEST_NAME}${CMAKE_EXECUTABLE_SUFFIX}
    COMMENT "Running ${TEST_NAME} unit tests"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/
  )
endmacro(add_test_executable)

enable_testing()

set(TEST_USART_DRIVER
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/test_usart_driver.c
  ${MAIN_PROJ_PATH}/drivers/usart_driver.c
)

set(TEST_IMAGE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/test_image.c
  ${MAIN_PROJ_PATH}/image.c
)

set(TEST_STORAGE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/test_storage.c
  ${MAIN_PROJ_PATH}/storage.c
  ${MAIN_PROJ_PATH}/drivers/usart_driver.c
)

list(GET TEST_USART_DRIVER 0 UNIT_TEST_SRC)
GET_FILENAME_COMPONENT(UT_TEST_NAME ${UNIT_TEST_SRC} NAME_WE)
add_test_executable(${UT_TEST_NAME} "${TEST_USART_DRIVER}")

list(GET TEST_IMAGE 0 UNIT_TEST_SRC)
GET_FILENAME_COMPONENT(UT_TEST_NAME ${UNIT_TEST_SRC} NAME_WE)
add_test_executable(${UT_TEST_NAME} "${TEST_IMAGE}")

list(GET TEST_STORAGE 0 UNIT_TEST_SRC)
GET_FILENAME_COMPONENT(UT_TEST_NAME ${UNIT_TEST_SRC} NAME_WE)
add_test_executable(${UT_TEST_NAME} "${TEST_STORAGE}")

